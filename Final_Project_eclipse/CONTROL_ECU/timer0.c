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
static volatile void (*callback_ptr)(void) = NULL_PTR;

/*initilization function for the timer driver*/
void TIMER0_init(TIMER0_ConfigType* configptr){

	/*conditional for comparing the timer mode in the configuration structure*/
	if(configptr->timer0_mode==COMPARE_MODE){
		/*initilizaing timer in compare mode
		 setting force compare mode (1<<fOC0)
		 setting WGMO01 to activate CTC mode
		 * */
		TCCR0= (1<<WGM01) | (1<<FOC0);
		/*assigning the initial value of the timer*/
		TCNT0 = configptr->timer_initial_value;
		/*choosing the compare value*/
		OCR0 = configptr->compare_value;
		/*masking CS00 and CS01 and CS02 to select the required prescaling factor*/
		TCCR0 = (TCCR0 & 0xF8) | (configptr->prescalar);
		/*enabling Timer/Counter0 Output Compare Match Interrupt Enable*/
		TIMSK |= (1<<OCIE0);
		/*choosing the mode of operation of OCR0
		 * 1)Normal port operation, OC0 disconnected.
		 * 2)Toggle OC0 on compare match
		 * 3)Clear OC0 on compare match
		 * 4)Set OC0 on compare match
		 * */
		TCCR0 = (TCCR0 & 0xCF) | (configptr->compare_mode << 4);

	}
	else{
		/*initialzing the timer in over flow mode
		 * WGM00 = 0
		 * WGM00 = 0
		 * FOC0 = 1 (because a non-pwm mode)
		 * */
		TCCR0 = (1<<FOC0);
		/*assigning the initial value of the timer*/
		TCNT0 = configptr->timer_initial_value;

		/*masking CS00 and CS01 and CS02 to select the required prescaling factor*/
		TIMSK |= (1<<TOIE0);

		/*masking CS00 and CS01 and CS02 to select the required prescaling factor*/
		TCCR0 = (TCCR0 & 0xF8) | (configptr->prescalar);

	}

}

/*function to set the call back function of timer0*/
void TIMER0_setcallback(void (*a_ptr)(void)){

	callback_ptr=a_ptr;

}

/*function to stop the timer*/
void TIMER0_deinit(){

TCCR0=0;
TCNT0=0;
OCR0=0;

}
/*interrupt service routine of timer0 in overflow mode

 * */
ISR(TIMER0_OVF_vect){
	if(callback_ptr != NULL_PTR){
		(*callback_ptr)();
	}

}
/*interrupt service routine of timer0 in compare mode*/
ISR(TIMER0_COMP_vect){
	if(callback_ptr!=NULL_PTR){
		(*callback_ptr)();
	}

}
