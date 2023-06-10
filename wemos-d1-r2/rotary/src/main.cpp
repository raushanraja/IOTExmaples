#include <Arduino.h>
#include <SocketIOclient.h>
#include <Hash.h>
#include <wifisetup.h>
// Rotary Encoder Inputs
#define CLK D1
#define DT D2
#define SW D5

// Rotary Encoder Vairalbes
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
int lastButtonState = 1;

int LED_STATE = HIGH;

void setup()
{
    Serial.begin(9600);
    for (uint8_t t = 8; t > 0; t--)
    {
        Serial.printf("[SETUP] BOOT WAIT %d...\r", t);
        Serial.flush();
        delay(1000);
    }

    Serial.println("Rotary Encoder Test:");
    Serial.flush();
    // Set encoder pins as inputs
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);

    // Read the initial state of CLK
    lastStateCLK = digitalRead(CLK);
    lastButtonState = 1;
}

void loop()
{

    currentStateCLK = digitalRead(CLK);

    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {

        // If the DT state is different than the CLK state then
        // the encoder is rotating CCW so decrement
        if (digitalRead(DT) != currentStateCLK)
        {
            counter--;
            currentDir = "CCW";
        }
        else
        {
            // Encoder is rotating CW so increment
            counter++;
            currentDir = "CW";
        }

        // Serial.print("Counter: ");
        // Serial.println(counter);
        // Print counter in smae line
        // Check the count remains within range 0 to 100
        if (counter < 0)
        {
            counter = 0;
        }
        else if (counter > 100)
        {
            counter = 100;
        }

        Serial.println("Counter: " + String(counter));
    }

    // Remember last CLK state
    lastStateCLK = currentStateCLK;

    delay(2);
}
