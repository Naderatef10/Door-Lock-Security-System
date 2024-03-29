/**************************************************************************************
 * AUTHOR : Nader Atef Labib
 *
 * file name: hmi_ecu.c
 * file type: source code
 * PROJECT description: door lock security system it consists of 2 microcontrollers
 * (HMI and CONTROL UNIT) which communicate together with UART
 *
 * this is the source code file for the HMI microcontroller

 * the HMI interface main specifications is only taking inputs from the user,
 * any computations or control for the system is executed in the control unit.
 *
 *
 * Kindly check the header file hmi_ecu.h because macrodefinitions and functions,
 * are include in that file.
 **************************************************************************************/


/***************************************************************************************
 *
 * 					Included modules in the project
 *
 *
 **************************************************************************************/
#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include "timer0.h"
#include <util/delay.h>
#include <avr/io.h>
#include "hmi_ecu.h"


/****************************************************************************************
 *
 * 							Global variables
 *
 * *************************************************************************************/

/*alarm_flag is a global it is used in the call back function
  of timer in case a user entered the password 3 times wrong it will be used as a flag
  in this call back function
 * */
volatile uint8 alarm_flag=1;
/*an array to store the password of the door*/
uint8 Door_password[5];
/*timer_overflows is a global variable to count the number of overflows of timer0
 in order to calculate certain intervals of times.
 **/
volatile uint32 timer_overflows=0;
/*startup_check is a variable used to recieve (using uart) the startup value of the code from the control unit
  if it receives 0 it means that it is not the first time the system is starting.
  if it receives 1 it means that it is the first time the system is running.

 */
uint8 startup_check=1;
/*door_flag is a flag used in the call back function of timer0 when open the door to freeze the system
 *till the door opens.
 * */
volatile uint8 door_flag=1;


/********************************************************************************************************
 *
 * 								Main application code
 *
 *
 * *****************************************************************************************************/
int main(void)

{   /*initialzing the drivers used in the application layer*/
	drivers_init();
	/*sending a hand shake to the control unit to recieve the startup condition*/
	UART_sendByte(HAND_SHAKE);
	startup_check = UART_recieveByte();
	/*checking if it is the first time to run the system or not*/
	if(startup_check==1){
		Change_Password();
	}
	while(UART_recieveByte() != HAND_SHAKE){}
	while(1)
	{    /*calling the main menu in which the 2 main options appear in*/
		main_menu();
	}

}

