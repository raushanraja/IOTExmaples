import os
import wifi
import ssl
import socketpool
import adafruit_minimqtt.adafruit_minimqtt as MQTT
import adafruit_ssd1306 as ssd1306
import board
import bitbangio as io


# print("Loading environment variables...")
WSSID = os.getenv("WSSID")
WPASSWORD = os.getenv("WPASSWORD")
MQTTURI = os.getenv("MQTTURI")
MQTTUSER = os.getenv("MQTTUSER")
MQTTPASSWORD = os.getenv("MQTTPASSWORD")

i2c = io.I2C(board.D5, board.D6)

class OLED:

    def __init__(self):
        self.led = None
        self.current_position = 0



    def setup(self):
         self.display = ssd1306.SSD1306_I2C(128, 64, i2c, addr=0x3C)
    
    def print(self, message):
        self.display.fill(0)
        # self.display.text(message, 0, 0,True, size=1)
        # self.display.show()
        self.message = message
        self.current_position = 0

    def scroll(self):
        if self.current_position > len(self.message)*4:
            self.current_position = 0
    
        self.display.fill(0)
        self.display.text(self.message,  self.current_position, 0,True, size=1)
        self.display.show()
        self.current_position += 4



    def test(self):
        self.display.fill(0)

        self.display.show()

        # Set a pixel in the origin 0,0 position.
        self.display.pixel(0, 0, 1)
        # Set a pixel in the middle 64, 16 position.
        self.display.pixel(64, 16, 1)
        # Set a pixel in the opposite 127, 31 position.
        self.display.pixel(127, 31, 1)
        self.display.show()




print("-------------------------")
# print("WSSID: %s" % WSSID)
# print("WPASSWORD: %s" % WPASSWORD)
# print("MQTTURI: %s" % MQTTURI)
# print("MQTTUSER: %s" % MQTTUSER)
# print("MQTTPASSWORD: %s" % MQTTPASSWORD)


class Network:
    def __init__(self) -> None:
        self.ssl = ssl.create_default_context()

    def connect(self) -> None:
        # print("Connecting to WiFi...")
        wifi.radio.connect(WSSID, WPASSWORD)
        print("Connected! to IP: %s" % wifi.radio.ipv4_address)

    def is_connected(self) -> bool:
        return wifi.radio.connected

    def socket_pool(self) -> socketpool.SocketPool:
        if not self.is_connected():
            self.connect()
        return socketpool.SocketPool(wifi.radio)


class MQTTClient:
    def __init__(self, network: Network, oled: OLED):
        self.network = network
        self.oled = oled
        self.mqtt_client = None

    def connect(self):
        # print("Connecting to MQTT Server")
        self.mqtt_client = MQTT.MQTT(broker=MQTTURI,
                                     username=MQTTUSER,
                                     password=MQTTPASSWORD,
                                     is_ssl=True,
                                     ssl_context=self.network.ssl,
                                     socket_pool=self.network.socket_pool())
        self.mqtt_client.connect()
        self.add_callback()
        print("Connected to MQTT Server")

    def add_callback(self):
        if not self.mqtt_client:
            raise ValueError("MQTT client is not connected. Call connect() first.")
        self.mqtt_client.on_message = self.messge_handler


    def publish(self, topic, message):
        if not self.mqtt_client:
            raise ValueError("MQTT client is not connected. Call connect() first.")
        # print("Publishing to %s" % topic)
        self.mqtt_client.publish(topic, message)

    def subscribe(self, topic):
        if not self.mqtt_client:
            raise ValueError(
                "MQTT client is not connected. Call connect() first.")
        # print("Subscribing to %s" % topic)
        self.mqtt_client.subscribe(topic)
        # print("Subscribed to %s" % topic)

    def on_test(self, client, topic, message):
        print("Subscription Message Received:  %s on %s" % (message, topic))
        self.oled.print(message)


    def messge_handler(self, client, topic, message):
        callback = getattr(self, "on_%s" % topic, None)
        if callback:
            callback(client, topic, message)
        else:
            print("Received %s on %s, no handler attached to topic" % (message, topic))

    def loop(self):
        if not self.mqtt_client:
            self.connect()
        self.mqtt_client.loop()


def main():

    oled = OLED()
    oled.setup()

    network = Network()
    network.connect()
    mqtt_client = MQTTClient(network, oled)
    mqtt_client.connect()
    mqtt_client.subscribe("test")
    mqtt_client.publish("test", "Hello World!")




    while True:
        mqtt_client.loop()
        oled.scroll()

main()
