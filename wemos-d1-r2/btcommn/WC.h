#ifndef WC_h
#define WC_h
#include "Arduino.h";
class WC{
    public:
        WC(int pin1, int pin2);
        void connect();
        void disconnect();
        void isConnected();
        void sendData();
        void getDat();
    private:
        int _pin1;
        int _pin2;
};
#endif