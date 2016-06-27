/*
 * serial.h
 *
 * Created: 07.06.2016 16:15:30
 *  Author: campe
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 8000000 /* evtl. bereits via Compilerparameter definiert */
#endif
#define BAUD 9600
#include <util/setbaud.h>

void uart_init();
void uart_putc(uint8_t c);
void uart_putstring(uint8_t *s);

#endif /* SERIAL_H_ */
