/************************************************************************************************************

 	 	 	 	 	 	 	 AUTHOR: Nader Atef
 	 	 	 	 	 	 	 MODULE NAME: BUZZER
 	 	 	 	 	 	 	 MODULE TYPE : C FILE

 ************************************************************************************************************/
#include "buzzer.h"
#include "gpio.h"

void BUZZER_init(){
 /*setting buzzer pin direction as output*/
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);


}

void BUZZER_on(){
/*writting high on buzzer pin*/
 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);

}

void BUZZER_off(){
	/*writting low on buzzer pin*/
	 GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);

}
