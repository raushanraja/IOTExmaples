#include <Arduino.h>
#include <SocketIOclient.h>
#include <Hash.h>
#include <wifisetup.h>
// Rotary Encoder Inputs
#define CLK D1
#define DT D2
#define SW D5
#define HOST "0.0.0.0"

// Rotary Encoder Vairalbes
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
int lastButtonState = 1;

SocketIOclient socketIO;
const int pavu = D5;
const int discord = D6;
int LED_STATE = HIGH;
#define USE_SERIAL Serial

void socketIOEvent(socketIOmessageType_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case sIOtype_DISCONNECT:
        USE_SERIAL.printf("[IOc] Disconnected!\n");
        break;
    case sIOtype_CONNECT:
        USE_SERIAL.printf("[IOc] Connected to url: %s\n", payload);

        // join default namespace (no auto join in Socket.IO V3)
        socketIO.send(sIOtype_CONNECT, "/");
        break;
    case sIOtype_EVENT:
        USE_SERIAL.printf("[IOc] get event: %s\n", payload);
        hexdump(payload, length);
        break;
    case sIOtype_ACK:
        USE_SERIAL.printf("[IOc] get ack: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_ERROR:
        USE_SERIAL.printf("[IOc] get error: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_BINARY_EVENT:
        USE_SERIAL.printf("[IOc] get binary: %u\n", length);
        hexdump(payload, length);
        break;
    case sIOtype_BINARY_ACK:
        USE_SERIAL.printf("[IOc] get binary ack: %u\n", length);
        hexdump(payload, length);
        break;
    }
}

void setup()
{
    // USE_SERIAL.begin(9600);

    // Set encoder pins as inputs
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);

    USE_SERIAL.begin(9600);

    Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(true);

    // Read the initial state of CLK
    lastStateCLK = digitalRead(CLK);
    lastButtonState = 1;

    // USE_SERIAL.println();
    // USE_SERIAL.println();
    // USE_SERIAL.println();

    for (uint8_t t = 4; t > 0; t--)
    {
        USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\r", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    startWiFi();

    // server address, port and URL
    socketIO.begin(HOST, 5000, "/socket.io/?EIO=4");

    // event handler
    socketIO.onEvent(socketIOEvent);
}

unsigned long messageTimestamp = 0;
int prev = 2;
int prevDiscord = 2;
void loop()
{
    socketIO.loop();

    currentStateCLK = digitalRead(CLK);

    // If last and current state of CLK are different, then pulse occurred
    // React to only 1 state change to avoid double count
    if (currentStateCLK != lastStateCLK && currentStateCLK == 1)
    {

        // If the DT state is different than the CLK state then
        // the encoder is rotating CCW so decrement
        if (digitalRead(DT) != currentStateCLK)
        {
            counter--;
            currentDir = "CCW";
            String message = "[\"message\",{\"dir\":\"CCW\", \"counter\":" + String(counter) + "}]";
            socketIO.sendEVENT(message);
        }
        else
        {
            // Encoder is rotating CW so increment
            counter++;
            currentDir = "CW";
            String message = "[\"message\",{\"dir\":\"CW\", \"counter\":" + String(counter) + "}]";
            socketIO.sendEVENT(message);
        }

        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
    }

    // Remember last CLK state
    lastStateCLK = currentStateCLK;

    // Read the button state
    int btnState = digitalRead(SW);

    // If we detect LOW signal, button is pressed
    if (btnState == LOW)
    {
        lastButtonState = 0;
        // if 50ms have passed since last LOW pulse, it means that the
        // button has been pressed, released and pressed again
        if (millis() - lastButtonPress > 50)
        {
            Serial.println("Button pressed!");
            String message = "[\"message\",{\"dir\":\"Button\", \"counter\": \"1\"}]";
            socketIO.sendEVENT(message);
        }

        // Remember last button press event
        lastButtonPress = millis();
    }

    // Handle button relase
    else
    {
        if (lastButtonState == 0)
        {
            Serial.println("Button released!");
            lastButtonState = 1;
            String message = "[\"message\",{\"dir\":\"Button\", \"counter\": \"0\"}]";
            socketIO.sendEVENT(message);
        }
    }
    delay(2);
}