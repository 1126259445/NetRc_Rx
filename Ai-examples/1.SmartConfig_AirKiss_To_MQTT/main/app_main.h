#ifndef __APP_MIAN_H__
#define __APP_MIAN_H__
#include "freertos/semphr.h"

extern bool isConnect2Server;
extern bool isWifiConnectd;
extern esp_mqtt_client_handle_t client;
extern SemaphoreHandle_t xSemaphore;
#endif

