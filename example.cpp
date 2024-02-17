#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <stdlib.h>
#include "ma_api_wifi_auto_ap_station.h"

#define DF_FORMAT_SPIFFS_IF_FAILED true //true or false

void setup() {
//Necessary when ESP32 or Devkit does not have a capacitor strong enough to withstand peak communications consumption (WiFi)
#ifdef BROWNOT_OFF
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector. 
#endif
  Serial.begin(115200);
  if(!SPIFFS.begin(DF_FORMAT_SPIFFS_IF_FAILED))
  {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
  ma_api_wifi_init();
}

void loop() 
{

}




