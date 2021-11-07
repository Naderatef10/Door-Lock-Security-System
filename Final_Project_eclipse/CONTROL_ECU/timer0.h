 /************************************************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer0.h
 *
 * Description: Header file for the AVR timer0 driver
 *
 * Author: Nader Atef
 *
 ************************************************************************************************************/
#ifndef TIMER0_H_
#define TIMER0_H_



#include "std_types.h"

/*************************************************************************************************************
 *                                Definitions                                  *
 ************************************************************************************************************/


/*************************************************************************************************************
 *                               Types Declaration                             *
 ************************************************************************************************************/

/*enum to determine the type of operation on OC0 port*/
typedef enum{

	NORMAL,TOGGLE,CLEAR,SET

}TIMER0_compareMode;


/*enum to determine the type of mode in which the timer is operated either overflow mode or compare mode*/
typedef enum{
	OVERFLOW,COMPARE_MODE=2

}TIMER0_mode;
/*enum to determine the prescalar of the frequency of the timer*/
typedef enum{

 NOCLOCK,FCPU,FCPU_8,FCPU_64,FCPU_256,FCPU_1024

}TIMER0_prescalar;

/*Configuration structure of timer0 to dynamically configure timer to different modes*/
typedef struct{
	/*enum variable to determine opration of oc0*/
    TIMER0_compareMode compare_mode;
	/*unsigned character to calculate the timer initial value*/
    uint8 timer_initial_value;
	/*unsigned character to store the value of OCR0*/
    uint8 compare_value;
	/*enum variable to determine the prescalar required */
    TIMER0_prescalar prescalar;
    /*enum variable to determine timer0 mode of operation*/
    TIMER0_mode  timer0_mode;


}TIMER0_ConfigType;

/************************************************************************************************************
 *                              Functions Prototypes                           *
 ***********************************************************************************************************/

/*TIMER0_init is a function to initialize the timer0 module it takes pointer to the configuration structure
 to dynamically configure the driver
 */
void TIMER0_init(TIMER0_ConfigType*);

/*TIMER0_setcallback function is a function to set the call back function designed in the upper layer module
 (Application layer) to be called in the ISR using pointer to function. Therefore, it takes a pointer to,
 function as an input argument
 * */
void TIMER0_setcallback(void (*a_ptr)(void));

/*FUNCTON to stop timer0*/
void TIMER0_deinit();


#endif /* TIMER0_H_ */
