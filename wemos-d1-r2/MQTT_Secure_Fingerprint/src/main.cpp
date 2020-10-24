#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

const char *ssid = "Android_84";
const char *password = "1234467890";
const char *mqtt_server = "raushanraja.site";
const int port = 8883;             // REPLCAE WITH YOUR PORT
const char *device_id = "esp8266"; // CAN REPLACE
const char *matt_client_username = "raushanread";
const char *matt_client_password = "raushan";

// SHA fingerprint of certificate
// Change If required as per certificate
// Get from browser or use below command on server
// openssl x509 -noout -in mqtt-serv.crt -fingerprint
// do replace `mqtt-serv.crt` with your certificate name
const char fingerprint[] PROGMEM = "BB D4 AD 97 AD 4E 64 0B A2 62 83 A3 83 6F 54 1D EB 13 E9 C4"; // REPLCAE

WiFiClientSecure espClient; // Using Secure Client
PubSubClient client(espClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
String reply = "";
int value = 0;
void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1')
  {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
    client.publish("test/reply", "LED ON");

    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  }
  else if ((char)payload[0] == '0')
  {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
    client.publish("test/reply", "LED OFF");
  }
  else
  {
    value = digitalRead(LED_BUILTIN);
    if (value == HIGH)
    {
      client.publish("test/reply", "LED OFF");
    }
    else
    {
      client.publish("test/reply", "LED ON");
    }
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {

    // Create a secure Connection again
    // Before we connect to MQTT Server
    if (!espClient.connect(mqtt_server, port))
    {
      Serial.println("connection failed");
      return;
    }

    //
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), matt_client_username, matt_client_password))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      //   client.publish("test/iot", "hello world");
      client.publish("test/reply", "LED OFF");
      // ... and resubscribe
      client.subscribe("test/iot");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  Serial.begin(9600);
  setup_wifi();
  Serial.print("connecting to ");
  Serial.println(mqtt_server);

  Serial.printf("Using fingerprint '%s'\n", fingerprint);
  espClient.setFingerprint(fingerprint);
  if (!espClient.connect(mqtt_server, port))
  {
    Serial.println("connection failed");
    return;
  }
  client.setServer(mqtt_server, port);
  client.setCallback(callback);
  client.publish("test/reply", "LED OFF");
  digitalWrite(LED_BUILTIN, 1);
}

void loop()
{

  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    ++value;
    // snprintf (msg, MSG_BUFFER_SIZE, "hello world #%ld", value);
    // Serial.print("Publish message: ");
    // Serial.println(msg);
    // client.publish("test/reply", "msg");
  }
}
