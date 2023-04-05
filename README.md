# LiquidCrystal with I2C
Copy both hd44780.c and hd44780.h to your project and just start using it. 
**NOTE:** this library uses custom I2C AVR implementation (cheprogrammer/i2c_avr). 
# Usage example
At the beginning of the program initialize your I2C peripheral and call the Liquid Crystal I2C initialization routine by using the following method:

init(0x27, 16, 2, LCD_5x8DOTS);

This function accepts I2C shield address for LCD display, number of columns, rows and dot size (defined LCD_5x8DOTS, LCD_5x10DOTS). After performing all initialization routine, this function returns instance of LiquidCrystalDevice structure. This structure should be stored somewhere in programm because it contains required information for evaluating future operations.

The definition of this structure is: 

typedef struct LiquidCrystalDevice { 
	uint8_t Address;
	uint8_t Columns;
	uint8_t Rows;
	uint8_t Backlight;
	uint8_t DisplayFunction;
	uint8_t DisplayControl;
	uint8_t DisplayMode;
} LiquidCrystalDevice;

Example of this initialization and other coding can be found in the examples photos with videos in the wiki.
