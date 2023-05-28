#include "MCP4922.h"

//----Local variables--------------
int _SDI	=	0;
int _SCK 	=	0;
int _CS  	=	0;
int _LDAC	=	0;
/// @brief ///////////////////////
/// @param SDI data in
/// @param SCK  clock
/// @param CS chip select
/// @param LDAC /Latch of dac

//------------dac init fuction-------------------------
void mcp4922_init(uint8_t SDI, uint8_t SCK, uint8_t CS) {
_SDI = SDI;
_SCK = SCK;
_CS  = CS;
gpio_reset_pin((gpio_num_t)_SDI);
gpio_reset_pin((gpio_num_t)_SCK);
gpio_reset_pin((gpio_num_t)_CS);
gpio_set_direction((gpio_num_t)_SDI,GPIO_MODE_OUTPUT);
gpio_set_direction((gpio_num_t)_SCK,GPIO_MODE_OUTPUT);
gpio_set_direction((gpio_num_t)_CS,GPIO_MODE_OUTPUT);
}
///////////////////////////////////////////////////////

//--------Set Channel A Voltage----------------------
void setDacVoltageMcp4922_CHA(uint16_t bit,uint8_t gain,uint8_t buffered,uint8_t shutDown ) {
uint16_t data = 0x0000; //data buffer append according to given arguements
uint16_t _val = bit; //input value should be in 12 bit range

if (_val > 4095) { //constrain not more than 4095
_val = 4095;
}

//data |= 0x8000; //0b1000 // channel set to be 1 or A

if (buffered) {
	data |= 0x4000; //0b100 ,_buffered: 0 is UnBuffered,X: 1 is Buffered
}    

if (gain == 1) { //set gain,0 :2X, 1: 1X 
	data |= 0x2000; //0b10
}  

if (shutDown == 1) { //shutdown output disable/enable 
	data |= 0x1000; //0b1
}

// if (_val == bit) { //if input bit is more than 4095 12 bit dac then append max
// 	data |= _val;
// } 
data |= (_val << 4);

mcp4922_WriteSPI(data);
}
////////////////////////////////////////////////////
//--------Set Channel B Voltage----------------------
void setDacVoltageMcp4922_CHB(uint16_t bit,uint8_t gain,uint8_t buffered,uint8_t shutDown ) {
//uint16_t channelA = A | 0b0111000000000000;
//uint16_t channelB = B | 0b1111000000000000;
uint16_t data = 0x1000; //data buffer append according to given arguements
uint16_t _val = bit; //input value should be in 12 bit range

if (_val > 4095) { //constrain not more than 4095
_val = 4095;
}

data |= 0x8000; //0b1000 // channel set to be 1 or A

if (buffered) {
	data |= 0x4000; //0b100 ,_buffered: 0 is UnBuffered,X: 1 is Buffered
}    

if (gain == 1) { //set gain,0 :2X, 1: 1X 
	data |= 0x2000; //0b10
}  

if (shutDown == 1) { //shutdown output disable: 0, enable: 1 
	data |= 0x1000; //0b1
}

// if (_val == bit) { //if input bit is more than 4095 12 bit dac then append max
// 	data |= _val;
// } 
data |= (_val << 4);
mcp4922_WriteSPI(data);
}
/////////////////////////////////////////////////////

//----------spi write function-----------------------------
void mcp4922_WriteSPI(uint16_t _data) {
gpio_set_level((gpio_num_t)_CS, 0);

//-----------------MSBFIRST------------------
//------------higher byte of data-------------
for (uint8_t mask = 0x80; mask; mask >>= 1) {
	gpio_set_level((gpio_num_t)_SDI, (_data >> 8 & mask));
	gpio_set_level((gpio_num_t)_SCK, 1);
	gpio_set_level((gpio_num_t)_SCK, 0);
}
// gpio_set_level((gpio_num_t)_CS, 1);
// gpio_set_level((gpio_num_t)_CS, 0);
//------------lower byte of data----------
for (uint8_t mask = 0x80; mask; mask >>= 1) {
	gpio_set_level((gpio_num_t)_SDI, (_data & 0xFF & mask));
	gpio_set_level((gpio_num_t)_SCK, 1);
	gpio_set_level((gpio_num_t)_SCK, 0);
}

gpio_set_level((gpio_num_t)_CS, 1);
vTaskDelay(1);
} 
//************************************************************************ 
/*
Bitmasking for setting options in dac:

The four MSB in the Mask 0b0111000000000000 and 0b1111000000000000 is for
setting different options of the DAC setup.

0bX111000000000000 where X is What DAC channel the SPI is writing to.
bit15			   X=0 is writing to channel A.
		X=1 is writing to channel B.

0b0X11000000000000 where X is Buffered or UnBuffered mode. Buffered uses LDAC 
bit14			   pin to simuttaneous update both channels.
		UnBuffered I guess is writing outputs directly to DAC 
		outputs and ignoring LDAC pin.
		X=0 is UnBuffered.
		X=1 is Buffered.
		
0b01x1000000000000 where X is GAIN selector.  
bit13			   X=0 is 2X GAIN.
		X=1 is 1X GAIN.
		
0b011X000000000000 where X SHUTDOWN.
bit12			   X=0 OUTPUT is DISABLED on selected channel.
		X=1 OUTPUT is ENABLED on selected channel.

0b0111XXXXXXXXXXXX where X is the 12 bits to be written to the active channel.
bit 11 down to bit 0			

*/
//************************************************************************
