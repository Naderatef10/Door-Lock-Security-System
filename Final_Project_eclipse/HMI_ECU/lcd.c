#include "gpio.h"
#include "lcd.h"
#include <util/delay.h>


/* LCD_sendCommand is a function to send commands to LCD*/
void LCD_sendCommand(uint8 command){
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN,LOGIC_LOW);
	GPIO_writePin(LCD_READ_WRITE_PORT_ID,LCD_READ_WRITE_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin( LCD_ENABLE_PORT_ID,LCD_ENABLE,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_BUS,command);
	_delay_ms(1);
	GPIO_writePin( LCD_ENABLE_PORT_ID,LCD_ENABLE,LOGIC_LOW);
	_delay_ms(1);
}
/*LCD_displayCharacter is a function to display character on the LCD*/
void LCD_displayCharacter(uint8 value){
	GPIO_writePin(LCD_RS_PORT_ID,LCD_RS_PIN,LOGIC_HIGH);
	GPIO_writePin(LCD_READ_WRITE_PORT_ID,LCD_READ_WRITE_PIN,LOGIC_LOW);
	_delay_ms(1);
	GPIO_writePin( LCD_ENABLE_PORT_ID,LCD_ENABLE,LOGIC_HIGH);
	_delay_ms(1);
	GPIO_writePort(LCD_DATA_BUS,value);
	_delay_ms(1);
	GPIO_writePin( LCD_ENABLE_PORT_ID,LCD_ENABLE,LOGIC_LOW);
	_delay_ms(1);

}

/*LCD_init() is a function to initialize the LCD driver*/
void LCD_init(){
	GPIO_setupPinDirection(LCD_RS_PORT_ID, LCD_RS_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_READ_WRITE_PORT_ID, LCD_READ_WRITE_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(LCD_ENABLE_PORT_ID,LCD_ENABLE,PIN_OUTPUT);
	GPIO_setupPortDirection(LCD_DATA_BUS,PORT_OUTPUT);
#if ( LCD_FUNC_MODE == LCD_FUNC_TWO_LINES_4_BITS )
	LCD_sendCommand(LCD_FUNC_TWO_LINES_4_BITS );
#elif(LCD_FUNC_MODE == LCD_FUNC_TWO_LINES_8_BITS)
		LCD_sendCommand(LCD_FUNC_TWO_LINES_8_BITS );
#endif
	LCD_sendCommand( LCD_CURSOR_OFF);
	LCD_sendCommand(LCD_CLEAR_SCREEN);
}
/*LCD_displayString is a function to display string on LCD*/
void LCD_displayString(const  uint8 * str ){
	uint8 i=0;
	while(str[i]!='\0'){
		LCD_displayCharacter(str[i]);
		i++;
	}



}
/*LCD_moveCursosr is a function to move the cursor of the LCD to a certain row and column*/
void LCD_moveCursor(uint8 row, uint8 col){
	uint8 lcd_memory_address;

	switch(row){

	case 0: lcd_memory_address= col;
		break;
	case 1: lcd_memory_address= col + 0x40;
		break;
	case 2: lcd_memory_address= col + 0x10;
		break;
	case 3: lcd_memory_address= col + 0x50;
		break;


	}

LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);


}
/*Function to display a string at a certain row and column to the LCD*/
void LCD_displayStringRowColumn(uint8 row, uint8 col, const char* str){
	LCD_moveCursor(row,col);
	LCD_displayString(str);


}
/*LCD_clearScreen function used to clear the screen of the LCD*/
void LCD_clearScreen(){

	LCD_sendCommand(LCD_CLEAR_SCREEN);

}
/*LCD_intgerToString function used to print integers on the LCD*/
void LCD_intgerToString(int data){
	char buffer[16];
	itoa(data,buffer,10);
	LCD_displayString(buffer);

}







