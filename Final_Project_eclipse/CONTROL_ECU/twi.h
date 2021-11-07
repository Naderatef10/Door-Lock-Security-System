/************************************************************************************************************
 	 	 	 	 	 	 Module Name: I2C header file

 	 	 	 	 	 	 Author : Nader Atef

 ************************************************************************************************************/

#ifndef TWI_H_
#define TWI_H_

/*************************************************************************************************************
  	  	  	  	  	  	  Include Libraries
 ************************************************************************************************************/
#include "std_types.h"

/************************************************************************************************************
 	 	 	 	 	 	 MACRO DEFINITIONS
 ***********************************************************************************************************/


/* I2C Status Bits in the TWSR Register */


#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */
/***************************************************************************************************************************************************
											USER DEFINED DATA TYPES

***************************************************************************************************************************************************/

/*TWI_bitRate is an enum to determine the mode of operation of the TWI*/
typedef enum{

	NORMAL_MODE=100000,FAST_MODE=400000,FAST_MODE_PLUS=1000000,HIGH_SPEED_MODE=3400000


}TWI_bitRate;
/*TWI_ConfigType is a structure to dynamically configure the TWI to a certain mode(speed)*/
typedef struct{

	TWI_bitRate F_SCL;
	uint8 address_slave;

}TWI_ConfigType;

/***************************************************************************************************************************************************

  	  	  	  	  	  	  	  	  	  	  Functions prototypes

 **************************************************************************************************************************************************/
/* function to initialize the TWI driver*/
void TWI_init(TWI_ConfigType*);

/*function to send the start bit */
void TWI_start();
/*function to send the stop bit*/
void TWI_stop();

/*function to receive a byte of data and respond with ACK*/
uint8 TWI_readByteWithACK();
/*function to receive a byte of data and doesn't respond with ACK*/
uint8 TWI_readByteWithNACK();
/*function to get the TWSR status register bits to recognize if the operations performed is successful*/
uint8 TWI_getStatus();

/*function to send a byte of data using TWI*/
void TWI_writeByte(uint8);



#endif /* TWI_H_ */
