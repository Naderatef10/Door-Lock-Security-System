/************************************************************************************************************

 	 	 	 	 	 	 	 AUTHOR: Nader Atef
 	 	 	 	 	 	 	 MODULE NAME: UART
 	 	 	 	 	 	 	 MODULE TYPE : C FILE

 ************************************************************************************************************/

#include "uart.h"
#include <avr/io.h>
#include "common_macros.h"

void UART_init(UART_ConfigType * configptr){
	uint16 data;

	UCSRA = (1<<U2X);
	/*enabling receiver enable and transmitter enable in UART*/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	/*note while writing in UCSRC URSEL must be equal to 1*/

	/*masking other bits in UCSRC except  UPM1 AND UPM0 to configure the parity configuration*/

	UCSRC = (UCSRC & 0xCF) | ((configptr->parity_type)<<4) | (1<<URSEL);

	/*Conserving all bits of UCSRC except USBS to configure the number of stop bits*/
	UCSRC = (UCSRC & 0xF7) | ((configptr->stop_bits_number)<<3) | (1<<URSEL);


	/*this condition is needed because if we choose the UART in Nine bits mode,
	 * there will be a problem because we need to set UCSZ2 which is in another,
	 * register, so in the case of NINE bits mode will execute the else block
	 * */
	if(configptr->frame_size!= NINE_BITS){
		/*masking all bits in UCSRC except UCSZ1 and UCSZ0 to choose the word size
		 either 5 or 6 or 7 or 8 or 9 bits per frame
		 */
		UCSRC = (1<<URSEL) | (UCSRC & 0xF9) | (((configptr->frame_size)&0x03)<<1);
	}
	else{

		/*will only be executed in case the nine bits mode will be choosen*/

		UCSRC = (1<<URSEL) | (UCSRC & 0xF9) | ((configptr->frame_size)<<1);
		UCSRB = (1<<UCSZ2);
	}



	/* the following 3 lines of code is the equation to determine the value in the UBBR register
   depending on the baud rate
	 */

	data=(uint16)(((F_CPU / ((configptr->baud_rate) * 8UL))) - 1);

	UBRRL=data;
	UBRRH= (data>>8);

}
/*void UART_sendByte(uint8 data){

	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC));
another solution using the TXC flag but the uncommented solution is the best
}
 */
void UART_sendByte(uint8 data){

	while(BIT_IS_CLEAR(UCSRA,UDRE));
	UDR = data;


}
uint8 UART_recieveByte(){

	while(BIT_IS_CLEAR(UCSRA,RXC));

	return UDR;

	/*returning UDR will clear the RXC flag and read the data at the same time*/

}
void UART_sendString(const uint8* str){
	uint8 i=0;
	while(str[i]!='\0'){

		UART_sendByte(str[i]);
		i++;

	}

}
void UART_recieveString(uint8*str){
	uint8 i=0;
	str[i] = UART_recieveByte(); /*receive first byte*/

	while( str[i] != '#'){

		i++;
		str[i] = UART_recieveByte(); /*keep reading the rest of the string till we read # also*/

	}

	str[i]= '\0'; /*replace the # sign with the null terminator so we can process the variable in c*/



}

