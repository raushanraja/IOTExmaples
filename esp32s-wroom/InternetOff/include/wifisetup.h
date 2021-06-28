#include <WiFi.h>

void startWiFi()
{
    WiFi.disconnect();
    Serial.println("Inside WiFi Server");
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin("TP-Link_9C82", "38702303");
    // WiFi.begin("redmii", "1234467890");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(2000);
    }
    Serial.println(WiFi.localIP());
}