/**************************************************************************************
 * AUTHOR : Nader Atef Labib
 *
 * file name: control_ecu.c
 * file type: source code
 * PROJECT description: door lock security system it consists of 2 microcontrollers
 * (HMI and CONTROL UNIT) which communicate together with UART
 *
 * this is the source code file for the control_ecu microcontroller

 *
 *
 *
 *
 * Kindly check the header file control_ecu.h because macrodefinitions and functions,
 * are include in that file.
 **************************************************************************************/




/***************************************************************************************
 *
 * 					Included modules in the project
 *
 *
 **************************************************************************************/

#include "external_eeprom.h"
#include "motor.h"
#include "timer0.h"
#include "buzzer.h"
#include "twi.h"
#include "uart.h"
#include "util/delay.h"
#include "gpio.h"
#include <avr/io.h>
#include "control_ecu.h"

/****************************************************************************************
 *
 * 							Global variables
 *
 * *************************************************************************************/

/*door_flag is a global variable used as a flag when opening the door
 * when the password has been entered correctly by the user
 *
 * */
uint8 door_flag=1;
/*array used to store the password of the door*/
uint8 Door_password[5];
/*variable used to store the number of timer overflows*/
volatile uint32 timer_overflows=0;



/********************************************************************************************************
 *
 * 								Main application code
 *
 *
 * *****************************************************************************************************/


int main(){
	/*checking if the system was running for the first time*/
	uint8 required_function;
	drivers_init();
	if(check_first_time()){

		Change_Password();
	}


	/*handshaking between the 2 controllers*/
	UART_sendByte(HAND_SHAKE);
	while(1)
	{
		/*receiving which function is needed by HMI*/
		required_function = recieve_function();

		/*comparing the macro flag with the recieved macro*/
		if(required_function == CHANGE_PASSWORD){
			Change_Password();
		}

		else if(required_function == CHECK_PASSWORD){
			Check_Password();

		}
		else if(required_function == OPEN_DOOR){
			open_door();
		}

		else if(required_function == THEIF_ALARM){

			theif_function();
		}



	}
	return 0;

}
/*drivers_init() is a function to initialize the drivers needed in the application*/
void drivers_init(){
	/*enabling the global interrupt enable*/
	SREG |= (1<<7);
	/*initializing motor*/
	DcMotor_Init();
	/*initializing buzzer */
	BUZZER_init();
	/*initializing I2C in Normal mode and address of the controller in case of being addressed
	 * as a slave
	 * */
	TWI_ConfigType t1= {NORMAL_MODE,0x05};
	TWI_init(&t1);
	/*dynamically configuring UART to settings
	 * baud rate: 9600
	 * Parity : disabled
	 * number of stop bits: one
	 * word size: eight bits
	 * */
	UART_ConfigType u1 = {9600,PARITY_OFF,STOP_ONE,EIGHT_BITS};
	UART_init(&u1);

}

/*function used to receive the needed function from the HMI using UART*/
uint8 recieve_function(){

	//while(UART_recieveByte()!= HAND_SHAKE){};
	return  UART_recieveByte() ;

}

/*function used to change the password of the system*/
void Change_Password(){

	/*flag used to determine if the 2 passwords entered match or not*/
	uint8 check_match_condition;
	uint8 i=0;

	/*synchronization technique to ensure that the 2 controllers are ready to transmit and
	 * recieve at the same time
	 * */
	while(UART_recieveByte() != CHANGE_PASSWORD ){};
	UART_sendByte(CHANGE_PASSWORD);

	do{

		check_match_condition=1;
		/*recieving the password for the first time*/
		for(i=0;i<5;i++){
			UART_sendByte(HAND_SHAKE);
			Door_password[i]=  UART_recieveByte();

		}

		/*recieving the password for the second time*/
		for(i=0;i<5;i++){


			UART_sendByte(HAND_SHAKE);
			/*comparing the password entered in the first time with the password entered in the second
			 * time*/
			if( UART_recieveByte() != Door_password[i] ){
				/*if the 2 passwords doesn't match repeat the do while loop*/
				check_match_condition=0;
			}
			else{
				/*if the 2 passwords match store the password in the EEPROM*/
				EEPROM_writeByte(LOW_ADDRESS+i,Door_password[i]);
			}

		}


		while(UART_recieveByte() != HAND_SHAKE){}
		UART_sendByte(check_match_condition);


	}while(!check_match_condition);
	EEPROM_writeByte(START_UP_ADDRESS,START_UP_CHECK);


}
/*function used to check if it is the first time to run the system*/
uint8 check_first_time(){

	/*saved_number is a variable used to store the value read from the EEPROM*/
	uint8 saved_number;
	/*read from the start up address(default address included in headerfile) in the EEPROM*/
	EEPROM_readByte(START_UP_ADDRESS,&saved_number);
	/*check if the saved number equals the default number choosen to be stored in case of the
	 * system runned before
	 * */
	if(saved_number == START_UP_CHECK){
		while(UART_recieveByte() != HAND_SHAKE){}
		/*send zero if it is not the first time to run the system*/
		UART_sendByte(0);
		return 0;
	}
	else{
		while(UART_recieveByte() != HAND_SHAKE){}
		/*send one if it is the first time to run the system*/
		UART_sendByte(1);
		return 1;

	}

}


