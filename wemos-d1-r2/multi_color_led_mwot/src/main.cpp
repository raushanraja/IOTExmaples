#define ARDUINOJSON_USE_LONG_LONG 1
#define LARGE_JSON_BUFFERS 1

#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char *ssid = "TP-Link_9C82";
const char *password = "38702303";

WebThingAdapter *adapter;

const char *deviceTypes[] = {"OnOffSwitch", nullptr};
ThingDevice device1("switc1", "Led One", deviceTypes);
ThingDevice device2("switch2", "Led Two", deviceTypes);
ThingDevice device3("switch3", "Led Three", deviceTypes);

ThingProperty on1("on1", "On/Off", BOOLEAN, "OnOffProperty");
ThingProperty on2("on2", "On/Off", BOOLEAN, "OnOffProperty");
ThingProperty on3("on3", "On/Off", BOOLEAN, "OnOffProperty");

bool red, green, blue = false;
int redLed = D5;
int blueLed = D6;
int greenLed = D7;

void setup(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
  digitalWrite(D7, LOW);

  Serial.begin(9600);
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  adapter = new WebThingAdapter("leds", WiFi.localIP());
  device1.addProperty(&on1);
  device2.addProperty(&on2);
  device3.addProperty(&on3);
  adapter->addDevice(&device1);
  adapter->addDevice(&device2);
  adapter->addDevice(&device3);
  adapter->begin();

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
}

void loop()
{
  adapter->update();
  bool one = on1.getValue().boolean;
  bool two = on2.getValue().boolean;
  bool three = on3.getValue().boolean;

  digitalWrite(redLed, one ? LOW : HIGH);
  digitalWrite(greenLed, two ? LOW : HIGH);
  digitalWrite(blueLed, three ? LOW : HIGH);
  
  red = one;
  green = two;
  blue = three;
}