#include "motor.h"
#include "gpio.h"


void DcMotor_Init(void){

	GPIO_setupPinDirection( MOTOR_PORT,INPUT1_PIN,PIN_OUTPUT);
	GPIO_setupPinDirection( MOTOR_PORT,INPUT2_PIN,PIN_OUTPUT);

	GPIO_writePin(MOTOR_PORT,INPUT1_PIN,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT,INPUT2_PIN,LOGIC_LOW);

}

void DcMotor_Rotate(DcMotor_State state){

	if(state== OFF){
		GPIO_writePin(MOTOR_PORT,INPUT1_PIN,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT,INPUT2_PIN,LOGIC_LOW);

	}

	else if(state == CW){

		GPIO_writePin(MOTOR_PORT,INPUT1_PIN,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT,INPUT2_PIN,LOGIC_LOW);

	}

	else if(state == ACW){
		GPIO_writePin(MOTOR_PORT,INPUT1_PIN,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT,INPUT2_PIN,LOGIC_HIGH);

	}



}
