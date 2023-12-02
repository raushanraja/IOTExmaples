#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <HardwareSerial.h>
#include <PubSubClient.h>

HardwareSerial ATSerial(1);

const char *ssid = "TP-Link_9C83";
const char *password = "g397023045";
const char *mqtt_server = "192.168.68.61";
const char *mqtt_username = "raushanraja";
const char *mqtt_password = "hJsQ:6Dt>&.R5sE";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

const char *sensor1_topic = "input";
const char *sensor2_topic = "output";

void reconnect()
{
  // Loop until we’re reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection…");
    String clientId = "esp32c3"; // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");

      client.subscribe("input"); // subscribe the topics here
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds"); // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//=======================================
// This void is called every time we have a message from the broker

void callback(char *topic, byte *payload, unsigned int length)
{
  String incommingMessage = "";
  for (int i = 0; i < length; i++)
    incommingMessage += (char)payload[i];
  Serial.println("Message arrived [" + String(topic) + "]" + incommingMessage);

  if (incommingMessage.equalsIgnoreCase("EOSS"))
  {
    Serial.println("End of string");
    ATSerial.write(26);
  }
  else
  {
    incommingMessage.concat("\r\n");
    ATSerial.print(incommingMessage);
  }
}

//======================================= publising as string
void publishMessage(const char *topic, String payload, boolean retained)
{
  // check if payload is not empty after stripping spaces
  payload.trim();
  if (payload.length() > 0)
  {

    if (client.publish(topic, (char *)payload.c_str(), retained))
    {
      Serial.println("Publish ok");
    }
    else
    {
      Serial.println("Publish failed");
    }
  }
}

void setup()
{

  Serial.begin(9600);
  ATSerial.begin(115200, SERIAL_8N1, D6, D7);

  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());

  // while (!Serial)
  //   delay(1);

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop()
{

  if (!client.connected())
    reconnect();
  client.loop();
  while (ATSerial.available())
  {
    String incommingMessage = "";
    incommingMessage = ATSerial.readString();
    incommingMessage.trim();
    Serial.println(incommingMessage);
    publishMessage(sensor2_topic, incommingMessage, true);
  }
}
