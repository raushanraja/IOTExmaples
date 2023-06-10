#define LED LED_BUILTIN
#include <Arduino.h>

#define CLK 22
#define DT 23
#define SW 21

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

        Serial.print("Counter: " + String(counter) + "\n");
    }

    // Remember last CLK state
    lastStateCLK = currentStateCLK;

        // Read the button state
    int btnState = digitalRead(SW);

    // If we detect LOW signal, button is pressed
    if (btnState == LOW)
    {
        lastButtonState = 0;
        // if 50ms have passed since last LOW pulse, it means that the
        // button has been pressed, released and pressed again
        if (millis() - lastButtonPress > 50)
        {
            // Serial.println("Button pressed!");
        }

        // Remember last button press event
        lastButtonPress = millis();
    }

    // Handle button relase
    else
    {
        if (lastButtonState == 0)
        {
            // Serial.println("Button released!");
            Serial.println("value: 1234");

            lastButtonState = 1;
        }
    }
    // Serial.print("Button State: " + String(btnState) + "\n");
    delay(1);
}
