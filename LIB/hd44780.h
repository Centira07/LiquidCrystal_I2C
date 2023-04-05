#ifndef hd44780_h
#define hd44780_h

#include <inttypes.h>
#include <avr/io.h>
#include <Print.h>

#ifndef  F_CPU
#define F_CPU 16000000UL
#endif

//commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
//display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
//display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
//display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
//function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
//backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

#define En B00000100  // Enable bit
#define Rw B00000010  // Read/Write bit
#define Rs B00000001  // Register select bit

typedef struct LiquidCrystalDevice {
	uint8_t Address;
	uint8_t Columns;
	uint8_t Rows;
	uint8_t Backlight;
	uint8_t DisplayFunction;
	uint8_t DisplayControl;
	uint8_t DisplayMode;
} LiquidCrystalDevice;

struct LiquidCrystalDevice init(uint8_t address, uint8_t columns, uint8_t rows, uint8_t dotSize);
void clear(struct LiquidCrystalDevice* device);

void setCursor(struct LiquidCrystalDevice* device, uint8_t row, uint8_t column);

void returnHome(struct LiquidCrystalDevice* device);

void turnOnBacklight(struct LiquidCrystalDevice* device);

void turnOffBacklight(struct LiquidCrystalDevice* device);

void print(struct LiquidCrystalDevice* device, char* value);

void turnOnDisplay(struct LiquidCrystalDevice* device);

void turnOffDisplay(struct LiquidCrystalDevice* device);

void lq_turnOnCursor(struct LiquidCrystalDevice_t* device);

void turnOffCursor(struct LiquidCrystalDevice* device);

void turnOnBlink(struct LiquidCrystalDevice* device);

void turnOffBlink(struct LiquidCrystalDevice* device);

void scrollDisplayLeft(struct LiquidCrystalDevice* device);

void scrollDisplayRight(struct LiquidCrystalDevice* device);

void leftToRight(struct LiquidCrystalDevice* device);

void rightToLeft(struct LiquidCrystalDevice* device);

void turnOnAutoscroll(struct LiquidCrystalDevice* device);

void turnOffAutoscroll(struct LiquidCrystalDevice* device);

void createChar(struct LiquidCrystalDevice* device, uint8_t slot, uint8_t charmap[8]);

void sendCommand(struct LiquidCrystalDevice* device, uint8_t command);

void writeDeviceByte(struct LiquidCrystalDevice* device, uint8_t value, uint8_t mode);

void writeDevice4Bits(struct LiquidCrystalDevice* device, uint8_t value);

void writeDevicePulse(struct LiquidCrystalDevice* device, uint8_t value);

void transmitI2C(struct LiquidCrystalDevice* device, uint8_t value);

#endif
