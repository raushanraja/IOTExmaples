#define ARDUINOJSON_USE_LONG_LONG 1
#define LARGE_JSON_BUFFERS 1

#define RELAY_0 D1
#define RELAY_1 D2



#include <Arduino.h>
#include <TaskScheduler.h>
#include "Thing.h"
#include "WebThingAdapter.h"
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
#include <EEPROM.h>

const char *ssid = "TP-Link_ECAE";
const char *password = "96559383";

WebThingAdapter *adapter;
AsyncWebServer server(80);

const char *dhtTypes[] = {nullptr};
const char *deviceTypes[] = {"OnOffSwitch", nullptr};

// Defining device to control LED and then adding it's property
ThingDevice device("switch", "Light One", deviceTypes);
ThingProperty on("on", "On/Off", BOOLEAN, "OnOffProperty");
ThingProperty ton("ton", "On/Off", BOOLEAN, "OnOffProperty");
ThingProperty update("update", "On/Off", BOOLEAN, "OnOffProperty");

uint32_t delayMS;
bool firstIsOn = false;
bool secondIsOn = false;
bool updateIsOn = false;

int flagAddress = 0; // EEPROM address to store the flag
byte flagValue = 0;  // Variable to hold the flag value

void setFlag(byte value)
{
  EEPROM.write(flagAddress, value);
  EEPROM.commit(); // Save the changes to EEPROM
}

byte getFlag()
{
  return EEPROM.read(flagAddress);
}

void clearFlag()
{
  setFlag(0);
}

void updateSetup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", "Hi! I am ESP8266."); });

  AsyncElegantOTA.begin(&server, "IddLAi_adfVBA1SD#DF", "1aa2dKDFW#%&sHVBdd");
  server.begin();
  Serial.println("HTTP server started");
  clearFlag();
}

void webthingSetup()
{


  pinMode(RELAY_0, OUTPUT);
  pinMode(RELAY_1, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Set relay to HIGH because it is active low
  digitalWrite(RELAY_0, HIGH);
  digitalWrite(RELAY_1, HIGH);

  delayMS = 2000;

  // Initiating Runner & add task to it
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

  device.addProperty(&on);
  device.addProperty(&ton);
  device.addProperty(&update);
  adapter->addDevice(&device);

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
void setup(void)
{
  Serial.begin(9600);

  EEPROM.begin(1);
  flagValue = getFlag();

  Serial.printf("flagValue: %d\n", flagValue);

  if (flagValue == 1)
  {
    updateSetup();
    updateIsOn = true;
  }
  else
  {
    webthingSetup();
  }
}

void loop()
{
  if (!updateIsOn)

  {
    adapter->update();
    bool oN = on.getValue().boolean;
    bool toN = ton.getValue().boolean;
    bool flag = update.getValue().boolean;

    // Check if it is the previous state
    if (oN != firstIsOn)
    {
      // If not, then update the state
      firstIsOn = oN;
      // And turn on/off the relay
      digitalWrite(RELAY_0, oN ? LOW : HIGH);
    }

    if (toN != secondIsOn)
    {
      // If not, then update the state
      secondIsOn = toN;
      // And turn on/off the relay
      digitalWrite(RELAY_1, toN ? LOW : HIGH);
    }

    if (flag)
    {
      setFlag(1);
    }
  }
}
