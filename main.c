/*
 * main.h
 *
 *  Created on: 13.05.2016
 *      Author: sascha
 */
#include "main.h"


void init(void){
	init_PWM();
	init_aus();
	tasten_init();
	#ifdef DEBUG_MAIN
	uart_init();
	#endif // DEBUG

	DDRB = 0xff;
	PORTB = 0xff;
	
	
	SYS_DDR  |= (1 << LED);
	SYS_PORT |= (1 << LED);

	//DDRD = 0xFF;//(1 << PD6);

}

int main(void){

	init();
	//SYS_DDR |=(1<<PD6);
	//SYS_PORT |=(1<<PD6);
	while(1){
		//uart_putc('a');
        aus_set(readCap());
		//uart_putc(readCap());
		uart_putc('\n');
	}
	
	return 1;
}
