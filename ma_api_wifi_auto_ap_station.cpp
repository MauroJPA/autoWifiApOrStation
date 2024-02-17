/**
  ******************************************************************************
  * @Company    : Mauro Almeida.
  * @file       : ma_api_wifi.cpp
  * @author     : Mauro Almeida
  * @version	: V0.0 
  * @date       : 09/02/2024
  * @brief      : API of WiFi module
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/ 
// C language standard library

// Mauro Almeida driver library

// API library
#include "ma_api_wifi_auto_ap_station.h"

/*******************************************************************************
							HOW TO USE THIS API
********************************************************************************

1. 	First, you should include in your .c file the 
    "ma_api_rain_sensor.h" file.

2.  Call ma_api_rain_sensor_init(DESIRED-PIN) to initialize and setup 
    the sensor parameters.

3.  Call ma_api_rain_sensor_value() to get the sensor raw value.

4.  Call ma_api_rain_sensor_average_percentage_value() to get the sensor 
    average in percentage value.

*******************************************************************************/

/* Private define ------------------------------------------------------------*/
#define DF_IP_ADDRESS                   IPAddress(192, 168, 123, 123)
#define DF_IP_MASK                      IPAddress(255, 255, 255, 0)

#define DF_SSID_AP                      "SOBREIRO_MONITOR"
#define DF_PASSWORD_AP                  "12345678"
/* Private macros ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

// Set web server port number to 80
WiFiServer clsWifiServer(80);

// Variable to store the HTTP request
String clsStringHeader;

// Variable to store the Wifi Credentials
st_wifi_station_credential_t stWifiStationCredential;

/* Private function prototypes -----------------------------------------------*/  
void ma_api_wifi_get_token(String in_header, String *out_ssid, String *out_password);

/* Public objects ------------------------------------------------------------*/


/* Body of private functions -------------------------------------------------*/

/**
  * @Func       : ma_api_wifi_init  
  * @brief      : This function initializes the WiFi module in either Station mode or Access Point (AP) mode based on 
  *                 the provided mode.
  * 
  * @pre-cond.  : Installation of SPIFFS and FS library and SPIFFS.begin() must be called before using this function.
  * @post-cond. : WiFi is initialized and ready to be used.
  * @parameters : eWIFI_MODE: The WiFi mode to initialize (WIFI_MODE_STA for Station mode, WIFI_MODE_AP for AP mode).
  * @retval     : None
  */
void ma_api_wifi_init(void) 
{
    ma_api_wifi_read_network_credentials(&stWifiStationCredential);
    if(stWifiStationCredential.clsStringSsid.isEmpty() || stWifiStationCredential.clsStringPassword.isEmpty())
    {
        PRINTF("INICIANDO EM WIFI STATION POR QUE TEMOS DADOS NA MEMORIA\n");
    }
    else
    {
        PRINTF("Setting AP (Access Point)… Only to set SSID and PASSWORD.\n");
        WiFi.mode(WIFI_AP); 
        WiFi.softAP(DF_SSID_AP, DF_PASSWORD_AP);   //launch the access point
        PRINTF("Wait 100 ms for AP_START...\n");
        delay(100);
        PRINTF("Setting the AP\n");
        PRINTF("AP IP address: %s\n", DF_IP_ADDRESS);
        WiFi.softAPConfig(DF_IP_ADDRESS, DF_IP_ADDRESS, DF_IP_MASK);
        clsWifiServer.begin();
    }
}



/**
  * @Func       : ma_api_wifi_send_http_response
  * @brief      : This function sends an HTTP response containing an HTML page to the client
  * @pre-cond.  : The client must be connected and ready to receive the response
  * @post-cond. : The HTTP response is sent to the client
  * @parameters : in_client: The WiFiClient object representing the client to which the response will be sent
  * @retval     : None
  */
