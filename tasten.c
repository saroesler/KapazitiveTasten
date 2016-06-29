/*
 * capazitiveTasten.c
 *
 *  Created on: 13.05.2016
 *      Author: sascha
 */
#include "tasten.h"

static uint8_t laufzeit_buffer[CAP_NUM][BUFFER_SIZE];
static uint8_t counter = 0;
static uint8_t status = 0;
static uint8_t tasten_ausgabe = 0;
static int8_t abweich_count[CAP_NUM];
static uint8_t zuordnung[8] = {T_WECKER,T_MENUE,T_RUNTER,T_RECHTS,T_SNNOZE,T_HOCH,T_LINKS,T_ENTER};


void tasten_init() {
	#ifdef DEBUG_TASTEN
	uart_init();
	dbFlagDDR |= (1<<dbFlagPin);
	#endif
	for (uint8_t ii = 0; ii<CAP_NUM; ii++) {
		for (uint8_t jj = 0; jj<BUFFER_SIZE; jj++) {
			laufzeit_buffer[ii][jj] = 0;
		}
		abweich_count[ii] = 0;
	}	
}

void capStart(void){
	//Caps entlerren
	CAP_DDR0 = 0xff;
	CAP_DDR1 = 0xff;

	CAP_PORT0 = 0;
	CAP_PORT1 = 0;
	
	for(uint8_t ii = 0; ii<100; ii++);
}

void capLaden(void){
	//Caps entlerren
	CAP_PORT0 |= MASK_VERSORGER0;
	CAP_DDR0 = MASK_VERSORGER0;
	CAP_PORT0 = MASK_VERSORGER0; //Braucht man glaub ich nicht. Mal ausprobieren. Oben entsprechend |= zu =.
	
	CAP_PORT1 |= MASK_VERSORGER1;
	CAP_DDR1 = MASK_VERSORGER1;
	CAP_PORT1 = MASK_VERSORGER1; //Braucht man glaub ich nicht. Mal ausprobieren	
}

void capLeeren(void){
	//Caps entlerren
	CAP_DDR0 = MASK_SENSOR0;
	CAP_PORT0 = 0x00;
	
	CAP_DDR1 = MASK_SENSOR1;
	CAP_PORT1 = 0x00;
}

uint8_t readOneCap(uint8_t ii, volatile uint8_t * pinptr){
	//initialisieren
	capStart();

	uint8_t mask = (1 << ((ii * 2)%8 + 1));
	uint8_t zykles = 0;

	while(zykles < 255){
		//Sensor aufladen
		capLaden();

		//kurz warten
		for(uint8_t i = 0; i < 200; i ++);

		//sendor leeren
		capLeeren();

		//Ladung messen
		if((*pinptr) & mask)
			return zykles;
		zykles ++;

	}
	return 255;
}

uint8_t readCap(void){
	
	//Mittelwerte berrechnen
	uint8_t avg[8];
	for (uint8_t ii = 0; ii<CAP_NUM; ii++) {
		int32_t avg_temp = 0;
		for(uint8_t jj = 0; jj<BUFFER_SIZE; jj++) {
			avg_temp += laufzeit_buffer[ii][jj];
		}
		avg[ii] = (avg_temp / BUFFER_SIZE);


		//Spalte initialisieren
		laufzeit_buffer[ii][counter] = 0;
	}
	

	volatile uint8_t * pinptr = &CAP_PIN0;

	//caps nacheinander lesen
	for(uint8_t ii = 0; ii < CAP_NUM; ii ++){
		if(ii == 4)
			pinptr = &CAP_PIN1;
		laufzeit_buffer[ii][counter] = readOneCap(ii, pinptr);
	}

#ifdef DEBUG_TASTEN
	uart_putc(100);
	for(uint8_t i = 0; i < CAP_NUM; i ++){
		uart_putc(laufzeit_buffer[i][counter]);
		uart_putc(avg[i]);
	}
#endif

#ifdef DEBUG_TASTEN
	dbFlagPORT |= (1<<dbFlagPin);
#endif /* DEBUG_TASTEN */

	//Auswertung
	for(uint8_t ii = 0; ii<CAP_NUM; ii++) {
		//wenn Pin an, dann prüfe, ob laufzeit wieder steigt
		if(status & (1<<ii)) {
			if (laufzeit_buffer[ii][counter] > (avg[ii] + CAP_TOLERANZ)) {
				abweich_count[ii]++;
			} else {
				abweich_count[ii] = 0;
			}
			if (abweich_count[ii] > CAP_COUNT_MAX) {
				abweich_count[ii] = 0;
				status &= ~(1<<ii);
				tasten_ausgabe &= ~(1<<zuordnung[ii]);
			}
		//wenn Pin aus, dann prüfe, ob laufzeit singt
		} else {
			if (laufzeit_buffer[ii][counter] < (avg[ii] - CAP_TOLERANZ)) {
				abweich_count[ii]--;
			} else {
				abweich_count[ii] = 0;
			}
			if (abweich_count[ii] < CAP_COUNT_MIN) {
				abweich_count[ii] = 0;
				status |= (1<<ii);
				tasten_ausgabe |= (1<<zuordnung[ii]);
			}
		}
	}

#ifdef DEBUG_TASTEN
	dbFlagPORT &= ~(1<<dbFlagPin);
#endif /* DEBUG_TASTEN */

#ifdef DEBUG_TASTEN
	if(status & (0xff)){	//Wenn erste Taste gedrückt
		SYS_PORT &= ~(1 << LED);	//LED aus
	} else {
		SYS_PORT |= (1 << LED);		//LED an
	}
#endif

	counter++;

	//reset counter, wenn Maxwert erreicht
	if(counter == BUFFER_SIZE)
		counter = 0;

	return tasten_ausgabe;
}
