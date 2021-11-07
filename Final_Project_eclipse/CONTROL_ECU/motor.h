#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"


#define MOTOR_PORT PORTB_ID
#define INPUT1_PIN PIN0_ID
#define INPUT2_PIN PIN1_ID
#define ENABLE_PIN  PIN3_ID

/*enum to determine the state of the motor either OFF or rotating clock wise and anti clock wise*/
typedef enum{

	OFF,CW,ACW

}DcMotor_State;




/*function to initialize DC motor driver*/
void DcMotor_Init(void);

/*function to operate the DC motor*/

void DcMotor_Rotate (DcMotor_State);



#endif /* MOTOR_H_ */