void ma_api_wifi_send_http_response(WiFiClient in_client) {
  in_client.println("HTTP/1.1 200 OK");
  in_client.println("Content-type:text/html");
  in_client.println("Connection: close");
  in_client.println();
  in_client.println("<!DOCTYPE html><html>");
  in_client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  in_client.println("<link rel=\"icon\" href=\"data:,\">");
  in_client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
  in_client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
  in_client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
  in_client.println(".button2 {background-color: #555555;}</style></head>");
  in_client.println("<body><h1>Sobreiro Monitor</h1>");
  in_client.println("<form id=\"formSalvar\" action=\"/save_data\" method=\"post\">");
  in_client.println("<p>Digite o SSID: </p>");
  in_client.println("<p><input type=\"text\" name=\"user\" value=\"" + stWifiStationCredential.clsStringSsid + "\"><p>");
  in_client.println("<p>Digite a SENHA: </p>");
  in_client.println("<p><input type=\"password\" name=\"password\" id=\"password\" value=\"" + stWifiStationCredential.clsStringPassword + "\"></p>");
  in_client.println("<button type=\"button\" onclick=\"togglePasswordVisibility()\">Mostrar/Ocultar Senha</button>");
  in_client.println("<p><input type=\"submit\" value=\"Salvar\"></p>");
  in_client.println("</form>");
  in_client.println("<script>");
  in_client.println("function togglePasswordVisibility() {");
  in_client.println("  var passwordField = document.getElementById('password');");
  in_client.println("  if (passwordField.type === 'password') {");
  in_client.println("    passwordField.type = 'text';");
  in_client.println("  } else {");
  in_client.println("    passwordField.type = 'password';");
  in_client.println("  }");
  in_client.println("}");

  in_client.println("document.getElementById('formSalvar').addEventListener('submit', function(event) {");
  in_client.println("  event.preventDefault();"); // Evita que o formulário seja enviado automaticamente
  in_client.println("  var ssid = document.getElementsByName('user')[0].value;");
  in_client.println("  var password = document.getElementsByName('password')[0].value;");
  in_client.println("  var form = document.getElementById('formSalvar');");
  in_client.println("  var url = '/save_data?ssid=' + encodeURIComponent(ssid) + '&password=' + encodeURIComponent(password);");
  in_client.println("  form.action = url;"); // Define a URL com os parâmetros ssid e password como o atributo action do formulário
  in_client.println("  form.submit();"); // Envia o formulário
  in_client.println("});");

  in_client.println("</script>");
  in_client.println("</body></html>");
}


/**
  * @Func       : ma_api_wifi_process_client_request
  * @brief      : This function processes the client request and sends an HTTP response
  * @pre-cond.  : The client must be connected and ready to receive the response
  * @post-cond. : The HTTP response is sent to the client
  * @parameters : 
  *       - in_client: The WiFiClient object representing the client that sent the request
  *       - in_oldSsid: The old SSID stored in memory
  *       - in_oldPassword: The old password stored in memory
  * @retval     : None
  */
void ma_api_wifi_process_client_request(WiFiClient in_client, String in_oldSsid, String in_oldPassword) 
{
    String header = "";
    String currentLine = "";

    // Ler o cabeçalho HTTP linha por linha
    while (in_client.connected()) {
        if (in_client.available()) {
        char c = in_client.read();
        //Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {
            if (currentLine.length() == 0) {
            // Cabeçalho HTTP completo recebido
            in_client.println("HTTP/1.1 200 OK");
            in_client.println("Content-type:text/html");
            in_client.println("Connection: close");
            in_client.println();

            // Enviar resposta HTTP
            ma_api_wifi_send_http_response(in_client);
            break;
            } else {
            currentLine = "";
            }
        } else if (c != '\r') {
            currentLine += c;
        }
        }
    }

    ma_api_wifi_get_token(header, &stWifiStationCredential.clsStringSsid, &stWifiStationCredential.clsStringPassword);

    if(in_oldSsid != stWifiStationCredential.clsStringSsid || in_oldPassword != stWifiStationCredential.clsStringPassword)
    {
        ma_api_wifi_update_network_credentials(stWifiStationCredential.clsStringSsid, stWifiStationCredential.clsStringPassword);
        PRINTF("Novas credenciais salvas na memória:\n    SSID: %s\n   PASSWORD: %s\n", stWifiStationCredential.clsStringSsid, stWifiStationCredential.clsStringPassword);
    }
    in_client.stop();
}

/**
  * @Func       : ma_api_wifi_get_token
  * @brief      : Extracts SSID and password from the URL parameters in the header
  * @pre-cond.  : Client connected and send data via Method POST
  * @post-cond. : SSID and password are extracted from the URL parameters
  * @parameters : 
  *       - in_header: The header string containing the URL parameters
  *       - out_ssid: Pointer to a string to store the extracted SSID
  *       - out_password: Pointer to a string to store the extracted password
  * @retval     : None
  */
