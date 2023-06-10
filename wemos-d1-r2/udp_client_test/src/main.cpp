#include <Arduino.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
// #include <WiFiUdp.h>

// // UDP setup
// WiFiUDP Udp;
// unsigned int localUdpPort = 4210;
// char welcomePacket[] = "Hello World!";
// char packetBuffer[255];


// // WiFi setup
// const char* ssid = "TP-Link_9C83";
// const char* password = "g397023045";




// void setup() {
//   Serial.begin(9600);
//   Serial.println("Starting ESP8266...");

//   // WiFi
//   WiFi.begin(ssid, password);
//   Serial.println("Connecting to WiFi...");
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.print("Connected to WiFi! ");
//   Serial.println(WiFi.localIP());

//   // UDP 
//   Udp.begin(localUdpPort);

//   if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
//     Serial.println("Error setting up MDNS responder!");
//   }
// }

// void loop() {
//   int packetSize = Udp.parsePacket();
//   if(packetSize){
//     Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
//     int len = Udp.read(packetBuffer, 255);
//     if(len > 0){
//       packetBuffer[len] = 0;
//     }
//     Serial.printf("UDP packet contents: %s\n", packetBuffer);
//   }
// }

#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("TP-Link_9C83", "g397023045");   // add Wi-Fi networks you want to connect to

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (!MDNS.begin("esp8266")) {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");
}

void loop() { }