/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);
  
  emon1.current(34, 15.80);             // Current: input pin, calibration.

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(20);
  display.setTextColor(WHITE);

}

void loop() {
  
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println(Irms);
  Serial.println(Irms);
  display.display();                  // Display on serial monitor
  delay(1000);

}
