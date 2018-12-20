#!/usr/bin/python
# Example using a character LCD connected to a Raspberry Pi using 
# Adafruit_CircuitPython_CharLCD librabry https://github.com/adafruit/Adafruit_CircuitPython_CharLCD 
import time
import board
import digitalio
import adafruit_character_lcd
import adafruit_character_lcd.character_lcd as characterlcd

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

# Print a 2x line message
lcd.message='hello\nWorld'

