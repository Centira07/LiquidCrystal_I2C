# LiquidCrystal with I2C
Copy both hd44780.c and hd44780.h to your project and just start using it. 
**NOTE:** this library uses custom I2C AVR implementation (cheprogrammer/i2c_avr). 
# Usage example
At the beginning of the program initialize your I2C peripheral and call the Liquid Crystal I2C initialization routine by using the following method:
#include <stdint.h>
#include "lib/hd44780.h"

int main(void)
{
  LiquidCrystalDevice device = init(0x27, 16, 2, LCD_5x8DOTS);
  turnOnBacklight(&device);
  
  setCursor(&device,0,0);
  print(&device, "Hello World");
  
  while(1);
  
  return 0;
}
