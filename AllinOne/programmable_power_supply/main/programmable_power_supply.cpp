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
#define STACK_SIZE_0 4096 //1024 //2048 // // 4096
StaticTask_t xTaskBuffer_0; // Structure that will hold the TCB of the task being created.
StackType_t xStack_0[ STACK_SIZE_0 ]; /* Buffer that the task being created will use as its stack.  Note this is
                                    an array of StackType_t variables.  The size of StackType_t is dependent on
                                    the RTOS port.
                                */
#define STACK_SIZE_1 4096 //1024 //2048 // 4096
StaticTask_t xTaskBuffer_1; // Structure that will hold the TCB of the task being created.
StackType_t xStack_1[ STACK_SIZE_1 ]; /* Buffer that the task being created will use as its stack.  Note this is
                                    an array of StackType_t variables.  The size of StackType_t is dependent on
                                    the RTOS port.
                                */
float vcc       = 0;
int Mpwm        = 3500;
float i         = 1.1253;
float w         = 12.3423;
uint16_t raw    = 0;
int count_      = 0;
//////////////////////////////////////

//----------functions defines------------------
void vTaskCore_0 ( void * pvParameters );
void vTaskCore_1 ( void * pvParameters );

void adc_thread_0(void *pvParameters);
void rotary_encoder_thread_0(void *pvParameters);
////////////////////////////////////////////////

//---bitbang pins------------------------------
const int _CS    = 15;
const int _RESET = 19;
const int _SCLK  = 18;
const int _SID   = 23; // Mosi
//Backlight Default Gpio4 //PWM BRIGHTNESS
Nokia105 display( _SID,  _SCLK, _RESET, _CS);

extern "C" void app_main(void) {
//--------task core created-----------------------------------------------
xTaskCreateStaticPinnedToCore(
                   vTaskCore_0,       // Function that implements the task.
                   "vTaskCore_0",          // Text name for the task.
                   STACK_SIZE_0,      // Stack size in bytes, not words.
                   ( void * ) 1,    // Parameter passed into the task.
                   tskIDLE_PRIORITY,// Priority at which the task is created.
                   xStack_0,          // Array to use as the task's stack.
                   &xTaskBuffer_0,    // Variable to hold the task's data structure.
                   0 );             // Specify the task's core affinity //core0

xTaskCreateStaticPinnedToCore(
                   vTaskCore_1,       // Function that implements the task.
                   "vTaskCore_1",          // Text name for the task.
                   STACK_SIZE_1,      // Stack size in bytes, not words.
                   ( void * ) 1,    // Parameter passed into the task.
                   tskIDLE_PRIORITY,// Priority at which the task is created.
                   xStack_1,          // Array to use as the task's stack.
                   &xTaskBuffer_1,    // Variable to hold the task's data structure.
                   1 );             // Specify the task's core affinity //core1
//////////////////////////////////////////////////////////////////////////////

//-----init-timer interrupt----------
init_Interrupt_Timer0_Sec ();
init_Interrupt_Timer1_MSec (); //for switch handler
/////////////////////////////////

//----init-switch pin---------
init_Interrupt_Gpio (); 
//////////////////////////////

//----mosfet--pwm--init-----------
initMosfetHandler ();
// setPwmMosfet (100);
////////////////////////////////////

gpio_reset_pin(GPIO_LED);
gpio_set_direction(GPIO_LED,GPIO_MODE_OUTPUT);
//gpio_set_level(GPIO_LED,1);
while (1) {
// vTaskDelay(pdMS_TO_TICKS(100)); //delay
vTaskDelay(1);
//---------------------------------------------------
}// end of while
}//end of main


