/****************************************************************************************
 *
 *  AUTHOR : Nader Atef Labib
 *
 * file name: control_ecu.h
 * file type: header file
 * PROJECT description: door lock security system it consists of 2 microcontrollers
 * (HMI and CONTROL UNIT) which communicate together with UART
 *
 * This is the header file for the control unit
 *
 *
 *
 *
 ***************************************************************************************/

#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

/****************************************************************************************

   	   	   	   	   	   Macro definition

 ***************************************************************************************/

/*Hand shaking bytes between the 2 controllers to synchronize UART
 * and determine which function both ECUS must execute right now
 * in order to be both in the required function at the same time
 * */


#define HAND_SHAKE 0x50
#define CHANGE_PASSWORD 0x72
#define THEIF_ALARM 0x20
#define CHECK_PASSWORD 0x82
#define CHECK_FIRST_TIME 0x12
#define OPEN_DOOR  0x30
#define FATAL_ERROR 0x51

#define LOW_ADDRESS 56
#define START_UP_ADDRESS 64
#define START_UP_CHECK 111



/*MODE_OPENING_DOOR is a macro for choosing timer0 to calculate 15 seconds*/
#define MODE_OPENING_DOOR 0

/*MODE_OPENING_DOOR is a macro for choosing timer0 to calculate 60 seconds*/
#define  MODE_ALARM 1

/*MODE_OPENING_DOOR is a macro for choosing timer0 to calculate 3 seconds*/
#define MODE_MOTOR_REST 2



/****************************************************************************************

   	   	   	   	   	  Functions Prototypes

 ***************************************************************************************/
/*call back function for stopping the motor for 3 seconds while opening the door operation
 * it calculates 3 seconds time interval
 * */
void timer_stopmotor();

/*alarm call back function it calculates a 1 minute time interval*/
void alarm_callback();

/*function to initialize timer0 mode of operations and its's corresponding callback function*/
void init_timer(uint8 );


/*function to initialize drivers used in the application
 * it takes an unsigned character as an input to determine the time needed to be calculated
 * */
void drivers_init();

/*timer0 call back function in case of opening door to calculate exactly 15 seconds,
 * while opening the door and while closing the door
 * */
void timer0_processing();


/*function to store the password in EEPROM and to compare if the 2 passwords entered matches then it
 * will send to the HMI with UART that the password is succesfully entered
  */
void Change_Password();


/*Check_password is a function used to check if the user entered the correct password or not*/
void Check_Password();


/*function used to check if it was the first time to run the software or if it has been runned before*/
uint8 check_first_time();


/*function to correspond to opening the door
 * it activates the motor for 15 seconds CW
 * it stops the motor for 3 seconds
 * then it reactivate the motor 15 seconds anticlock wise
 * then turn it off
 * */
void open_door();


/*function to respond for theft attempt it operates the buzzer for 1 minute*/
void theif_function();


/*function to recieve what the HMI wants to execute right now and run the corresponding needed function*/
uint8 recieve_function();


#endif /* CONTROL_ECU_H_ */
