#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  Serial.begin(9600);
  // Serial.println("Adafruit AHT10/AHT20 demo!");

  if (!aht.begin())
  {
    // Serial.println("Could not find AHT? Check wiring");
    while (1)
      delay(10);
  }

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  // Serial.println("AHT10 or AHT20 found");
}

void loop()
{
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data

  // write the temperature to lcd
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp.temperature);
  lcd.print((char)223);
  lcd.print("C");

  // write the humidity to lcd
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(humidity.relative_humidity);
  lcd.print("%");
  delay(3500);
}
