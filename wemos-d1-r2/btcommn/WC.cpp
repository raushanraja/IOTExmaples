#include "Arduino.h";
#include "WC.h";
#include "ESP8266WiFi.h";

WC::WC(int pin1, int pin2){
    pinMode(pin1,INPUT);
    pinMode(pin2,OUTPUT);
    _pin1=pin1;
    _pin2=_pin2;
}

void WC::connect(char* ssid, char* passwd, int port=80){
   WiFiServer server(port);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid,passwd);
   while(WiFi.status() != WL_CONNECTED){
       delay(500);
       Serial.print("..");
   }
   Serial.println("\n WiFi Connected");


   server.begin();
   Serial.println("Server Connected");
   Serial.print("IP:");
   Serial.println(WiFi.localIP());
}