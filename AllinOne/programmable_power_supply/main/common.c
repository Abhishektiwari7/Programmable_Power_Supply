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
              ADS1115_CFG_LS_COMP_QUE_DIS |   // Compator is disabled
              ADS1115_CFG_LS_DR_1600SPS |     // No. of samples to take
              ADS1115_CFG_MS_MODE_SS,         // Mode is set to single-shot
.dev_addr = I2C_ADS1115, //i2c address
};
////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//------initialize i2c library-----
void init_peripherals () {
ESP_LOGI(TAG_COMMON,"Set up i2c Config");
init_I2C ();
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
ESP_LOGI(TAG_COMMON,"Set DAC..");
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
while(!ADS1115_get_conversion_state()) 
vTaskDelay(1);          // wait before check again   
// Return latest conversion value
uint16_t raw_adc_count = ADS1115_get_conversion();
*result = raw_adc_count;   
float vol = computeVolts(raw_adc_count);
ESP_LOGI(TAG_COMMON,"Conversion Value: %d, measure voltage %f", raw_adc_count,vol);     
return vol;
}
////////////////////////////////////////////////////////////////////////////

//-----------calculate volTAG_COMMONe from raw adc reading----------------
float computeVolts(int16_t counts) {
float fsRange = 6.144f;
return counts * (fsRange / (32768 >> 0));
}
///////////////////////////////////////////////////////////////////////////////