//------------CORE_0 task---------------------------------------------------------
//---------------------Function that implements the task being create-------------
void vTaskCore_0 ( void * pvParameters ) {
// The parameter value is expected to be 1 as 1 is passed in the
// pvParameters value in the call to xTaskCreateStaticPinnedToCore().
configASSERT( ( uint32_t ) pvParameters == 1UL );
ESP_LOGI(pcTaskGetName(NULL), "CORE_0");

//---------------------DISPLAY SETUP----------------------------------------
display.initDisplay();
display.PWMinit();
display.setLcdBrightness(100);   //max 512, duty cycle
display.setDrawPosition(128,160);
display.backgroundColor(BLUE);
display.displayClear();
ESP_LOGI(TAG, "CORE_0 - Display Setup Completed, Functions are Activated");
/////////////////////////////////////////////////////////////////////////////

//---------dac adc init-----------------------------------------------------
init_peripherals ();
init_Adc_Config ();
set_Dac_Voltage (1500); //12 bit dac
/////////////////////////////////////////////////////////////////////////////

//-------init Rotary Encoder-------------------------------------------------
init_Rotary_Encoder ();
////////////////////////////////////////////////////////////////////////////

//------------------------THREAD_0------------------------------------
xTaskCreate(&adc_thread_0, "adc_thread_0", 1024*4, NULL, tskIDLE_PRIORITY, NULL); //check priority make macro of it
xTaskCreate(&rotary_encoder_thread_0, "rotary_encoder_thread_0", 1024*4, NULL, 5, NULL);
///////////////////////////////////////////////////////////////////////

while (1) {
vTaskDelay(1);
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

display.printString("Rot:",1,90,GREEN,BLACK); //there are problem in 0 printing
display.printString("C",112,90,GREEN,BLACK);
display.printDigitInteger(count_,50,90,GREEN,BLACK);

} //end of while
}//end of task
//////////////////////////////////////////////////////////////////////////////////

//------------CORE_1 task---------------------------------------------------------
//---------------------Function that implements the task being create-------------
void vTaskCore_1 ( void * pvParameters ) {
// The parameter value is expected to be 1 as 1 is passed in the
// pvParameters value in the call to xTaskCreateStaticPinnedToCore().
configASSERT( ( uint32_t ) pvParameters == 1UL );
ESP_LOGI(pcTaskGetName(NULL), "CORE_1");

//--------init wifi------------------
initWifi();
/////////////////////////////////////

while (1) {
if ( alarm_5_second_tick() == 5) {
ESP_LOGI(TAG, "\t--Sending data to WiFi--\t\n");
sendtowifi(&vcc,&raw,&i); //set some timer in which data will send
}
vTaskDelay(1);
//----testing--maintain set voltage-----------------------
if (7 > (vcc )) {  //read voltage less than set voltage, increment in pwm
    Mpwm = Mpwm - 1;
    if (0 > Mpwm) { //constarin pwm
        Mpwm = 500;
    }
} else if (7 < (vcc )) { //read voltage more than set voltage, decrement in pwm
    Mpwm = Mpwm + 1;
}
//------constarin pwm in range--------------------
if (4096 < Mpwm) { //constarin pwm
    Mpwm = 4095;
} 

setPwmMosfet (Mpwm); //12 bit timer: 4096, 72:  | 100: | 600: 14.83%  | 1500: 36.63% | 3000: 73.2% , 4000: 97.75%, 4095: 99.98%
ESP_LOGI(TAG, "readMosfet pwm: %d, set voltage: 7, read Voltage: %f ",Mpwm,vcc);  //esp_rom_delay_us(1); esp idf version 5 ets delay name change 
} //end of while
}//end of task
/////////////////////////////////////////////////////////////////////////////////

//----------Thread_0-----ADC-----------------------------------------------------
void adc_thread_0(void *pvParameters) {
ESP_LOGI(pcTaskGetName(NULL), "ADC_Thread_0");
while (1) {
vTaskDelay(1);
vcc = printADC_Count_Voltage (&raw) * 10; //opamp, 0.1 gain.
} //end of while
}//end of task
/////////////////////////////////////////////////////////////////////////////////

//----------Thread_0---Rotary--Encoder-----------------------------------------------------
void rotary_encoder_thread_0(void *pvParameters) {
ESP_LOGI(pcTaskGetName(NULL), "Rotary_Encoder_Thread_0");
while (1) {
vTaskDelay(1);
count_ = read_Rotary_Encoder ();
} //end of while
}//end of task
/////////////////////////////////////////////////////////////////////////////////