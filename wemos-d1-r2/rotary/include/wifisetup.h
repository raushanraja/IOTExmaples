#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SSID "SSID"
#define PASSWORD "PASSWORD12234554"


void startWiFi()
{
    WiFi.disconnect();
    Serial.println("Inside Start WiFi");
    delay(1000);
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    Serial.println(WiFi.localIP());
}