#### BLE Keyboard
- BLE Keyboard and OLED SSD1306 Basic Example




#### Change inside BLE Library `BLESecurity.cpp`
```cpp

/**
 * Setup for static PIN connection, call it first and then call setAuthenticationMode eventually to change it
 */
void BLESecurity::setStaticPIN(uint32_t pin){
    uint32_t passkey = pin;
    esp_ble_gap_set_security_param(ESP_BLE_SM_SET_STATIC_PASSKEY, &passkey, sizeof(uint32_t));
	setCapability(ESP_IO_CAP_OUT);
	setKeySize();
	setAuthenticationMode(ESP_LE_AUTH_REQ_SC_ONLY);
	setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
}
```


#### Change inside BleKeyboard.cpp
```cpp
void BleKeyboard::begin(void)
{
  BLEDevice::init(deviceName);
  BLEDevice::setEncryptionLevel(ESP_BLE_SEC_ENCRYPT);
  BLEDevice::setSecurityCallbacks(securityCallbacks);


  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(this);

  hid = new BLEHIDDevice(pServer);
  inputKeyboard = hid->inputReport(KEYBOARD_ID);  // <-- input REPORTID from report map
  outputKeyboard = hid->outputReport(KEYBOARD_ID);
  inputMediaKeys = hid->inputReport(MEDIA_KEYS_ID);

  outputKeyboard->setCallbacks(this);

  hid->manufacturer()->setValue(deviceManufacturer);

  hid->pnp(0x02, vid, pid, version);
  hid->hidInfo(0x00, 0x01);


#if defined(USE_NIMBLE)

  BLEDevice::setSecurityAuth(true, true, true);

#else

  BLESecurity* pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_REQ_SC_MITM_BOND);
  pSecurity->setCapability(ESP_IO_CAP_OUT);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  pSecurity->setRespEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);
  pSecurity->setKeySize(16);
  pSecurity->setKeySize(6);


#endif // USE_NIMBLE

  hid->reportMap((uint8_t*)_hidReportDescriptor, sizeof(_hidReportDescriptor));
  hid->startServices();

  onStarted(pServer);

  advertising = pServer->getAdvertising();
//   advertising->setAppearance(HID_KEYBOARD);
  advertising->addServiceUUID(hid->hidService()->getUUID());
  advertising->setScanResponse(false);
  advertising->setMinPreferred(0x0);
  advertising->start();

  ESP_LOGD(LOG_TAG, "Advertising started!");
}
```