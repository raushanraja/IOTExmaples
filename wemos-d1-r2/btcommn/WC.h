#ifndef WC_h
#define WC_h
#include "Arduino.h";
#include "ESP8266WiFi.h";
class WC{
    public:
        WC(int pin1, int pin2);
        WiFiServer connect(char* ssid, char* passwd, int port);
        void disconnect();
        void isConnected();
        void sendData();
        void getDat();
    private:
        int _pin1;
        int _pin2;
};
#endif