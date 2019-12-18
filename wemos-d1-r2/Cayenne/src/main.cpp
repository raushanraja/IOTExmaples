//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "JioFi2_AB7347";
char wifiPassword[] = "tchcrpcf3x";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "7aa69e20-210e-11ea-8221-599f77add412";
char password[] = "4aee20f0053fdb7a5b42200c15e0fb4d7c3fe5f9";
char clientID[] = "a30c5630-2126-11ea-b73d-1be39589c6b2";


void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Cayenne.loop();
	delay(2000);
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(2, 100);
	// Some examples of other functions you can use to send data.
	Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
