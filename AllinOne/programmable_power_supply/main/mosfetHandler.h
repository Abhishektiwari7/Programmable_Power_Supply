/*
Written by Abhishek Tiwari
*/
#ifdef __cplusplus
extern "C"
{
#endif

#ifndef MOSFETHANDLER_H
#define MOSFETHANDLER_H
//----header files----------------------
#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
///////////////////////////////////////

//---------pwm macros------------------------

//---------------------------PWM----------------------------------------------------------------------
const int MOSFET_PWM = 27;

// // Set the LEDC peripheral configuration
#define MOSFET_TIMER LEDC_TIMER_0
#define MOSFET_MODE LEDC_HIGH_SPEED_MODE
#define MOSFET_OUTPUT_IO MOSFET_PWM // Define the output GPIO
#define MOSFET_CHANNEL LEDC_CHANNEL_1
#define MOSFET_DUTY_RES LEDC_TIMER_12_BIT // LEDC_TIMER_13_BIT //for 5khz Set duty resolution to 13 bits
#define MOSFET_FREQUENCY (180*100)        //(5000) // Frequency in Hertz. Set frequency at 5 kHz
//////////////////////////////////////////////

//-------------functions--------------------

void initMosfetHandler();
void setPwmMosfet(uint32_t mPwm);
/////////////////////////////////////////////

#endif
#ifdef __cplusplus
}
#endif // MOSFETHANDLER_H
