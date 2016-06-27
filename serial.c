/*
 * serial.c
 *
 * Created: 07.06.2016 16:15:03
 *  Author: campe
 */ 
#include "serial.h"

void uart_init() {
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	#if USE_2X
	/* U2X-Modus erforderlich */
	UCSRA |= (1 << U2X);
	#else
	/* U2X-Modus nicht erforderlich */
	UCSRA &= ~(1 << U2X);
	#endif
	/* Enable receiver and transmitter */
	UCSRB |= (1<<TXEN);

	/*Asynchron Modus*/
	UCSRC &= ~(1<<UMSEL);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	//UCSRC |= (1<<UCSZ0)|(1<<UCSZ1);
}

void uart_putc(uint8_t c) {
	while (!(UCSRA & (1<<UDRE)));
	UDR = c;
}

void uart_putstring(uint8_t *s) {
	while (*s) {
		uart_putc(*s);
		s++;
	}
}
