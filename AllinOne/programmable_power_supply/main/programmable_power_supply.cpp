#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "math.h"
#include "driver/gptimer.h" //for timers
#include "lcd.h"
#include "image.h"
#include "common.h" //simple function of adc , dac
#include "interruptHandler.h" //timer and gpio interrupt

//---bitbang pins------------------------------
const int _CS    = 15;
const int _RESET = 19;
const int _SCLK  = 18;
const int _SID   = 23; // Mosi
//Backlight Default Gpio4 //PWM BRIGHTNESS
Nokia105 display( _SID,  _SCLK, _RESET, _CS);

extern "C" void app_main(void) {
//-----init-timer interrupt----------
init_Interrupt_Timer0_Sec ();
/////////////////////////////////

//----init-switch pin---------
init_Interrupt_Gpio (); 
//use me for debounce: https://github.com/khoih-prog/ESP32TimerInterrupt/blob/master/examples/SwitchDebounce/SwitchDebounce.ino
//////////////////////////////

gpio_reset_pin(GPIO_LED);
gpio_set_direction(GPIO_LED,GPIO_MODE_OUTPUT);
//gpio_set_level(GPIO_LED,1);
//---------------------DISPLAY SETUP----------------------------------------
display.initDisplay();
display.PWMinit();
display.setLcdBrightness(100);   //max 512, duty cycle
display.setDrawPosition(128,160);
display.backgroundColor(BLUE);
display.displayClear();
ESP_LOGI(TAG, "APP main - Display Setup Completed, Functions are Activated");
/////////////////////////////////////////////////////////////////////////////
//---------dac adc init-----------------------------------------------------
init_peripherals ();
init_Adc_Config ();
set_Dac_Voltage (1500); //12 bit dac
/////////////////////////////////////////////////////////////////////////////
while (1) {
float vcc = 0;
float i = 1.1253;
float w = 12.3423;

uint16_t raw = 0;
vcc = printADC_Count_Voltage (&raw);
display.printString("Volt:",1,10,GREEN,BLACK); //there are problem in 0 printing
display.printDigitFloat(vcc,4,50,10,GREEN,BLACK);
display.printString("V",112,10,GREEN,BLACK);

display.printString("RAW:",1,30,GREEN,BLACK);
display.printDigitInteger(raw,48,30,GREEN,BLACK);

display.printString("Curr:",1,50,GREEN,BLACK); //there are problem in 0 printing
display.printString("A",112,50,GREEN,BLACK);
display.printDigitFloat(i,4,50,50,GREEN,BLACK);

display.printString("Pow:",1,70,GREEN,BLACK); //there are problem in 0 printing
display.printString("W",112,70,GREEN,BLACK);
display.printDigitFloat(w,4,50,70,GREEN,BLACK);


vTaskDelay(pdMS_TO_TICKS(100)); //delay
}// end of while
}//end of main
