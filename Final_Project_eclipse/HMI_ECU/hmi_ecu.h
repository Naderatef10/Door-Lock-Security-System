/****************************************************************************************
 *
 *  AUTHOR : Nader Atef Labib
 *
 * file name: hmi_ecu.h
 * file type: header file
 * PROJECT description: door lock security system it consists of 2 microcontrollers
 * (HMI and CONTROL UNIT) which communicate together with UART
 *
 * This is the header file for the HMI controller
 *
 *
 *
 *
 ***************************************************************************************/
#ifndef HMI_ECU_H_
#define HMI_ECU_H_
/****************************************************************************************

   	   	   	   	   	   Macro definition

 ***************************************************************************************/
/*MODE_OPENING_DOOR is a macro for choosing timer0 to calculate 15 seconds*/
#define MODE_OPENING_DOOR 0
/*MODE_OPENING_DOOR is a macro for choosing timer0 to calculate 60 seconds*/
#define MODE_ALARM 1
/*MODE_OPENING_DOOR is a macro for choosing timer0 to calculate 3 seconds*/
#define MODE_MOTOR_REST 2

/*Hand shaking bytes between the 2 controllers to synchronize UART*/
#define THEIF_ALARM 0x20
#define HAND_SHAKE 0x50

#define CHANGE_PASSWORD 0x72
#define CHECK_PASSWORD 0x82
#define CHECK_FIRST_TIME 0x12

#define FATAL_ERROR 0x51
#define OPEN_DOOR  0x30


/****************************************************************************************

   	   	   	   	   	  Functions Prototypes

 ***************************************************************************************/

/*function to initialize timer0 mode of operations and its's corresponding callback function*/
void init_timer(uint8 );


/*function to initialize drivers used in the application*/
void drivers_init();


/*timer0 call back function in case of opening door to calculate exactly 15 seconds,
 * while opening the door and while closing the door
 * */
void timer0_processing();


/*function to change the password of the system or to create the password on startup*/
void Change_Password();



/*function to check if the password entered by the user is correct or not
 *it returns 1 if the control unit succesfully compared the 2 passwords
 *it returns FATAL_ERROR macro which indicates entering the password wrong for 3 times
 * */

uint8 Check_Password();


/*the main menu of the door lock security system in which we have 2 options
 * +: to open the door
 * -: to change the password
 * in both cases it is required to enter the password as a check first
 * if the password is entered wrong 3 times alarm will be triggered
 * */
void main_menu();

/*function to respond to entering the password correctly in addition to pressing on the + key*/

void open_door();

/*timer0 call back function when password is entered wrong 3 times
 * it lower alarm_flag to zero
 * and it calculates 1 minute which happens in 1832 overflows
 */
void alarm_callback();

/*routine of operations when the user enters the password wrong 3 times*/

void theif_function();

/*timer0 call back function while stoping the motor for 3 seconds to allow the user to enter
 * through the door
 *
 * */
void timer_stopmotor();



#endif /* HMI_ECU_H_ */