void Check_Password(){
	/*check_match_condition is a flag to break the infinite loop in case the,
	 * 2 passwords match
	 * */
	uint8 check_match_condition;
	/*loop counter*/
	uint8 i=0;
	/*variable used to save the byte which we read from the EEPROM*/
	uint8 byte;
	/*variable used to count the number of times the user entered the password wrong*/
	uint8 wrong_counter=0;

	/*synchronization technique to ensure that the 2 controllers send and recieve the two microcontrollers
	 * at the same time
	 * */
	while(UART_recieveByte() != CHECK_PASSWORD ){};
	UART_sendByte(CHECK_PASSWORD);


	do{

		check_match_condition=1;
		/*loop to compare between the password stored in the EEPROM and the password
		 * the user enters*/
		for(i=0;i<5;i++){
			/*reading the password from the EEPROM*/
			EEPROM_readByte(LOW_ADDRESS+i,&byte);
			UART_sendByte(HAND_SHAKE);
			if(	 UART_recieveByte() !=  byte){
				check_match_condition=0;

			}

		}
		if(check_match_condition==0){
			/*increase the counter in case of entering the password wrong*/
			wrong_counter++;
		}


		/*if the password was entered by the user wrong 3 times
		 * send the FATAL_ERROR byte which indicates a thieft attempt
		 * */
		if(wrong_counter==3){
			while(UART_recieveByte() != HAND_SHAKE){}
			UART_sendByte(FATAL_ERROR);
			check_match_condition=1;


		}
		else{
			while(UART_recieveByte() != HAND_SHAKE){}
			UART_sendByte(check_match_condition);

		}


	}while(!check_match_condition);



}

/*function to initialize timer0 mode of operations and its's corresponding callback function*/
void init_timer(uint8 mode){
	if(mode==MODE_OPENING_DOOR){
		/*count 15 seconds
		 * Timer calculations:
		 * FCPU= 8MHZ
		 * FTIMER= 7812.5 HZ
		 * TOVERFLOW= 0.032768 SECS
		 * NUMBER OF OVER FLOWS PER 15 SECONDS: 458
		 * */
		TIMER0_ConfigType t1= {NORMAL,0,0,FCPU_1024,OVERFLOW};
		TIMER0_setcallback(timer0_processing);
		TIMER0_init(&t1);
	}
	else if(mode == MODE_ALARM){
		/*count 1 minute
		 * Timer calculations:
		 * FCPU= 8MHZ
		 * FTIMER= 7812.5 HZ
		 * TOVERFLOW= 0.032768 SECS
		 * NUMBER OF OVER FLOWS PER 1 minute : 1832
		 * */
		TIMER0_ConfigType t1= {NORMAL,0,0,FCPU_1024,OVERFLOW};
		TIMER0_setcallback(alarm_callback);
		TIMER0_init(&t1);
	}
	else if(mode==MODE_MOTOR_REST){
		/*count 3 seconds
		 * Timer calculations:
		 *FCPU= 8MHZ
		 * FTIMER= 7812.5 HZ
		 * TOVERFLOW= 0.032768 SECS
		 * NUMBER OF OVER FLOWS PER 15 SECONDS: 92
		 * */
		TIMER0_ConfigType t1= {NORMAL,0,0,FCPU_1024,OVERFLOW};
		TIMER0_setcallback(timer_stopmotor);
		TIMER0_init(&t1);
	}
}

/*function used to open the door if the password was entered correctly by the user and the user
 * choose the + option in the main menu
 * */
void open_door(){

	/*synchronization technique between the 2 microcontrollers*/
	while(UART_recieveByte() != OPEN_DOOR ){};
	UART_sendByte(OPEN_DOOR);

	/*initialize the timer to count 15 seconds*/
	init_timer(MODE_OPENING_DOOR);
	/*start the motor with direction clockwise to open the door*/
	DcMotor_Rotate(CW);

	/*freeze the system until the door opens*/
	while(door_flag){};
	door_flag=1;
	/*initialize the timer to count 3 seconds*/
	init_timer(MODE_MOTOR_REST);
	/*turn the motor off to hold the door opened for 3 seconds*/
	DcMotor_Rotate(OFF);
	/*freeze the system till the 3 seconds pass*/
	while(door_flag){};
	door_flag=1;
	/*initialize the timer to count 15 seconds*/
	init_timer(MODE_OPENING_DOOR);
	/*turn the motor to rotate anticlock wise to close the door*/
	DcMotor_Rotate(ACW);

	/*keep polling till the 15 seconds pass*/
	while(door_flag){};
	door_flag=1;
	/*turnoff the timer and the motor*/
	DcMotor_Rotate(OFF);
	TIMER0_deinit();

}
/*function to respond to a theift operation*/
void theif_function(){
	/*synchronization techniques to ensure that both the 2 controllers are working on the theift functions*/
	while(UART_recieveByte() != THEIF_ALARM ){}
	UART_sendByte(THEIF_ALARM);
	/*start the timer in the alarm mode to count 1 minute*/
	init_timer(MODE_ALARM);
	/*turn on the alarm to indicate a theift operation*/
	BUZZER_on();


}
/*timer0 call back function when password is entered wrong 3 times
 * it lower alarm_flag to zero
 * and it calculates 1 minute which happens in 1832 overflows
 * */
void alarm_callback(){
	timer_overflows++;
	if(timer_overflows== 1832){
		/*calculating 1 minute in 1832 overflows*/
		BUZZER_off();
		timer_overflows=0;
		TIMER0_deinit();
	}


}
/*timer0 call back function while stoping the motor for 3 seconds to allow the user to enter
 * through the door
 *
 * */
void timer_stopmotor(){
	timer_overflows++;
	if(timer_overflows == 92){
		/*calculating 3 seconds which happens through 92 overflows*/
		door_flag=0;
		timer_overflows=0;

	}

}
/*timer0 call back function in case of opening door to calculate exactly 15 seconds,
 * while opening the door and while closing the door
 * */
void timer0_processing(){
	timer_overflows++;

	if(timer_overflows ==  458){
		/* reached 15 seconds which happens in 458 overflows*/
		/*lower the flag so we stop the system freeze while opening or closing the door*/
		door_flag=0;
		timer_overflows=0;
	}


}
