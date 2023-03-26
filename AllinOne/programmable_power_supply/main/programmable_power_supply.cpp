#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h" //error
#include "driver/gpio.h"
#include "math.h"
#include "driver/gptimer.h" //for timers
#include "lcd.h"
#include "image.h"
#include "common.h" //simple function of adc , dac
#include "interruptHandler.h" //timer and gpio interrupt
#include "wifi_handler.h" //init wifi
#include "rotaryEncoderHandler.h" //rotary encoder
#include "mosfetHandler.h" //mosfet pwm

//-------variables----------
float vcc = 0;
int Mpwm = 70;
////////////////////////////
//----------functions defines------------------

////////////////////////////////////////////////

//---bitbang pins------------------------------
const int _CS    = 15;
const int _RESET = 19;
const int _SCLK  = 18;
const int _SID   = 23; // Mosi
//Backlight Default Gpio4 //PWM BRIGHTNESS
Nokia105 display( _SID,  _SCLK, _RESET, _CS);

extern "C" void app_main(void) {

//--------init wifi------------------
initWifi();
/////////////////////////////////////

//-----init-timer interrupt----------
init_Interrupt_Timer0_Sec ();
init_Interrupt_Timer1_MSec (); //for switch handler
/////////////////////////////////

//----init-switch pin---------
init_Interrupt_Gpio (); 
//////////////////////////////

//----mosfet--pwm--init-----------
initMosfetHandler ();
setPwmMosfet (100);
////////////////////////////////////

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

//-------init Rotary Encoder-------------------------------------------------
init_Rotary_Encoder ();
////////////////////////////////////////////////////////////////////////////

while (1) {
float i = 1.1253;
float w = 12.3423;
uint16_t raw = 0;
vcc = printADC_Count_Voltage (&raw);
// vcc = vcc *10; //opamp, 0.1 gain.
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

sendtowifi(&vcc,&raw,&i);
vTaskDelay(pdMS_TO_TICKS(100)); //delay
int count_ = read_Rotary_Encoder ();
// setPwmMosfet (count_); //set pwm by rotary encoder
//----testing--maintain set voltage-----------------------
if (5 > (vcc )) {  //read voltage less than set voltage, increment in pwm
    if (4096 < Mpwm) { //constarin pwm
        Mpwm = 4095;
    }
    Mpwm = Mpwm + 1;
} else if (5 < (vcc )) { //read voltage more than set voltage, decrement in pwm
    if (0 > Mpwm) { //constarin pwm
        Mpwm = 0;
    }
    Mpwm = Mpwm - 1;
}
if ((vcc < 0)) { //starting boost
Mpwm = 600;
}
setPwmMosfet (Mpwm); //12 bit timer: 4096, 72:  | 100: | 600: 14.83%  | 1500: 36.63% | 3000: 73.2% , 4000: 97.75%, 4095: 99.98%
ESP_LOGI(TAG, "Mosfet pwm: %d ",Mpwm);  //esp_rom_delay_us(1); esp idf version 5 ets delay name change 
//---------------------------------------------------
}// end of while
}//end of main