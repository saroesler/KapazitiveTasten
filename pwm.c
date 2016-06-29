/*
 * CFile1.c
 *
 * Created: 07.06.2016 16:10:50
 *  Author: campe
 */ 
#include "pwm.h"

void init_PWM() {
	DDRD |= (1<<PD4)|(1<<PD5);
	TCCR1A |= (1<<WGM10)|(1<<COM1A1)|(1<<COM1B1);
	TCCR1B |= (1<<WGM12)|(1<<CS10);
	OCR1A = 200;
	OCR1B = 150;
}
