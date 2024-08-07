#include <Arduino.h>

#define RGB_BRIGHTNESS 10 // Change white brightness (max 255)

// the setup function runs once when you press reset or power the board
#ifdef RGB_BUILTIN
#undef RGB_BUILTIN
#endif
#define RGB_BUILTIN 21

void setup()
{
  // neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
  // No need to initialize the RGB LED
}

// the loop function runs over and over again forever
void loop()
{
#ifdef RGB_BUILTIN
  // digitalWrite(RGB_BUILTIN, HIGH);   // Turn the RGB LED white
  neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, RGB_BRIGHTNESS, RGB_BRIGHTNESS); // Red
  delay(1000);
  // digitalWrite(RGB_BUILTIN, LOW);    // Turn the RGB LED off
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Red
  delay(1000);

  neopixelWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0); // Red
  delay(1000);
  neopixelWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0); // Green
  delay(1000);
  neopixelWrite(RGB_BUILTIN, 0, 0, RGB_BRIGHTNESS); // Blue
  delay(1000);
  neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Off / black
  delay(1000);
#endif
}
