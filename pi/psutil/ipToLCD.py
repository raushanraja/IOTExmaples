# Printing IP of the WIFI Adapter to LCD using
# psutil library https://github.com/giampaolo/psutil   and
# Adafruit_CircuitPython_CharLCD librabry https://github.com/adafruit/Adafruit_CircuitPython_CharLCD  

import board
import digitalio
import adafruit_character_lcd
import adafruit_character_lcd.character_lcd as characterlcd
from time import sleep
import psutil as ps


# Character LCD Config:
# Modify this if you have a different sized character LCD
lcd_columns = 16
lcd_rows = 2

# Raspberry Pi Pin Config:
lcd_rs = digitalio.DigitalInOut(board.D25)
lcd_en = digitalio.DigitalInOut(board.D24)
lcd_d7 = digitalio.DigitalInOut(board.D22)
lcd_d6 = digitalio.DigitalInOut(board.D18)
lcd_d5 = digitalio.DigitalInOut(board.D17)
lcd_d4 = digitalio.DigitalInOut(board.D23)
lcd_backlight = digitalio.DigitalInOut(board.D4)

# Init the lcd class
lcd =characterlcd.Character_LCD_Mono(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6,lcd_d7, lcd_columns, lcd_rows, lcd_backlight)


#Get wlan0 IP 
interfaces= ps.net_if_addrs()
reqInteface='wlan0'                          # change this to get required interafce's IP ex. reqInteface='lo', reqInteface='eth0'

if reqInteface in interfaces:
    ip='\n'+interfaces[reqInteface][0].address
else:
    ip=' xx  Wifi  xx\n    Disconnected'    #change this to print required message if interface not connected

# Print IP to LCD
lcd.message='IP:'+ip