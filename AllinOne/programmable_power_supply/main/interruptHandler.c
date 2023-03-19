#include "interruptHandler.h" //timer and gpio interrupt

//----------------timer macros------------------
#define TIMER0_SECOND         (1000000)  //  Hardware timer clock divider
////////////////////////////////////////////////

//----------pin gpio number set-----------------
//Pins with internal pull up INPUT_PULLUP , NOT ON gpio12
#define GPIO_INTERRUPT_SW0 (gpio_num_t)(14)
#define GPIO_INTERRUPT_SW1 (gpio_num_t)(16)
#define GPIO_INTERRUPT_SW2 (gpio_num_t)(17)
#define GPIO_INTERRUPT_SW3 (gpio_num_t)(12)
volatile uint8_t switch_State_0 = 0;
////////////////////////////////////////////////

//------------switch 1 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw0 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
// state = gpio_get_level(GPIO_LED);
gpio_set_level(GPIO_LED,1);
ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 0"); //able to use in interrupt isr unlike esp_logi it crash code
}
//////////////////////////////////////////////////////

//------------switch 2 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw1 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
// state = gpio_get_level(GPIO_LED);
gpio_set_level(GPIO_LED,0);
ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 1");
}
///////////////////////////////////////////////////////

//------------switch 3 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw2 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
// state = gpio_get_level(GPIO_LED);
gpio_set_level(GPIO_LED,1);
ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 2");
}
///////////////////////////////////////////////////////

//------------switch 4 Interrupt Handdler-------------
static void IRAM_ATTR gpio_isr_handler_sw3 (void *arg) {
//	gpio_set_level(GPIO_LED,gpio_get_level(GPIO_LED));
gpio_set_level(GPIO_LED,0);
ESP_DRAM_LOGI(TAG_INT, "Interrupt Handler Call: Switch 3");
}
////////////////////////////////////////////////////////

//-------timer interrupt callback handler---------------
static bool timer_0_sec(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
BaseType_t high_task_awoken = pdFALSE;
// Optional: send the event data to other task by OS queue
//-add here specific variables for toogle
static bool ON;
ON = !ON;  
gpio_set_level(GPIO_LED, ON);
// return whether we need to yield at the end of ISR
return high_task_awoken == pdTRUE;
}
/////////////////////////////////////////////////////////////

//----------attach interrupt gpio to isr handler--------
void init_Interrupt_Gpio () {
//interrupt setting
gpio_config_t io_conf_0,io_conf_1,io_conf_2,io_conf_3;
//interrupt of falling edge
io_conf_0.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
io_conf_1.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
io_conf_2.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
io_conf_3.intr_type = GPIO_INTR_NEGEDGE; // GPIO interrupt type : falling edge
//bit mask of the pins
io_conf_0.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW0;
io_conf_1.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW1;
io_conf_2.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW2;
io_conf_3.pin_bit_mask = 1ULL<<GPIO_INTERRUPT_SW3;
//set as input mode
io_conf_0.mode = GPIO_MODE_INPUT;
io_conf_1.mode = GPIO_MODE_INPUT;
io_conf_2.mode = GPIO_MODE_INPUT;
io_conf_3.mode = GPIO_MODE_INPUT;
//enable pull-up mode
io_conf_0.pull_up_en = GPIO_PULLUP_ENABLE;
io_conf_1.pull_up_en = GPIO_PULLUP_ENABLE;
io_conf_2.pull_up_en = GPIO_PULLUP_ENABLE;
io_conf_3.pull_up_en = GPIO_PULLUP_ENABLE;

io_conf_0.pull_down_en = GPIO_PULLDOWN_DISABLE;
io_conf_1.pull_down_en = GPIO_PULLDOWN_DISABLE;
io_conf_2.pull_down_en = GPIO_PULLDOWN_DISABLE;
io_conf_3.pull_down_en = GPIO_PULLDOWN_DISABLE;

gpio_config(&io_conf_0);
gpio_config(&io_conf_1);
gpio_config(&io_conf_2);
gpio_config(&io_conf_3);

//install gpio isr service
gpio_install_isr_service(0);
//hook isr handler for specific gpio pin
gpio_isr_handler_add(GPIO_INTERRUPT_SW0, gpio_isr_handler_sw0, (void*) GPIO_INTERRUPT_SW0);
gpio_isr_handler_add(GPIO_INTERRUPT_SW1, gpio_isr_handler_sw1, (void*) GPIO_INTERRUPT_SW1);
gpio_isr_handler_add(GPIO_INTERRUPT_SW2, gpio_isr_handler_sw2, (void*) GPIO_INTERRUPT_SW2);
gpio_isr_handler_add(GPIO_INTERRUPT_SW3, gpio_isr_handler_sw3, (void*) GPIO_INTERRUPT_SW3);

}
///////////////////////////////////////////////////////////////////////////////////////////

//-------init of timer_0 interrupt for seconds---------------
void init_Interrupt_Timer0_Sec () {
gptimer_handle_t gptimer = NULL;
gptimer_config_t timer_config;
timer_config.clk_src = GPTIMER_CLK_SRC_DEFAULT;
timer_config.direction = GPTIMER_COUNT_UP;
timer_config.resolution_hz = 1000000; // 1MHz, 1 tick=1us

ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &gptimer)); //create Timer
//---attach call back function--------
gptimer_event_callbacks_t cbs; //call back function struct
cbs.on_alarm = timer_0_sec;
ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));

ESP_LOGI(TAG_INT, "Enable timer");
ESP_ERROR_CHECK(gptimer_enable(gptimer)); //enable timer


gptimer_alarm_config_t alarm_config1;
alarm_config1.reload_count = 0; //starting count of timer
alarm_config1.flags.auto_reload_on_alarm = true;  //auto reload enable
alarm_config1.alarm_count = TIMER0_SECOND; // period = 1s //set by user
ESP_ERROR_CHECK(gptimer_set_alarm_action(gptimer, &alarm_config1)); //set in action
ESP_ERROR_CHECK(gptimer_start(gptimer));   //start the timer to count
}
////////////////////////////////////////////////////////////////////////////////////