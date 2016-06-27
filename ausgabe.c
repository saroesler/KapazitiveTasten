/*
 * ausgabe.c
 *
 * Created: 07.06.2016 17:46:17
 *  Author: campe
 */ 
#include "ausgabe.h"



volatile static uint8_t ausgabe = 0;

ISR(TIMER0_OVF_vect) {
	if(ausgabe & (1<<T_HOCH) && PWM_LICHT < 255) {
		PWM_LICHT++;
	}
	if(ausgabe & (1<<T_RUNTER) && PWM_LICHT) {
		PWM_LICHT--;
	}
	if(ausgabe & (1<<T_RECHTS) && PWM_AUDIO < 255) {
		PWM_AUDIO++;
	}
	if(ausgabe & (1<<T_LINKS) && PWM_AUDIO) {
		PWM_AUDIO--;
	}	
}

void init_aus() {
	AUS_DDR = 0xFF;
	TIMSK |= (1<<OCIE0);
	sei();
}

void aus_refrech() {
	if(ausgabe) {
		AUS_SYS_PORT |= (1<<AUS_TAST_TON);
	} else {
		AUS_SYS_PORT &= ~(1<<AUS_TAST_TON);
	}
	if(AUS_SYS_PIN & (1<<AUS_MAN_P)) { 
		AUS_PORT = ausgabe;
		AUS_TIMER_AUS;
	} else { //Blockade aller Pfeiltasten
		AUS_PORT = ausgabe & MAN_AUS_MASK;
		//Lautstärke und Helligkeit
		if(ausgabe & T_PFEILMASKE) {
			AUS_TIMER_AN;
		} else {
			AUS_TIMER_AUS;
		}
	}
}

void aus_set(uint8_t val) {
	ausgabe = val;
	aus_refrech();
}
