/*
 * intervelometer.c
 *
 * Created: 18-Sep-21 04:51:23 PM
 * Author : PAWAN
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

#define PIN_PB0	0
#define PIN_PB1	1
#define PIN_PB2	2
#define PIN_PB3	3

void gpioConfig(void);
void interruptConfig(void);
void timerZeroConfig(void);
void startup(void);

uint8_t timerCounter;
uint8_t interruptCounter = 0x00;

int main(void){
    /* Replace with your application code */
			CCP = 0xD8;	  //unlock protected registers
			CLKMSR =0x00;
			CLKPSR = 0;	  // No clock divider
			
	gpioConfig();
	startup();
	interruptConfig();
	timerZeroConfig();
	

		
    while (1) 
    {
		
		
    }
}

void gpioConfig(void){
	
	DDRB |= (1<<PIN_PB1);	/*PB1 pin as output*/
	DDRB |= (1<<PIN_PB0);	/*PB0 pin as output*/	
	DDRB &= ~(1<<PIN_PB2);	/*PB2 pin as input*/
	PUEB |= (1<<PIN_PB2);	/*PB2 pin pull up internally*/
}

void interruptConfig(void){
	
	sei(); /*global interrupt enable*/
	EICRA = 0x00; /*interrupt  low on level */
	EIMSK = 0x01; /*external interrupt enable*/
	EIFR = 0x00;/*external interrupt flag cleared*/
	
	TIMSK0 = 0x01; /*timer0 overflow interrupt enable*/
	TIFR0 &= ~(1<<TOV0); /*timer0 overflow interrupt flag clear*/
	
}

void timerZeroConfig(void){
	
	TCCR0A = 0x00; /*Timer0 normal mode operation*/
}

void startup(void){
	
	EIMSK = 0x00; /*external interrupt disable*/
	
	for(uint8_t i = 0 ; i<20 ; i++){
		PORTB ^= (1<<PIN_PB0);
		_delay_ms(20);
	}
}

ISR( INT0_vect ){
	
	TCNT0H = 0x00;	/*loaded 0xE174 value 1sec time interval*/
	TCNT0L = 0x00;


	TCCR0B = 0x01;/*start timer with prescalar 8*/
		
	
	EIFR = 0x00;/*external interrupt flag cleared*/
}

ISR( TIM0_OVF_vect ){
	
	timerCounter++;
		if(timerCounter == 30){
			PORTB ^= (1<<PIN_PB1);
			PORTB ^= (1<<PIN_PB0);
			timerCounter = 0;
		}
	 
				TIFR0 &= ~(1<<TOV0) ;
				TCNT0H = 0x00;	/*loaded 0xE174 value 1sec time interval*/
				TCNT0L = 0x00;
}