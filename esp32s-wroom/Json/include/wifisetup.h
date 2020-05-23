#include <WiFi.h>

void startWiFi()
{
    WiFi.disconnect();
    Serial.println("Inside WiFi Server");
    delay(1000);
    WiFi.mode(WIFI_STA);
    WiFi.begin("Android_84", "1234467890");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }
    Serial.println(WiFi.localIP());
}