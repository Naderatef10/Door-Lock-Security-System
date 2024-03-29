/************************************************************************************************************

 	 	 	 	 	 	 	 AUTHOR: Nader Atef
 	 	 	 	 	 	 	 MODULE NAME: UART
 	 	 	 	 	 	 	 MODULE TYPE : HEADER FILE
 ************************************************************************************************************/


#ifndef UART_H_
#define UART_H_

/************************************************************************************************************
  	  	  	  	  	  	  	  INCLUDED MODULES and macro definitions

 ************************************************************************************************************/

#include "std_types.h"



/*************************************************************************************************************
 	 	 	 	 	 	 	 USER DEFINED DATA TYPES

 ***********************************************************************************************************/
/*enum to define the parity bit type and if it is enabled or not in the UART frame*/

typedef enum{
	PARITY_OFF=0,PARITY_EVEN=2,PARITY_ODD=3

}UART_parity;
/*enum to determine the number of stop bits either one or two */

typedef enum{
	STOP_ONE,STOP_TWO

}UART_stopBit;

/*enum to determine the size of the word transmitted or received in the UART frame */

typedef enum{

	FIVE_BITS,SIX_BITS,SEVEN_BITS,EIGHT_BITS,NINE_BITS=7

}UART_wordSize;


/*Configuration structure to dynamically configure the initilization of the UART  frame*/

typedef struct{
	/*Integar value to  store the value of the baud rate*/
	uint32 baud_rate;
	/*enum variable to store the parity_type of the */
	UART_parity parity_type;
	UART_stopBit stop_bits_number;
	UART_wordSize frame_size;

}UART_ConfigType;

/************************************************************************************************************
  	  	  	  	  	  	  	  Functions Prototypes

 ************************************************************************************************************/

/*UART_init to initialize the UART module it takes as an argument a pointer to the configuration structure,
  to configure the
 */
void UART_init(UART_ConfigType *);

/* UART_sendByte function to send a byte of data it takes an unsigned character as function argument*/
void UART_sendByte(uint8 data);

/*UART_recieveByte function to receive a byte of data*/

uint8 UART_recieveByte();

/*UART_sendString is a function to send a string via UART it takes a  pointer to constant(to protect the string in the function) character*/

void UART_sendString(const uint8* str);

/*UART_recieveString is a function to receive a string via UART it takes pointer to character so that we can access the array of strings
 *in which we will store the strings
 */
void  UART_recieveString(uint8*str);


#endif /* UART_H_ */
