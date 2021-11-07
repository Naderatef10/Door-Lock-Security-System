/************************************************************************************************************

 	 	 	 	 	 	 	 AUTHOR: Nader Atef
 	 	 	 	 	 	 	 MODULE NAME: BUZZER
 	 	 	 	 	 	 	 MODULE TYPE : HEADER FILE
 ************************************************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_




/************************************************************************************************************
  	  	  	  	  	  	  	  INCLUDED MODULES and macro definitions

 ************************************************************************************************************/

#include "std_types.h"

#define  BUZZER_PORT_ID  PORTA_ID

#define  BUZZER_PIN_ID   PIN0_ID

/************************************************************************************************************
  	  	  	  	  	  	  	  Functions Prototypes

 ************************************************************************************************************/

/*function to initialize buzzer driver*/
void BUZZER_init();

/*function to turn on the buzzer*/
void BUZZER_on();

/*function to turn off the buzzer*/
void BUZZER_off();




#endif /* BUZZER_H_ */
