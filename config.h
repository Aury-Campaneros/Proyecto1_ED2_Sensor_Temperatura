/************************ Configuración Adafruit IO  *******************************/

#define IO_USERNAME  "AuraC"
#define IO_KEY       "aio_vCLF44Uq63kfuEau4vs2WYvUD52e"

/******************************* WIFI **************************************/
#define WIFI_SSID "Redmi 12C"
#define WIFI_PASS "ngtprachch4g9p6"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
