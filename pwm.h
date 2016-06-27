/*
 * pwm.h
 *
 * Created: 07.06.2016 16:11:06
 *  Author: campe
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

#define PWM_AUDIO OCR1A
#define PWM_LICHT OCR1B

void init_PWM();

#endif /* PWM_H_ */