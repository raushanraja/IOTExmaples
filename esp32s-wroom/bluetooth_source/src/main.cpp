#include <Arduino.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
  uint8_t address[6]  = {0xAA, 0xBB, 0xCC, 0x11, 0x22, 0x33}; // Change this to reflect real MAC address of your slave BT device

void setup() {
  Serial.begin(9600);
  delay(5000);
  Serial.println("Starting..");
  SerialBT.begin("MESP", true); //Bluetooth device name
  SerialBT.connect("realme Buds Wireless 2");
  SerialBT.confirmReply(true);
  SerialBT.write("Hello World");
}

void loop() {
  delay(20);
}