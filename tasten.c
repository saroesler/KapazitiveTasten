/*
 * capazitiveTasten.c
 *
 *  Created on: 13.05.2016
 *      Author: sascha
 */
#include "tasten.h"

static uint8_t laufzeit_buffer[CAP_NUM][128];
static uint8_t counter = 0;
static uint8_t status = 0;
static uint8_t tasten_ausgabe = 0;
static int8_t abweich_count[CAP_NUM];
static uint8_t zuordnung[8] = {T_WECKER,T_MENUE,T_RUNTER,T_RECHTS,T_HOCH,T_SNNOZE,T_LINKS,T_ENTER};


void tasten_init() {
	#ifdef DEBUG_TASTEN
	uart_init();
	dbFlagDDR |= (1<<dbFlagPin);
	#endif
	for (uint8_t ii = 0; ii<CAP_NUM; ii++) {
		for (uint8_t jj = 0; jj<128; jj++) {
			laufzeit_buffer[ii][jj] = 0;
		}
		abweich_count[ii] = 0;
	}	
}

void capStart(void){
	//Caps entlerren
	#ifdef DEBUG_TASTEN
	dbFlagPORT |= (1<<dbFlagPin);
	#endif /* DEBUG_TASTEN */
	CAP_DDR0 = 0xff;
	CAP_DDR1 = 0xff;

	CAP_PORT0 = 0;
	CAP_PORT1 = 0;
	
	for(uint8_t ii = 0; ii<100; ii++);
	#ifdef DEBUG_TASTEN
	dbFlagPORT &= ~(1<<dbFlagPin);
	#endif /* DEBUG_TASTEN */
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

uint8_t readCap(void){
	
	//Mittelwerte berrechnen
	uint8_t avg[8];
	for (uint8_t ii = 0; ii<CAP_NUM; ii++) {
		uint16_t avg_temp = 0;
		for(uint8_t jj = 0; jj<128; jj++) {
			avg_temp += laufzeit_buffer[ii][jj];
		}
		avg[ii] = (avg_temp << 7);
		//Spalte initialisieren
		laufzeit_buffer[ii][counter] = 0;
	}
	
	uint8_t ready = 0;
	uint8_t zykles = 0;

	//speicher für die Laufzeit
	
	//caps entleeren
	capStart();

	//bis alle caps voll sind
#ifdef DEBUG_EINE_TASTE
	while(ready & 0x01){
#else
	while(ready == 0xff){
#endif
		//Sensor aufladen
		capLaden();

		//kurz warten
		for(uint8_t i = 0; i < 100; i ++);

		//sendor leeren
		capLeeren();

		//Ladung messen
		volatile uint8_t * pinptr = &CAP_PIN0;
		for(uint8_t i = 0; i < CAP_NUM; i ++){
			if(i == 4)
				pinptr = &CAP_PIN1;

			if( ((*pinptr) & (1<< ((i >> 1)%4)))
					&& laufzeit_buffer[i][counter] == 0)
				laufzeit_buffer[i][counter] = zykles;
		}
		zykles++;
	}
	
#ifdef DEBUG_TASTEN
	char buffer[5];
	itoa(laufzeit_buffer[0][counter], buffer ,10);
	uart_putstring((uint8_t*)buffer);
	uart_putc('\n');
#endif
	//Auswertung
	for(uint8_t ii = 0; ii<CAP_NUM; ii++) {
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

	counter++;

	return tasten_ausgabe;
}
