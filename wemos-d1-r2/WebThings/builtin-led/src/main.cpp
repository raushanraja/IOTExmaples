#define ARDUINOJSON_USE_LONG_LONG 1
#define LARGE_JSON_BUFFERS 1



#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

// TODO: Hardcode your wifi credentials here (and keep it private)
const char *ssid = "dlink_DWR-920V_7EE5";
const char *password = "nhaza95646";
bool lastOn = false;



#if defined(LED_BUILTIN)
const int ledPin = LED_BUILTIN;
#else
const int ledPin = 13; // manually configure LED pin
#endif

WebThingAdapter *adapter;

const char *ledTypes[] = {"OnOffSwitch", "Light", nullptr};
ThingDevice led("led", "Built-in LED", ledTypes);
ThingProperty ledOn("on", "", BOOLEAN, "OnOffProperty");


void setup(void) {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
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

  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledPin, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  digitalWrite(ledPin, HIGH); // active low led

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());






  adapter = new WebThingAdapter("w25", WiFi.localIP());
  led.addProperty(&ledOn);
  adapter->addDevice(&led);
  adapter->begin();




  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(led.id);
}

void loop(void) {
  adapter->update();
  bool on = ledOn.getValue().boolean;
  digitalWrite(ledPin, on ? LOW : HIGH); // active low led
  if (on != lastOn) {
    Serial.print(led.id);
    Serial.print(": ");
    Serial.println(on);
  }
  lastOn = on;
}