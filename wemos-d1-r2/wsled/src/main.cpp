// Use if you want to force the software SPI subsystem to be used for some
// reason (generally, you don't) #define FASTLED_FORCE_SOFTWARE_SPI Use if you
// want to force non-accelerated pin access (hint: you really don't, it breaks
// lots of things) #define FASTLED_FORCE_SOFTWARE_SPI #define
// FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

///////////////////////////////////////////////////////////////////////////////////////////
//
// Move a white dot along the strip of leds.  This program simply shows how to
// configure the leds, and then how to turn a single pixel white and then off,
// moving down the line of pixels.
//

// How many leds are in the strip?
#define NUM_LEDS 2

// For led chips like WS2812, which have a data line, ground, and power, you
// just need to define DATA_PIN.  For led chipsets that are SPI based (four
// wires - data, clock, ground, and power), like the LPD8806 define both
// DATA_PIN and CLOCK_PIN Clock pin only needed for SPI based chipsets when not
// using hardware SPI
#define DATA_PIN D4
#define DOOR D5
#define RELAY D6

// #define CLOCK_PIN 13

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// This function sets up the ledsand tells the controller about them
void setup()
{
  Serial.begin(9600);
  Serial.println("Starting");

  // sanity check delay - allows reprogramming if accidently blowing power
  // w/leds
  delay(2000);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(DOOR, INPUT_PULLUP);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); // GRB
}

// This function runs over and over, and is where you do the magic to light
// your leds.
void led_on()
{
  // Turn the first led red for 1 second
  leds[0] = CRGB::Red;
  leds[1] = CRGB::Red;
  FastLED.show();
  delay(100);
}

void led_off()
{
  leds[0] = CRGB::Black;
  leds[1] = CRGB::Black;
  FastLED.show();
  delay(100);
}

int state;

void loop()
{
  // digitalWrite(RELAY, HIGH);

  Serial.println("Loop");
  state = digitalRead(DOOR);
  if (state == LOW)
  {
    Serial.println("State LOW");
    digitalWrite(RELAY, LOW);
    led_off();
  }
  else
  {
    Serial.println("state HIGH");
    digitalWrite(RELAY, HIGH);
    led_on();
  }

  delay(500);
  // digitalWrite(RELAY, LOW);
  // delay(500);

  // Turn the first led red for 1 second
}
