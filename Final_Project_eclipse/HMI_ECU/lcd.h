

#ifndef LCD_H_
#define LCD_H_

/* included libraries   */
#include "std_types.h"

/* funct LCD_FUNC_MODE ion declarations          */

/*function to send commands to the LCD*/
void LCD_sendCommand(uint8 Command);

/*function to display the required character on the screen, the value it takes is the ascii of characters*/
 void LCD_displayCharacter(uint8 value);

 /*LCD_init function is the initilization for the LCD driver*/
 void LCD_init(void);

 /*function to display a complete string*/
 void LCD_displayString(const  uint8 * );

 /*function to clear the screen of LCD*/
 void LCD_clearScreen();

 /*function to move the cursor of the LCD*/
 void LCD_moveCursor(uint8 row, uint8 col);

/*function to display a string at certain row, column in the lcd*/
 void LCD_displayStringRowColumn(uint8 row, uint8 col, const char* str);

 /*function to display integers on the screen*/
 void LCD_intgerToString(int data);

 /*definitions */


#define LCD_RS_PIN PIN7_ID         /* Macro definition the pin number in which RS  is connected to*/
#define LCD_RS_PORT_ID PORTC_ID     /* Macro definition the port number in which RS  is connected to*/


#define LCD_READ_WRITE_PIN PIN6_ID   /* Macro definition the pin number in which RW' is connected to */
#define LCD_READ_WRITE_PORT_ID PORTC_ID  /* Macro definition the port number in which RW' is connected to */

#define LCD_ENABLE PIN5_ID             /*Macro definition the pin number in which Enable is connected to*/
#define LCD_ENABLE_PORT_ID PORTC_ID   /*Macro definition the port number in which Enable is connected to*/

#define LCD_DATA_BUS PORTA_ID           /*Macro definition which port the Data bus is connected to*/

#define LCD_FUNC_MODE 0x38             /* Macro to define function mode of lcd*/

#define LCD_CURSOR_OFF 0x0C
#define LCD_CLEAR_SCREEN 0x01

#define LCD_SET_CURSOR_LOCATION 0x80

#define LCD_FUNC_TWO_LINES_8_BITS 0x38
#define LCD_FUNC_TWO_LINES_4_BITS 0x28

#endif /* LCD_H_ */
