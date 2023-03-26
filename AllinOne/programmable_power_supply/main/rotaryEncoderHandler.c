#include "rotaryEncoderHandler.h"
//--------- gpio and encoder value of rotation-----------------
#define EXAMPLE_PCNT_HIGH_LIMIT 150
#define EXAMPLE_PCNT_LOW_LIMIT  -1
#define EXAMPLE_EC11_GPIO_A 26
#define EXAMPLE_EC11_GPIO_B 25
////////////////////////////////////////////////////////////////

//---------------Report counter value--------------------------
int pulse_count = 0;
volatile int event_count = 0;
static const char *TAG_Encoder = "Rotary_Encoder";
///////////////////////////////////////////////////////////

//-------define global for flexibility---------------------
// #define length 10
// #define si 4
QueueHandle_t queue_global = NULL; //local to global
pcnt_unit_handle_t pcnt_unit = NULL;
////////////////////////////////////////////////////////////

//---------------call back of count--------------------------------------------------------------------------------
// static bool example_pcnt_on_reach(pcnt_unit_handle_t unit, const pcnt_watch_event_data_t *edata, void *user_ctx) {
//     BaseType_t high_task_wakeup;
//     QueueHandle_t queue = (QueueHandle_t)user_ctx;
//     // send event data to queue, from this interrupt callback
//     xQueueSendFromISR(queue, &(edata->watch_point_value), &high_task_wakeup);
//     return (high_task_wakeup == pdTRUE);
// }
////////////////////////////////////////////////////////////////////////////////
//-------------init the pin------------------------------------------------------
void init_Rotary_Encoder () {
        ESP_LOGI(TAG_Encoder, "install pcnt unit");
    pcnt_unit_config_t unit_config = {
        .high_limit = EXAMPLE_PCNT_HIGH_LIMIT,
        .low_limit = EXAMPLE_PCNT_LOW_LIMIT,
    };
    // pcnt_unit_handle_t pcnt_unit = NULL;
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    ESP_LOGI(TAG_Encoder, "set glitch filter");
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 1000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));

    ESP_LOGI(TAG_Encoder, "install pcnt channels");
    pcnt_chan_config_t chan_a_config = {
        .edge_gpio_num = EXAMPLE_EC11_GPIO_A,
        .level_gpio_num = EXAMPLE_EC11_GPIO_B,
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_a_config, &pcnt_chan_a));
    pcnt_chan_config_t chan_b_config = {
        .edge_gpio_num = EXAMPLE_EC11_GPIO_B,
        .level_gpio_num = EXAMPLE_EC11_GPIO_A,
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_b_config, &pcnt_chan_b));

    ESP_LOGI(TAG_Encoder, "set edge and level actions for pcnt channels");
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    // ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
    // ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
     ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    // ESP_LOGI(TAG_Encoder, "add watch points and register callbacks");
    // int watch_points[] = {EXAMPLE_PCNT_LOW_LIMIT, -50, 0, 50, EXAMPLE_PCNT_HIGH_LIMIT};
    // for (size_t i = 0; i < sizeof(watch_points) / sizeof(watch_points[0]); i++) {
    //     ESP_ERROR_CHECK(pcnt_unit_add_watch_point(pcnt_unit, watch_points[i]));
    // }
    // pcnt_event_callbacks_t cbs = {
    //     .on_reach = example_pcnt_on_reach,
    // };
    // QueueHandle_t queue = xQueueCreate(10, sizeof(int));
    // ESP_ERROR_CHECK(pcnt_unit_register_event_callbacks(pcnt_unit, &cbs, queue));
    // queue_global = queue; //copty to global queue for usable in other appications
    ESP_LOGI(TAG_Encoder, "enable pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_LOGI(TAG_Encoder, "clear pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_clear_count(pcnt_unit));
    ESP_LOGI(TAG_Encoder, "start pcnt unit");
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));

#if CONFIG_EXAMPLE_WAKE_UP_LIGHT_SLEEP
    // EC11 channel output high level in normal state, so we set "low level" to wake up the chip
    ESP_ERROR_CHECK(gpio_wakeup_enable(EXAMPLE_EC11_GPIO_A, GPIO_INTR_LOW_LEVEL));
    ESP_ERROR_CHECK(esp_sleep_enable_gpio_wakeup());
    ESP_ERROR_CHECK(esp_light_sleep_start());
#endif
}
//////////////////////////////////////////////////////////////////////////

//-----------------print the read value from encoder-----------------------
int read_Rotary_Encoder () {
// // Report counter value
// if (xQueueReceive(queue_global, &event_count, pdMS_TO_TICKS(1000))) {
//     ESP_LOGI(TAG_Encoder, "Watch point event, count: %d", event_count);
// } else {
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &pulse_count));
    ESP_LOGI(TAG_Encoder, "Pulse count: %d", pulse_count);
// }
//vTaskDelay(pdMS_TO_TICKS(100));
return pulse_count;
}
////////////////////////////////////////////////////////////////////////////