#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include <nvs_memory.h>
#include "pinconfig.h"
#include "i2c.hpp"
#include <esp_log.h>
#include "app.h"
#include "pinconfig.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <system.h>
#include <net/esp32inet.h>
#include <i2c.hpp>
#include <device/display/ssd1306.h>
#include <device/sensor/dht11.h>

static const char *TAG = "proj2";
using namespace libesp;

extern "C" {
	void app_main();
}

#define ESP_INTR_FLAG_DEFAULT 0
static const uint32_t CONFIG_BLINK_PERIOD = 1000;

void app_main() {
	static const char *LOGTAG="APP_MAIN";
	gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

	libesp::ErrorType et;
	et = MyApp::get().init();

	if(!et.ok()) {
		ESP_LOGE(LOGTAG,"init error: %s", et.toString());
	}

	libesp::System::get().logSystemInfo();

	if(!(et=ESP32INet::get()->init()).ok()) {
		ESP_LOGE(TAG,"failed to init network %s", et.toString());
	} else {
		ESP32INet::dumpToLog();
	}

   do {
		et = MyApp::get().run();
		vTaskDelay(10 / portTICK_RATE_MS);
	} while (et.ok());

}
