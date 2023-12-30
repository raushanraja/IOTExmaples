#include <Arduino.h>
#include <BleKeyboard.h>
#include <led.h>

#define PASSKEY 999999

OLED oled;

bool isAuthComplete = false;
bool opened = false;
int counter = 0;
int currentKey;

class MyBLESecurityCallbacks : public BLESecurityCallbacks
{
  uint32_t onPassKeyRequest()
  {
    return PASSKEY;
  }

  void onPassKeyNotify(uint32_t pass_key)
  {
    Serial.println("Passkey received: " + String(pass_key));
    oled.display("Passkey received :\n" + String(pass_key));
    currentKey = pass_key;
  }

  bool onConfirmPIN(uint32_t pin)
  {
    // Here, you can confirm the PIN received during the pairing process.
    // You can compare this PIN with an expected value and return true if they match.
    Serial.println("Confirming PIN: " + String(pin));
    oled.display(String(pin));
    return true;
  }

  bool onSecurityRequest()
  {
    Serial.println("Security request received!");
    return true;
  }

  void onAuthenticationComplete(esp_ble_auth_cmpl_t auth_cmpl)
  {
    // Here, you can handle actions after the authentication process is completed.
    // For example:

    if (auth_cmpl.success)
    {
      Serial.println("Authenticated!");
      oled.display("Authenticated!");
      isAuthComplete = true;
      currentKey = 0;
    }
    else
    {
      Serial.println("Authentication failed!");
      oled.display("Authentication failed!");
      currentKey = 0;
    }
  }
};

class MyBLEServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("Client connected!");
  }

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("Client disconnected!");
  }
};

// Instantiate your callback class
MyBLESecurityCallbacks mySecurityCallbacks;
BleKeyboard bleKeyboard(&mySecurityCallbacks);
void setupOLED()
{
  oled = OLED();
  Serial.println("Starting OLED!");
  oled.init();
  oled.display("Hello World!");
}

void setupKeyb()
{
  bleKeyboard.begin();
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  setupOLED();
  setupKeyb();
}

void loop()
{
  delay(5000);
  oled.display(String(counter++));
  if (isAuthComplete && bleKeyboard.isConnected())
  {
    oled.display("Connected!");
    if (!opened)
    {
      bleKeyboard.press(KEY_LEFT_GUI);
      bleKeyboard.press('d');
      bleKeyboard.releaseAll();
      delay(500);
      bleKeyboard.print("code");
      bleKeyboard.press(KEY_RETURN);
      bleKeyboard.releaseAll();
      opened = true;
    }
  }
  else
  {
    String key =  currentKey == 0 ? "" : String(currentKey);
    oled.display("Not connected! " + key);
    if (isAuthComplete)
    {
      isAuthComplete = false;
    }
  }
}