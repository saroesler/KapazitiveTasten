/*
 * main.h
 *
 *  Created on: 13.05.2016
 *      Author: sascha
 */

#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 8000000

//#define DEBUG_MAIN
/************************************************************************
 * Aktiviert den UART.
 */
#ifdef DEBUG_MAIN
#include "serial.h"
#endif /* DEBUG_MAIN */

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>		//für itoa

#include "tasten.h"
#include "pwm.h"
#include "ausgabe.h"

//Betriebs LED
#define SYS_PORT PORTD
#define SYS_DDR DDRD
#define SYS_PIN PIND
#define LED PD7


#endif /* MAIN_H_ */
