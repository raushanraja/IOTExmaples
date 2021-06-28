#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
void startWiFi()
{
    WiFi.disconnect();
    Serial.println("Inside Start WiFi");
    delay(1000);
    // WiFi.begin("TP-Link_9C82", "38702303");
    // WiFi.begin("redmii", "1234467890");
    WiFi.begin("dlink_DWR-920V_6124", "Dcvve45447");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    Serial.println(WiFi.localIP());
}