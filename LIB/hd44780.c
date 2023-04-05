#include "hd44780.h"
#include "i2c_master.h"
#include <avr/io.h>
#include <util/delay.h>

LiquidCrystalDevice init(uint8_t address, uint8_t columns, uint8_t rows, uint8_t dotSize)
{
	LiquidCrystalDevice device;

	device.Address = address;
	device.Columns = columns;
	device.Rows = rows;
	device.Backlight = LCD_NOBACKLIGHT;
	device.DisplayFunction = LCD_4BITMODE | LCD_1LINE | dotSize;
	device.DisplayControl = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;

	// Initialize to default text direction (for roman languages)
	device.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	if (rows > 1) {
		device.DisplayFunction |= LCD_2LINE;
	}

	// for some 1 line displays you can select a 10 pixel high font
	if ((dotSize != 0) && (rows == 1)) {
		device.DisplayFunction |= LCD_5x10DOTS;
	}

	// SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
	// according to datasheet, we need at least 40ms after power rises above 2.7V
	// before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
	_delay_ms(100);
	
	// Now we pull both RS and R/W low to begin commands
	transmitI2C(&device, LCD_NOBACKLIGHT);	// reset expanderand turn backlight off (Bit 8 =1)
	_delay_ms(1000);

	//put the LCD into 4 bit mode
	// this is according to the hitachi HD44780 datasheet
	// figure 24, pg 46
	
	// we start in 8bit mode, try to set 4 bit mode
	transmitI2C(&device, 0x03 << 4);
	_delay_us(4500); // wait min 4.1ms
	
	// second try
	writeDevice4Bits(&device, 0x03 << 4);
	_delay_us(4500); // wait min 4.1ms
	
	// third go!
	writeDevice4Bits(&device, 0x03 << 4);
	_delay_us(150); // wait min 150 mics
	
	// finally, set to 4-bit interface
	writeDevice4Bits(&device, 0x02 << 4);

	// set # lines, font size, etc.
	sendCommand(&device, LCD_FUNCTIONSET | device.DisplayFunction);
	
	// turn the display on with no cursor or blinking default
	turnOnDisplay(&device);
	
	// clear it off
	clear(&device);
	
	// set the entry mode
	sendCommand(&device, LCD_ENTRYMODESET | device.DisplayMode);
	
	returnHome(&device);

	return device;
};

void print(struct LiquidCrystalDevice* device, char * value)
{
	char letter = *value;

	while(letter != 0x00)
	{
		writeDeviceByte(device, letter, LCD_REGISTER_SELECT_BIT);
		letter = *(++value);
	}
};

void turnOnBacklight(struct LiquidCrystalDevice* device)
{
	device->Backlight = LCD_BACKLIGHT;
	transmitI2C(device, 0);
}

void turnOffBacklight(struct LiquidCrystalDevice* device)
{
	device->Backlight = LCD_NOBACKLIGHT;
	transmitI2C(device, 0);
}

void clear(LiquidCrystalDevice* device)
{
	sendCommand(device, LCD_CLEARDISPLAY); // clear display, set cursor position to zero
	_delay_us(2000);  // this command takes a long time!

	setCursor(device, 0, 0);
}

void setCursor(LiquidCrystalDevice* device, uint8_t row, uint8_t column)
{
	uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

	sendCommand(device, LCD_SETDDRAMADDR | (column + row_offsets[row]));
}

void returnHome(LiquidCrystalDevice* device)
{
	sendCommand(device, LCD_RETURNHOME);  // set cursor position to zero
	_delay_us(2000);  // this command takes a long time!
};

void turnOnDisplay(LiquidCrystalDevice* device)
{
	device->DisplayControl |= LCD_DISPLAYON;
	sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
};

void turnOffDisplay(LiquidCrystalDevice* device)
{
	device->DisplayControl &= ~LCD_DISPLAYON;
	sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
};

void turnOnCursor(struct LiquidCrystalDevice* device)
{
	device->DisplayControl |= LCD_CURSORON;
	sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
}

void turnOffCursor(struct LiquidCrystalDevice* device)
{
	device->DisplayControl &= ~LCD_CURSORON;
	sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
}

void turnOnBlink(struct LiquidCrystalDevice* device)
{
	device->DisplayControl |= LCD_BLINKON;
	sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
}

void turnOffBlink(struct LiquidCrystalDevice* device)
{
	device->DisplayControl &= ~LCD_BLINKON;
	sendCommand(device, LCD_DISPLAYCONTROL | device->DisplayControl);
}

void scrollDisplayLeft(struct LiquidCrystalDevice* device)
{
	sendCommand(device, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight(struct LiquidCrystalDevice* device)
{
	sendCommand(device, LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void leftToRight(struct LiquidCrystalDevice* device)
{
	device->DisplayMode |= LCD_ENTRYLEFT;
	sendCommand(device, LCD_ENTRYMODESET | device->DisplayMode);
}

void rightToLeft(struct LiquidCrystalDevice* device)
{
	device->DisplayMode &= ~LCD_ENTRYLEFT;
	sendCommand(device, LCD_ENTRYMODESET | device->DisplayMode);
}

void turnOnAutoscroll(struct LiquidCrystalDevice* device)
{
	device->DisplayMode |= LCD_ENTRYSHIFTINCREMENT;
	sendCommand(device, LCD_ENTRYMODESET | device->DisplayMode);
}

void turnOffAutoscroll(struct LiquidCrystalDevice* device)
{
	device->DisplayMode &= ~LCD_ENTRYSHIFTINCREMENT;
	sendCommand(device, LCD_ENTRYMODESET | device->DisplayMode);
}

void createChar(struct LiquidCrystalDevice* device, uint8_t slot, uint8_t charmap[8])
{
	uint8_t i = 0;
	slot &= 0x7; // we only have 8 locations 0-7
	sendCommand(device, LCD_SETCGRAMADDR | (slot << 3));

	for (i = 0; i < 8; i++) 
	{
		writeDeviceByte(device, charmap[i], LCD_REGISTER_SELECT_BIT);
	}
}

void sendCommand(LiquidCrystalDevice* device, uint8_t command)
{
	writeDeviceByte(device, command, 0);
}

void writeDeviceByte(LiquidCrystalDevice* device, uint8_t value, uint8_t mode)
{
	uint8_t highnib= value & 0xf0;
	uint8_t lownib= (value<<4) & 0xf0;

	writeDevice4Bits(device, highnib | mode);
	writeDevice4Bits(device, lownib | mode);
};

void writeDevice4Bits(LiquidCrystalDevice* device, uint8_t value)
{
	transmitI2C(device, value);
	writeDevicePulse(device, value);
};

void writeDevicePulse(LiquidCrystalDevice_t* device, uint8_t value)
{
	transmitI2C(device, value | LCD_ENABLE_BIT);
	_delay_us(2);

	transmitI2C(device, value & ~LCD_ENABLE_BIT);
	_delay_us(50);
};

void transmitI2C(LiquidCrystalDevice_t* device, uint8_t value)
{
	i2c_master_sendByte(device->Address, value | device->Backlight);
};
