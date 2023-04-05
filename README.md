# LiquidCrystal with I2C
Copy both hd44780.c and hd44780.h to your project and just start using it. 
**NOTE:** this library uses custom I2C AVR implementation (cheprogrammer/i2c_avr). 
# Usage example
At the beginning of the program initialize your I2C peripheral and call the Liquid Crystal I2C initialization routine by using the following method:
<img width="164" alt="image" src="https://user-images.githubusercontent.com/123511269/230233732-11480b6c-85f2-43e4-acc1-a0049cee45f1.png">

