#include "Arduino.h";
#include "WC.h";

WC::WC(int pin1, int pin2){
    pinMode(pin1,INPUT);
    pinMode(pin2,OUTPUT);
    _pin1=pin1;
    _pin2=_pin2;
}

void WC::connect(){
    digitalWrite(_pin2,HIGH);
    delay(250);
    digitalWrite(_pin2,LOW);
    delay(250);
}