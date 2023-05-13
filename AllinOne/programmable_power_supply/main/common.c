/*
Written by Abhishek Tiwari
*/
//--------header file has macros and functions-------
#include "common.h"
#include "i2c.h"  //able to call initi2c function
/////////////////////////////////////////////////////

//--------------------config files of adc and dac-------------------------
//-------mcp4725 config-------------------------
mcp4725_config_t mcp4725_t = {
.address = I2C_MCP4725, //i2c address
.i2c_bus = I2C_NUM,
.ticks_to_wait = 1000,
};
////////////////////////////////////////////////
//-----ads1115 adc config-----------------------
ads1115_t ads1115_cfg = {
.reg_cfg =  ADS1115_CFG_LS_COMP_MODE_TRAD | // Comparator is traditional
              ADS1115_CFG_LS_COMP_LAT_NON |   // Comparator is non-latching
              ADS1115_CFG_LS_COMP_POL_LOW |   // Alert is active low
              ADS1115_CFG_LS_COMP_QUE_ONE |   // Compator is 1 shot
              ADS1115_CFG_MS_PGA_FSR_6_144V | //PROGRAMMABLE GAIN
              ADS1115_CFG_LS_DR_860SPS |     // No. of samples to take
              ADS1115_CFG_MS_MODE_CON,         // Mode is set to cont-shot
.dev_addr = I2C_ADS1115, //i2c address
};
////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//----variables and definations------------------------------------
uint16_t raw_adc_count;
#define ready_pin 34 
////////////////////////////////////////////////////////////////////

//------initialize i2c library-----
void init_peripherals () {
ESP_LOGI(TAG_COMMON,"Set up i2c Config");
init_I2C ();
  ESP_LOGI(TAG_COMMON, "intial ready pin gpio34 ");
  gpio_reset_pin((gpio_num_t)ready_pin);
  gpio_set_direction((gpio_num_t)ready_pin,GPIO_MODE_INPUT);
  gpio_set_pull_mode((gpio_num_t)ready_pin,GPIO_PULLUP_ONLY);
}
//////////////////////////////////////////////////

//------initialize the adc config in library-----
void init_Adc_Config () {
ESP_LOGI(TAG_COMMON,"Set up ADS1115 Config");
ADS1115_initiate(&ads1115_cfg);
}
//////////////////////////////////////////////////

//--------set dac output voltage-----------------
void set_Dac_Voltage (uint16_t dac_Count) {
ESP_LOGI(TAG_COMMON,"Set DAC 12 bit: %d",dac_Count);
if (dac_Count > 4095) {
dac_Count = 4095 ;
}
mcp4725_set_voltage(mcp4725_t,dac_Count); //100: 1.605v , 12 bit= 4095= 3.29v  
vTaskDelay(pdMS_TO_TICKS(10)); //settle down
}
////////////////////////////////////////////////

//-----------------print adc raw reading with measured voltage---------------
float printADC_Count_Voltage (uint16_t *result) {
ADS1115_request_single_ended_AIN0();     
// Check conversion state - returns true if conversion is complete =]=]=]
// while(!ADS1115_get_conversion_state()) 
vTaskDelay(1);          // wait before check again
if (gpio_get_level(34) == 1) {
// Return latest conversion value
raw_adc_count = ADS1115_get_conversion();
*result = raw_adc_count;   
}   
float vol = computeVolts(raw_adc_count);
//ESP_LOGI(TAG_COMMON,"Conversion Value: %d, measure voltage %f", raw_adc_count,vol);     
return vol;
}
////////////////////////////////////////////////////////////////////////////

//-----------calculate volTAG_COMMONe from raw adc reading----------------
float computeVolts(int16_t counts) {
float fsRange = 6.144f;
return counts * (fsRange / (32768 >> 0));
}
///////////////////////////////////////////////////////////////////////////////
