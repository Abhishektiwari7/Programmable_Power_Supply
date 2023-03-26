#include "mosfetHandler.h"

//--------defines-------------------------
ledc_timer_config_t mosfet_timer;     //ledc pwm
ledc_channel_config_t mosfet_channel; //ledc pwm
//////////////////////////////////////////

void initMosfetHandler() {
mosfet_timer.speed_mode       = MOSFET_MODE;
mosfet_timer.timer_num        = MOSFET_TIMER;
mosfet_timer.duty_resolution  = MOSFET_DUTY_RES;
mosfet_timer.freq_hz          = MOSFET_FREQUENCY;  // Set output frequency at ? kHz
mosfet_timer.clk_cfg          = LEDC_AUTO_CLK;
ESP_ERROR_CHECK(ledc_timer_config(&mosfet_timer));

mosfet_channel.speed_mode     = MOSFET_MODE;
mosfet_channel.channel        = MOSFET_CHANNEL;
mosfet_channel.timer_sel      = MOSFET_TIMER;
mosfet_channel.intr_type      = LEDC_INTR_DISABLE;
mosfet_channel.gpio_num       = MOSFET_OUTPUT_IO;
mosfet_channel.duty           = 0; // Set duty to 0%
mosfet_channel.hpoint         = 0;
ESP_ERROR_CHECK(ledc_channel_config(&mosfet_channel));
}

void setPwmMosfet (uint32_t mPwm) {
//   if (mPwm > 512) {
//      mPwm = 512; //max
//    } else if (mPwm < 20 ) {
//       mPwm = 20;  //min
//    }
ESP_ERROR_CHECK(ledc_set_duty(MOSFET_MODE, MOSFET_CHANNEL, mPwm));
ESP_ERROR_CHECK(ledc_update_duty(MOSFET_MODE, MOSFET_CHANNEL));
}