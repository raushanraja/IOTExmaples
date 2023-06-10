// /*
//  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
//  -----
//  Author: Ayush Sharma ( https://github.com/ayushsharma82 )

//  Important Notice: Star the repository on Github if you like the library! :)
//  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA
// */

// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>

// #include <ESPAsyncWebServer.h>
// #include <AsyncElegantOTA.h>

// IPAddress staticIP(192, 168, 10, 172); // ESP static ip
// IPAddress gateway(192, 168, 10, 1);    // IP Address of your WiFi Router (Gateway)
// IPAddress subnet(255, 255, 255, 0);    // Subnet mask
// IPAddress dns(8, 8, 8, 8);             // DNS

// const char *ssid = "TP-Link_9C82";
// const char *password = "38702303";

// AsyncWebServer server(80);

// //______________________ Switch _______________________________//
// // set pin numbers
// const int buttonPin = 4; // GPIO4
// const int ledPin = LED_BUILTIN;

// // variable for storing the pushbutton status
// int buttonState = 0;
// int ledState = 0;
// //______________________ Switch _______________________________//

// void setup(void)
// {
//   Serial.begin(9600);

//   //______________________ Switch _______________________________//

//   // initialize the pushbutton pin as an input
//   pinMode(buttonPin, INPUT);
//   // initialize the LED pin as an output
//   pinMode(ledPin, OUTPUT);
//   //______________________ Switch _______________________________//

//   // Configure a StaticIP Address
//   if (!WiFi.config(staticIP, gateway, subnet, dns))
//   {
//     Serial.println("STA Failed to configure");
//   }

//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, password);
//   Serial.println("");
//   // Wait for connection
//   while (WiFi.status() != WL_CONNECTED)
//   {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.print("Connected to ");
//   Serial.println(ssid);
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());

//   AsyncElegantOTA.begin(&server, "raushan", "rau@123"); // Start AsyncElegantOTA
//   server.begin();
//   Serial.println("HTTP server started");
// }

// void loop(void)
// {

//   //______________________ Switch _______________________________//
//   delay(1000);
//   // read the state of the pushbutton value
//   int new_button_state = digitalRead(buttonPin);
//   // check if the pushbutton is pressed.
//   // if it is, the buttonState is HIGH
//   if (buttonState != new_button_state)
//   {
//     ledState = !ledState;
//     buttonState = !buttonState;
//     // turn LED on
//     digitalWrite(ledPin, ledState);
//   }
//   //______________________ Switch _______________________________//
// }