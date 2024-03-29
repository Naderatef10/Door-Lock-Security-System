/************************************************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer0.c
 *
 * Description: source file for the AVR timer0 driver
 *
 * Author: Nader Atef
 *
 ************************************************************************************************************/

#include "timer0.h"
#include <avr/interrupt.h>
/*pointer to function in order to do the call back notification(calling the call back function of timer)*/
static volatile void (*callback_ptr)(void)=NULL_PTR;

/*TIMER0_init is a function to initialize the timer
 * it takes a pointer to the configuration structure to dynamically configure the timer
 *
 * */
void TIMER0_init(TIMER0_ConfigType* configptr){
	/*if the mode chosen is compare mode enter the if block else if mode is over flow enter the else*/
	if(configptr->timer0_mode==COMPARE_MODE){
		/*(1<<FOC0) NON PWM MODE
		 * (1<<WGM01) compare mode
		 * OCR0 = configptr->compare_value choosing cerating compare value and store it in OCR0
		 * */
		TCCR0= (1<<WGM01) | (1<<FOC0);
		/*selecting the timer initial value*/
		TCNT0 = configptr->timer_initial_value;
		OCR0 = configptr->compare_value;
		/*masking certain bits in order to choose the required prescalar*/
		TCCR0 = (TCCR0 & 0xF8) | (configptr->prescalar);
		/*enabling Output compare interrupt enable */
		TIMSK |= (1<<OCIE0);
		/*choosing what port operation to do on OCR0 on comparematch*/
		TCCR0 = (TCCR0 & 0xCF) | (configptr->compare_mode << 4);

	}
	else{
		/*non pwm mode (1<<FOC0)

		 */
		TCCR0 = (1<<FOC0);
		TCNT0 = configptr->timer_initial_value;
		/*enabling the timer overflow interrupt enable*/
		TIMSK |= (1<<TOIE0);
		TCCR0 = (TCCR0 & 0xF8) | (configptr->prescalar);

	}

}

/*timer0 call back function to be later called in the ISR*/
void TIMER0_setcallback(void (*a_ptr)(void)){
	callback_ptr=a_ptr;

}

/*function to deinitialize timer*/
void TIMER0_deinit(){

	TCCR0=0;
	TCNT0=0;
	OCR0=0;

}
/*ISR of timer0 over flow mode*/

ISR(TIMER0_OVF_vect){
	/*calling using pointer to function*/
	if(callback_ptr != NULL_PTR){
		(*callback_ptr)();
	}

}
/*ISR of timer0 compare mode*/
ISR(TIMER0_COMP_vect){
	if(callback_ptr!=NULL_PTR){
		(*callback_ptr)();
	}

}
