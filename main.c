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

	while(1){
        aus_set(readCap());
	}
	
	return 1;
}
