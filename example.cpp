#include <Arduino.h>
#include <WiFi.h>
#include <FS.h>
#include <SPIFFS.h>
#include <stdlib.h>
#include "ma_api_wifi_auto_ap_station.h"

#define DF_FORMAT_SPIFFS_IF_FAILED true //true or false
WiFiClient wifiClient;
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

  st_wifi_station_credential_t stWifiCredential;
  ma_api_wifi_read_network_credentials(&stWifiCredential);
  if(stWifiCredential.stringSsid.length() < 5 || stWifiCredential.stringPassword.length() < 5)
  {
    //Aqui deve de ativar o modo Activo
    ma_api_wifi_setup_access_point(stWifiCredential); // Enable AP mode with WebServer to get news credentials if is necessary
    
    while(1)
    {
      ma_api_wifi_process_client_request(&wifiClient, stWifiCredential.stringSsid, stWifiCredential.stringPassword, 60);
    }
  }
  ma_api_wifi_setup_station(stWifiCredential, 3);
}

void loop() 
{

}