/*function to initialize drivers used in the application*/
void drivers_init(){
	/*activating global interrupt enable*/
	SREG |= (1<<7);
	/*dynamically configuring UART to settings
	 * baud rate: 9600
	 * Parity : disabled
	 * number of stop bits: one
	 * word size: eight bits
	 * */
	UART_ConfigType u1 = {9600,PARITY_OFF,STOP_ONE,EIGHT_BITS};
	UART_init(&u1);
	/*initalizing LCD*/
	LCD_init();

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
/*function to change the password of the system or to create the password on startup*/
void Change_Password(){

	uint8 i=0;
	uint8 key_pressed;
	uint8 control_check_match=0;

	/*handshaking protocols to make sure that the control unit is in the change password function*/
	UART_sendByte(CHANGE_PASSWORD);
	while(UART_recieveByte() != CHANGE_PASSWORD ){};
	/*do while loop to keep taking inputs from user till the user enters the password
	 * correctly twice*/
	do{
		LCD_clearScreen();
		control_check_match=1;
		LCD_displayString("Enter password: ");
		LCD_moveCursor(1,0);

		for(i=0;i<5;i++){
			/*getting pressed key from user*/
			key_pressed = KEYPAD_getPressedKey();
			Door_password[i]=key_pressed;
			LCD_displayCharacter('*');
			/*handshaking protocol to make sure the control unit is ready to recieve*/
			while(UART_recieveByte() != HAND_SHAKE){}
			UART_sendByte(Door_password[i]);
			_delay_ms(500);
		}
		/*keep freezing the  keypad till the user presses the enter key*/
		while(1){
			key_pressed = KEYPAD_getPressedKey();
			if(key_pressed==13){
				break;
			}
			else{
				continue;
			}
			_delay_ms(500);
		}
		_delay_ms(500);

		LCD_clearScreen();
		LCD_displayString("Reenter password:");

		LCD_moveCursor(1,0);
		/*second loop to make the user reenter the password*/
		for(i=0;i<5;i++){

			key_pressed = KEYPAD_getPressedKey();
			Door_password[i]=key_pressed;
			LCD_displayCharacter('*');
			/*handshaking protocol to make sure the control unit is ready to recieve*/
			while(UART_recieveByte() != HAND_SHAKE){}
			UART_sendByte(Door_password[i]);
			_delay_ms(500);

		}
		/*keep freezing the  keypad till the user presses the enter key*/
		while(1){
			key_pressed = KEYPAD_getPressedKey();
			if(key_pressed==13){
				break;
			}
			else{
				continue;
			}
			/*delay between each press on the keypad*/
			_delay_ms(500);
		}
		_delay_ms(500);

		UART_sendByte(HAND_SHAKE);
		/*recieving the result of comparing the 2 passwords from the control unit*/
		control_check_match=UART_recieveByte();

	}while(!control_check_match);

	return;
}
/*the main menu of the door lock security system in which we have 2 options
 * +: to open the door
 * -: to change the password
 * in both cases it is required to enter the password as a check first
 * if the password is entered wrong 3 times alarm will be triggered
 * */
void main_menu(){

	uint8 response = 0;
	uint8 key;
	while(1){
		/*printing some GUI on the LCD*/
		LCD_clearScreen();
		LCD_displayString("+: open door");
		LCD_moveCursor(1,0);
		LCD_displayString("-: change pass");
		key= KEYPAD_getPressedKey();
		_delay_ms(500);

          /*if the key pressed is -*/
		if(key == '-'){

			UART_sendByte(CHECK_PASSWORD);
			response= Check_Password();
			if( response == 1){
				/*password entered correctly call change password*/
				UART_sendByte(CHANGE_PASSWORD);
				Change_Password();

			}
			if(response == FATAL_ERROR){
				/*password was wrong 3 times call the alarm function*/
				UART_sendByte(THEIF_ALARM);
				theif_function();

			}
		}
		/*if the key pressed is +*/
		else if(key =='+'){

			UART_sendByte(CHECK_PASSWORD);
			response= Check_Password();
			if  ( response == 1){
				/*password was entered correctly open the door*/
				/*handshaking to ensure UART is working properly*/
				UART_sendByte(HAND_SHAKE);
				UART_sendByte(OPEN_DOOR);
				open_door();

			}
			else if(response == FATAL_ERROR){
				/*password was wrong 3 times call the alarm function*/
				UART_sendByte(THEIF_ALARM);
				theif_function();


			}
		}

	}


}
/*function to check if the password entered by the user is correct or not
 *it returns 1 if the control unit succesfully compared the 2 passwords
 *it returns FATAL_ERROR macro which indicates entering the password wrong for 3 times
 * */
uint8 Check_Password(){
	uint8 i=0;
	uint8 key_pressed;
	uint8 control_check_match=0;
	LCD_clearScreen();

	/*handshaking to ensure that the control unit is in the check_password function*/
	UART_sendByte(CHECK_PASSWORD);
	while(UART_recieveByte() != CHECK_PASSWORD ){};


	do{
		LCD_clearScreen();
		control_check_match=1;
		LCD_displayString("Enter pass: ");
		LCD_moveCursor(1,0);
		/*for loop to take input password from the user*/
		for(i=0;i<5;i++){
			key_pressed = KEYPAD_getPressedKey();
			Door_password[i]=key_pressed;
			LCD_displayCharacter('*');
			/*waiting for the control unit to be ready to take the password*/
			while(UART_recieveByte() != HAND_SHAKE){}
			/*sending the password byte by byte*/
			UART_sendByte(Door_password[i]);
			_delay_ms(500);
		}
		/*keep taking input from keypad till the user pressess the enter key*/
		while(1){
			key_pressed = KEYPAD_getPressedKey();
			if(key_pressed==13){
				/*if the user pressed enter key break from the while(1)*/
				break;
			}
			else{
				continue;
			}
			_delay_ms(500);
		}
		_delay_ms(500);


		UART_sendByte(HAND_SHAKE);
		control_check_match=UART_recieveByte();
		/*recieving the compare of the password check result*/




	}while(!control_check_match);
	/*if the hmi recieved a FATAL_ERROR return FATAL_ERROR to indicate a thieft attempt
	 * else return 1
	 * */
	if(control_check_match != FATAL_ERROR){
		return 1;
	}
	else {
		/*return FATAL_ERROR if password is entered wrong 3 times*/
		return FATAL_ERROR;
	}


}

/*routine of operations when the user enters the password wrong 3 times*/
void theif_function(){

	/*handshaking to make sure control unit is in the right function*/
	UART_sendByte(THEIF_ALARM);
	while(UART_recieveByte() != THEIF_ALARM ){}

	LCD_clearScreen();
	LCD_displayString("theif !!");
	/*initialize timer in the alarm mode which computes a 1 minute*/
	init_timer(MODE_ALARM);
	/*poll on this flag till the callback function makes the alarm_flag=0*/
	while(alarm_flag){}
	alarm_flag=1;
	LCD_clearScreen();
	/*stop timer to save power*/
	TIMER0_deinit();

}
/*function to respond to entering the password correctly in addition to pressing on the + key*/
void open_door(){
	/*handshaking till the the control units enter the open door function*/
	UART_sendByte(OPEN_DOOR);
	while(UART_recieveByte() != OPEN_DOOR ){};


	/*initializing timer to count 15 seconds*/
	init_timer(MODE_OPENING_DOOR);
	LCD_clearScreen();
	LCD_displayString("door opening");
	/*polling till the timer finishes 15s second then ISR will make door_flag=0*/
	while(door_flag){};
	door_flag=1;
	/*Initializing timer to count 3 seconds in which the door will be kept open*/
	init_timer(MODE_MOTOR_REST);
	LCD_clearScreen();
	LCD_displayString("door is now open");

	/*polling till the timer finishes 3 seconds then ISR will make door_flag=0*/
	while(door_flag){};
	door_flag=1;
	/*initializing timer to count 15 seconds*/
	LCD_clearScreen();
	init_timer(MODE_OPENING_DOOR);
	LCD_displayString("door is closing");
	/*polling till the timer finishes 15s second then ISR will make door_flag=0*/
	while(door_flag){};
	door_flag=1;

	LCD_clearScreen();
	/*stopping the timer*/
	TIMER0_deinit();

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
/*timer0 call back function when password is entered wrong 3 times
 * it lower alarm_flag to zero
 * and it calculates 1 minute which happens in 1832 overflows
 * */
void alarm_callback(){
	timer_overflows++;
	if(timer_overflows== 1832){
		/*calculating 1 minute in 1832 overflows*/
		alarm_flag=0;
		timer_overflows=0;
	}


}
