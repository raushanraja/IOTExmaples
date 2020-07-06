#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

const char *ssid = "dlink_DWR-920V_7EE5";
const char *password = "nhaza95645";
const int hostPort = 80;
const char *hostAddress = "192.168.0.24";

WiFiClient wifi;
HttpClient client = HttpClient(wifi, hostAddress, hostPort);

int touchInput;
int value=0;
void setup()
{
  Serial.begin(9600);
  delay(100);
  Serial.print("connection to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi Connected:");
  Serial.println(WiFi.localIP());
  Serial.println("Starting Communication with WebServer");
}

void loop()
{
  touchInput = touchRead(T0);
  String on = "{\"on\":true}";
  String off = "{\"on\":false}";

  Serial.println(touchInput);
  if (touchInput < 15 && touchInput > 10)
  {
    String contentType = "text/plain";
    Serial.println("making PUT request");

    client.put("/things/switc/properties/on", contentType, value==0?on:off);
    value=value==0?1:0;
    client.responseBody();
    Serial.println();
    Serial.println("closing connection");
    delay(1);
  }

  Serial.flush();
}
