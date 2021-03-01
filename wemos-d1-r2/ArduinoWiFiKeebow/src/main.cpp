#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <wifisetup.h>
#include <ArduinoJson.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>
#include <Hash.h>


SocketIOclient socketIO;
const int pavu = D5;
const int discord = D6;
int LED_STATE=HIGH;
// #define USE_SERIAL Serial

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
        switch(type) {
        case sIOtype_DISCONNECT:
            // USE_SERIAL.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            // USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);
            break;
        case sIOtype_EVENT:
            // USE_SERIAL.printf("[IOc] get event: %s\n", payload);
            break;
        case sIOtype_ACK:
            // USE_SERIAL.printf("[IOc] get ack: %u\n", length);
            // hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            // USE_SERIAL.printf("[IOc] get error: %u\n", length);
            // hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            // USE_SERIAL.printf("[IOc] get binary: %u\n", length);
            // hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            // USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
            // hexdump(payload, length);
            break;
    }
}

void setup() {
    // // USE_SERIAL.begin(921600);

    pinMode(pavu,INPUT_PULLUP);
    pinMode(discord,INPUT_PULLUP);
    // USE_SERIAL.begin(9600);

    //Serial.setDebugOutput(true);
    // USE_SERIAL.setDebugOutput(true);

    // USE_SERIAL.println();
    // USE_SERIAL.println();
    // USE_SERIAL.println();

      for(uint8_t t = 4; t > 0; t--) {
          // USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\r", t);
          // USE_SERIAL.flush();
          delay(1000);
      }

    startWiFi();

    // server address, port and URL
    socketIO.begin("192.168.0.175", 3000);

    // event handler
    socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;
int prev = 2;
int prevDiscord = 2;
void loop() {
    socketIO.loop();
    int bs = digitalRead(pavu);
    int ds = digitalRead(discord);
    if(bs != prev) prev = 2;
    if(ds !=prevDiscord) prevDiscord = 2;
    if(bs == LOW && prev != bs) {     
        prev = bs;
        socketIO.sendEVENT("[\"message\",{\"a\":1,\"d\":0}]");
        delay(300);
    }

    if(ds == LOW &&  prevDiscord !=ds){
        prevDiscord = ds;
        socketIO.sendEVENT("[\"message\",{\"a\":0,\"d\":1}]");
        delay(300);
    }
}

