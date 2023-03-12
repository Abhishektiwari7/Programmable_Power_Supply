/*
Written by Abhishek Tiwari
*/
#ifdef __cplusplus
extern "C" {
#endif

#ifndef COMMON_H
#define COMMON_H

//---libraries-----------------------
#include <stdio.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "ADS1115.h"
#include "mcp4725.h"
#include "i2c.h"
/////////////////////////////////////

//----------custom defines----------
static const char *TAG_COMMON = "COMMON";
////////////////////////////////////

//---------set i2c address------------
#define I2C_MCP4725 (0x60)   
#define I2C_ADS1115 (0x48)   
/////////////////////////////////////



//-------functions are defined here---------------------------

//----------init i2c library--------------------
void init_peripherals ();
////////////////////////////////////////////////

//------initialize the adc config in library-----
void init_Adc_Config ();
//////////////////////////////////////////////////

//--------set dac output voltage-----------------
void set_Dac_Voltage (uint16_t dac_Count);
////////////////////////////////////////////////

//-----------------print adc raw reading with measured voltage---------------
float printADC_Count_Voltage (uint16_t *result);
///////////////////////////////////////////////////////////////

//-----------calculate voltage from raw adc reading----------------
float computeVolts(int16_t counts);
///////////////////////////////////////////////////////////////////////////////

#endif
#ifdef __cplusplus
}
#endif// COMMON_H 