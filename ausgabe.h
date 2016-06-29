/*
 * ausgabe.h
 *
 * Created: 07.06.2016 17:46:02
 *  Author: campe
 */ 


#ifndef AUSGABE_H_
#define AUSGABE_H_

#include <avr/io.h>
#include <stdint.h>
#include "pwm.h"
#include <avr/interrupt.h>
#include "tasten.h"

#define AUS_PORT PORTB
#define AUS_DDR DDRB
#define AUS_PIN PINB

#define AUS_SYS_PORT PORTD
#define AUS_SYS_DDR DDRD
#define AUS_SYS_PIN PIND
#define AUS_MAN_P PD3
#define AUS_TAST_TON PD2

#define AUS_TIMER_AN TCCR2 = (1<<CS22)|(1<<CS21)
#define AUS_TIMER_AUS TCCR2 = 0




// Freigegbene Tasten trotz Men�sperre
#define MAN_AUS_MASK ~T_PFEILMASKE

//extern uint8_t ausgabe;
void aus_set(uint8_t val);
void init_aus();

#endif /* AUSGABE_H_ */
