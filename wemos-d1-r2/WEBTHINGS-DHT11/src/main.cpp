#define ARDUINOJSON_USE_LONG_LONG 1
#define LARGE_JSON_BUFFERS 1
#define DHTPIN D6
#define RELAYPIN D7
#define DHTTYPE DHT11

#include <Arduino.h>
#include <DHT.h>
#include <TaskScheduler.h>
#include "Thing.h"
#include "WebThingAdapter.h"

const char *ssid = "wifi";
const char *password = "password";
bool ledCurrentState = false;


WebThingAdapter *adapter;
DHT dht(DHTPIN, DHTTYPE);

const char *dhtTypes[] = {nullptr};
const char *deviceTypes[] = {"OnOffSwitch", nullptr};

ThingDevice device("switc", "Led One", deviceTypes);
ThingProperty on("on", "On/Off", BOOLEAN, "OnOffProperty");

ThingDevice indoorWeahter("dht11", "Temperature & Humidity", dhtTypes);

ThingProperty dhtTemp("tempC", "", NUMBER, nullptr);
ThingProperty dhtHumid("humidity", "", NUMBER, nullptr);


void readDHT11()
{
  Serial.print("DHT Data Reading.....\n");
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Error reading data");
    return;
  }
  Serial.print(h);
  Serial.println(t);
  ThingPropertyValue value;

  value.number = t;
  dhtTemp.setValue(value);

  value.number = h;
  dhtHumid.setValue(value);
}

// Task Scheduling
Task t1(5000, TASK_FOREVER, &readDHT11);
Scheduler runner;

void setup(void)
{
  pinMode(D7, OUTPUT);
  Serial.begin(9600);

  // Initiating Runner & add task to it
  runner.init();
  runner.addTask(t1);

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

  dht.begin();

  adapter = new WebThingAdapter("leds", WiFi.localIP());
  indoorWeahter.addProperty(&dhtTemp);
  indoorWeahter.addProperty(&dhtHumid);
  adapter->addDevice(&indoorWeahter);

  device.addProperty(&on);
  adapter->addDevice(&device);

  adapter->begin();

  t1.enable();
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
  runner.execute();
  adapter->update();
  bool one = on.getValue().boolean;
  digitalWrite(REPLAYPIN, one ? LOW : HIGH);
  ledCurrentState = one;
}