#define LED LED_BUILTIN
#include <Arduino.h>

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 10;
int touchInput;

void setup()
{
    pinMode(LED, OUTPUT);
    ledcSetup(ledChannel,freq,resolution);
    ledcAttachPin(LED,ledChannel);
}

void loop()
{
    touchInput = touchRead(T0)*10-5;
    ledcWrite(ledChannel,touchInput);
}