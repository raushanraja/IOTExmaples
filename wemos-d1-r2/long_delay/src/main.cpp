#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "192.168.1.175:3000"

#ifndef STASSID
#define STASSID "TP-Link_9C82"
#define STAPSK "38702303"
#endif

const int relay = LED_BUILTIN;
int count = 0;
int delay_ms = 1000 * 60 * 60;
String data = "";

void send_post(WiFiClient client, String data)
{
  HTTPClient http;

  Serial.println("Connecting to server...");
  http.begin(client, "http://" SERVER_IP "/");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Accept", "application/json");
  int httpCode = http.POST(data);
  Serial.println(httpCode);

  if (httpCode > 0)
  {
    String payload = http.getString();
    Serial.println(payload);
  }
  else
  {
    Serial.println("Error on sending POST");
  }
  http.end();
}

void setup()
{
  Serial.begin(9200);
  Serial.println("Starting with setup");
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);
  WiFi.begin(STASSID, STAPSK);

  Serial.println('\n Waiting to Connect to WIFI\n');

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println('Connected to WIFI');
  Serial.println("\n Main loop will start after 5000 seconds\n");
  Serial.println("ON/OFF First Time");
  digitalWrite(relay, LOW);
  Serial.println("Current Flowing");
  delay(1000 * 60);
  digitalWrite(relay, HIGH);
  Serial.println("Current Not Flowing");
  Serial.println("\n Main loop will start after 5000 seconds\n");
  delay(5000);
}

void loop()
{
  if ((WiFi.status() == WL_CONNECTED))
  {
    if (count < 9)
    {
      count++;
      data = "{\"msg\":\"Sending Hourly data, count:" + String(count) + "\" , \"status\":\"OFF\"}";
    }
    else
    {
      count = 0;
      digitalWrite(relay, LOW);
      data = "{\"msg\":\"Sending 10Th Hour, last data sent\", \"status\":\"ON\"}";
      delay(1000 * 60);
      digitalWrite(relay, HIGH);
    }
    WiFiClient client;
    send_post(client, data);
  }
  else
  {
    Serial.println("Not Connected to WIFI");
  }
  delay(delay_ms);
}