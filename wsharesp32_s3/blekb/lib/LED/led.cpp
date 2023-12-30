#include <led.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>




#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


OLED::OLED() {
    Wire.begin(GPIO_NUM_6, GPIO_NUM_5);
    this->led = Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
}

void OLED::init() {
    if(!this->led.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

void OLED::clear() {
    this->led.clearDisplay();
}

void OLED::display(String text) {
    this->led.clearDisplay();
    this->led.setTextSize(2);
    this->led.setTextColor(SSD1306_WHITE);
    this->led.setCursor(0, 0);
    this->led.print(text);
    this->led.display();
}