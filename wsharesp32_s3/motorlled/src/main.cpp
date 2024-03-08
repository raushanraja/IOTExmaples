#include <Arduino.h>
#include <Mot.h>
#include <led.h>

uint32_t brightness = 0; // how bright the LED is
uint32_t fadeAmount = 5; // how many points to fade the LED by

Motor motor1(1);
Motor motor2(2);

int currentMotorOneDirection = FORWARD;
int currentMotorTwoDirection = FORWARD;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;
    Serial.println("Serial Connected");

    ledcAnalogSetup1();
    ledcAnalogSetup2();

    motor1.start();
    motor2.start();
    motor1.setSpeed(250);
    motor2.setSpeed(250);
    LEDSetup();
}

void loop()
{
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        if (c != '\n' && c != '\r')
        {
            Serial.println("Command: " + String(c));
        }
        if (c == 'f')
        {
            LEDGreen();
            motor1.run(FORWARD);
            motor2.run(FORWARD);
        }
        else if (c == 'r')
        {
            motor1.run(FORWARD);
            motor2.run(BACKWARD);
            LEDRed();
        }
        else if (c == 'b')
        {
            motor1.run(BACKWARD);
            motor2.run(BACKWARD);
            LEDBlue();
        }
        else if (c == 'l')
        {
            motor1.run(BACKWARD);
            motor2.run(FORWARD);
            LEDYellow();
        }
        else if (c == 'i')
        {
            motor1.setSpeed(brightness);
            motor2.setSpeed(brightness);
            brightness = brightness + fadeAmount;
            if (brightness <= 0 || brightness >= 255)
            {
                fadeAmount = -fadeAmount;
            }
        }
    }
}
