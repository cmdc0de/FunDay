#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <cstring>
#include "driver/gpio.h"
#include <system.h>
#include <net/esp32inet.h>

static const char *TAG = "proj1";
using namespace libesp;

extern "C" {
	void app_main(void);
}

static gpio_num_t BLINK_GPIO = GPIO_NUM_25;
static uint32_t s_led_state = 1;
static const uint32_t CONFIG_BLINK_PERIOD = 1000;

static void blink_led(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(BLINK_GPIO, s_led_state);
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
}

void app_main(void) {
    // Initialize NVS
	esp_err_t ret = nvs_flash_init();
	if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
		ESP_ERROR_CHECK(nvs_flash_erase());
		ret = nvs_flash_init();
	}
	ESP_ERROR_CHECK( ret );

	System::get().logSystemInfo();
	ErrorType et;
	if(!(et=ESP32INet::get()->init()).ok()) {
		ESP_LOGE(TAG,"failed to init network %s", et.toString());
	} else {
		ESP32INet::dumpToLog();
	}

	configure_led();

	while (1) {
		ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
		blink_led();
		/* Toggle the LED state */
		s_led_state = !s_led_state;
		vTaskDelay(CONFIG_BLINK_PERIOD / portTICK_PERIOD_MS);
	}
}
