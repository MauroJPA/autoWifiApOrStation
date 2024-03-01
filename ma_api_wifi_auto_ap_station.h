/**
    ******************************************************************************
    * @Company    : Mauro Almeida.
    * @file       : ma_api_wifi.h
    * @author     : Mauro Almeida
    * @version	  : V0.0 
    * @date       : 09/02/2024
    * @brief      : Header file of wifi Api for ESP32 with Arduino Framework
    ******************************************************************************
*/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MA_API_WIFI_H
#define __MA_API_WIFI_H

/* Includes ------------------------------------------------------------------*/  
#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <stdlib.h>

#ifdef BROWNOT_OFF
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#endif

/* Define --------------------------------------------------------------------*/
#ifdef PRINT_ENABLE
#define PRINTF(...) Serial.printf(__VA_ARGS__)
#else 
#define PRINTF(...)
#endif
/* Typedef -------------------------------------------------------------------*/
typedef struct {
  String stringSsid;
  String stringPassword;
  bool hasCredentials;
}st_wifi_station_credential_t;

/* Public objects ------------------------------------------------------------*/
extern int8_t ma_api_wifi_setup_station(st_wifi_station_credential_t in_wifiCredentials, int maxAttempts);
extern void ma_api_wifi_setup_access_point(st_wifi_station_credential_t in_credentials);
extern void ma_api_wifi_update_network_credentials(const String in_ssid, const String in_password);
extern int8_t ma_api_wifi_read_network_credentials(st_wifi_station_credential_t *out_credentials);
extern void ma_api_wifi_process_client_request(WiFiClient *in_wifiClient, String in_oldSsid, String in_oldPassword, uint16_t in_timeToWaitSeconds);

#endif /* __MA_API_WIFI_H */
/*****************************END OF FILE**************************************/