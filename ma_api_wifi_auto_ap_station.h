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
  String clsStringSsid;
  String clsStringPassword;
}st_wifi_station_credential_t;

/* Public objects ------------------------------------------------------------*/
extern void ma_api_wifi_init(void);
extern void ma_api_wifi_update_network_credentials(const String in_ssid, const String in_password);
extern void ma_api_wifi_read_network_credentials(st_wifi_station_credential_t *out_credential);
extern void ma_api_wifi_process_client_request(WiFiClient in_client, String in_oldSsid, String in_oldPassword) ;

#endif /* __MA_API_WIFI_H */
/*****************************END OF FILE**************************************/