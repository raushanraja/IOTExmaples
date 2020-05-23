#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <wifisetup.h>

WebServer server(80);

void handleRoot()
{

  server.send(200, "text/plain", "hello from esp8266!");
}

void handleNotFound()
{

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleTemp()
{
  String temperature = "20";
  String weahter = "\"Cold\"";
  String response = String("{\"Data\":" + temperature + ",\"Condition\":" + weahter + "}");
  server.send(200, "application/json", response);
}

void setup()
{
  WiFi.disconnect();
  Serial.begin(9600);
  delay(1000);
  Serial.println("Starting WiFi Server");
  startWiFi();
  if (MDNS.begin("esp32"))
  {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/temp", handleTemp);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}