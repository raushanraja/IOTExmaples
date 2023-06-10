// Include required libraries
#include <ESP8266WiFi.h>
#include <FastLED.h>

// Rotary Encoder Inputs
#define CLK D1
#define DT D2
#define SW D5

#define NUM_LEDS 2
#define DATA_PIN D7

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
int lastButtonState = 1;



// A method to handle a pair of ws2812b leds in a single pin
// Changs color of the leds with brightness based on the value of the counter
// If counter is 0, leds are off
// if counter value goes beyond 1024, the counter is reset to 0
// Use FastLED library


CRGB leds[NUM_LEDS];

void handleLeds()
{
  // Set the leds to the color based on the counter value
  // If counter is 0, leds are off
  // if counter value goes beyond 1024, the counter is reset to 0
  if (counter > 0 && counter <= 256)
  {
    leds[0] = CRGB(255, 0, 0);
    leds[1] = CRGB(0, 0, 0);
    FastLED.show();
  }
  else if (counter > 256 && counter <= 512)
  {
    leds[0] = CRGB(255, 255, 0);
    leds[1] = CRGB(0, 0, 0);
    FastLED.show();
  }
  else if (counter > 512 && counter <= 768)
  {
    leds[0] = CRGB(0, 255, 0);
    leds[1] = CRGB(0, 0, 0);
    FastLED.show();
  }
  else if (counter > 768 && counter <= 1024)
  {
    leds[0] = CRGB(0, 255, 0);
    leds[1] = CRGB(0, 0, 255);
    FastLED.show();
  }
  else
  {
    counter = 0;
    leds[0] = CRGB(0, 0, 0);
    leds[1] = CRGB(0, 0, 0);
    FastLED.show();
  }
}



void setup()
{

  // Setup FastLED
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);



  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
  lastButtonState = 1;
}


void loop()
{

  // Read the current state of CLK
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

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);

    // Handle leds
    handleLeds();
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
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Handle button relase
  else
  {
    if (lastButtonState == 0)
    {
      Serial.println("Button released!");
      lastButtonState = 1;
    }
  }

  // Put in a slight delay to help debounce the reading
  delay(2);


}