void ma_api_wifi_get_token(String in_header, String *out_ssid, String *out_password) 
{
    // Find the position of the first '?' in the URL (if any)
    int index = in_header.indexOf('?');
    if (index != -1) {
        // Get the part of the URL after the '?' (which contains the parameters)
        String params = in_header.substring(index + 1);

        // Find the position of the start of the SSID in the parameters string
        int ssidIndex = params.indexOf("ssid=");
        if (ssidIndex != -1) {
            // Extract the value of the SSID (after "ssid=")
            int delimiterIndex[5];
            delimiterIndex[0] = params.indexOf('&', ssidIndex);
            delimiterIndex[1] = params.indexOf(' ', ssidIndex);
            delimiterIndex[2] = params.indexOf('\n', ssidIndex);
            delimiterIndex[3] = params.indexOf('\r', ssidIndex);
            delimiterIndex[4] = params.indexOf('/', ssidIndex);

            // Find the next delimiter ('&', ' ', '\n', '\r', or '/') after the SSID
            int ssidEndIndex = params.length(); // Initialize with the length of the string to ensure a default value
            for (int i = 0; i < 5; i++) {
                if (delimiterIndex[i] != -1 && delimiterIndex[i] < ssidEndIndex) {
                    ssidEndIndex = delimiterIndex[i];
                }
            }

            *out_ssid = params.substring(ssidIndex + 5, ssidEndIndex);
            PRINTF("Found SSID: %s\n", *out_ssid);
        }

        // Find the position of the start of the password in the parameters string
        int passwordIndex = params.indexOf("password=");
        if (passwordIndex != -1) {
            // Extract the value of the password (after "password=")
            int delimiterIndex[5];
            delimiterIndex[0] = params.indexOf('&', passwordIndex);
            delimiterIndex[1] = params.indexOf(' ', passwordIndex);
            delimiterIndex[2] = params.indexOf('\n', passwordIndex);
            delimiterIndex[3] = params.indexOf('\r', passwordIndex);
            delimiterIndex[4] = params.indexOf('/', passwordIndex);

            // Find the next delimiter ('&', ' ', '\n', '\r', or '/') after the password
            int passwordEndIndex = params.length(); // Initialize with the length of the string to ensure a default value
            for (int i = 0; i < 5; i++) {
                if (delimiterIndex[i] != -1 && delimiterIndex[i] < passwordEndIndex) {
                    passwordEndIndex = delimiterIndex[i];
                }
            }

            *out_password = params.substring(passwordIndex + 9, passwordEndIndex);
            PRINTF("Found Password: %s\n", *out_password);
        }

    } else {
        PRINTF("Don't have parameters in URL.\n");
    }
}


/**
  * @Func       : ma_api_wifi_update_network_credentials
  * @brief      : Updates and saves network credentials to SPIFFS
  * @pre-cond.  : SPIFFS must be initialized and the file system must be accessible
  * @post-cond. : Network credentials are updated and saved to SPIFFS
  * @parameters : 
  *       - in_ssid: The new SSID to be saved
  *       - in_password: The new password to be saved
  * @retval     : None
  */
void ma_api_wifi_update_network_credentials(const String in_ssid, const String in_password) 
{
    // Open the file for writing to SPIFFS
    File file = SPIFFS.open("/wifi_credentials.txt", "w+");
    if (!file) {
        PRINTF("Error opening file for writing.\n");
        return;
    }

    // Write the credentials to the file
    file.println("SSID: " + in_ssid);
    file.println("Password: " + in_password);

    // Close the file
    file.close();

    PRINTF("New credentials saved in SPIFFS.\n");
}


/**
  * @Func       : ma_api_wifi_read_network_credentials
  * @brief      : Reads network credentials from SPIFFS and updates the provided structure
  * @pre-cond.  : SPIFFS must be initialized and the file system must be accessible
  * @post-cond. : Network credentials are read from SPIFFS and updated in the structure
  * @parameters : 
  *       - in_credentials: Pointer to the structure where the credentials will be stored
  * @retval     : None
  */
void ma_api_wifi_read_network_credentials(st_wifi_station_credential_t *in_credentials) 
{
    // Open the file for reading from SPIFFS
    File file = SPIFFS.open("/wifi_credentials.txt", "r");
    if (!file) {
        PRINTF("Error opening file for reading.\n");
        return;
    }

    // Read lines from the file and extract the credentials
    String line;
    while (file.available()) {
        line = file.readStringUntil('\n');
        if (line.startsWith("SSID: ")) {
            in_credentials->clsStringSsid = line.substring(6); // Remove "SSID: " from the line
        } else if (line.startsWith("Password: ")) {
            in_credentials->clsStringPassword = line.substring(10); // Remove "Password: " from the line
        }
    }

    // Close the file
    file.close();

    PRINTF("Credentials read from SPIFFS and updated in the credential structure.\n");
}

/*****************************END OF FILE**************************************/