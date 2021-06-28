#include <Arduino.h>
#include <wifisetup.h>
#include <WebSocketsClient.h>
#include <SocketIOclient.h>


SocketIOclient socketIO;
const int pir_pin = 23;
int LED_STATE=HIGH;

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
        switch(type) {
        case sIOtype_DISCONNECT:
            Serial.printf("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            Serial.printf("[IOc] Connected to url: %s\n", payload);
            break;
        case sIOtype_EVENT:
            Serial.printf("[IOc] get event: %s\n", payload);
            break;
        case sIOtype_ACK:
            Serial.printf("[IOc] get ack: %u\n", length);
            // hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            Serial.printf("[IOc] get error: %u\n", length);
            // hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            Serial.printf("[IOc] get binary: %u\n", length);
            // hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            Serial.printf("[IOc] get binary ack: %u\n", length);
            // hexdump(payload, length);
            break;
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(pir_pin,INPUT_PULLDOWN);
    Serial.setDebugOutput(true);
    Serial.setDebugOutput(true);

    Serial.println();
    Serial.println();
    Serial.println();

      for(uint8_t t = 4; t > 0; t--) {
          Serial.printf("[SETUP] BOOT WAIT %d...\r", t);
          Serial.flush();
          delay(1000);
      }

    startWiFi();

    // server address, port and URL
    socketIO.begin("192.168.0.175", 3001);

    // event handler
    socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;
int prev = 2;
void loop() {
    socketIO.loop();
    int bs = digitalRead(pir_pin);
    if(bs==LOW) Serial.println(bs);
    if(bs != prev) prev = 2;
    if(bs == HIGH && prev != bs) {  
        Serial.println(bs);
        prev = bs;
        socketIO.sendEVENT("[\"message\",{\"a\":1}]");
        delay(3000);
    }
}
