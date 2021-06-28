from machine import Pin
import utime
#button = Pin(6,Pin.OUT,Pin.PULL_DOWN)
button = Pin(4,Pin.IN,Pin.PULL_UP)

print(button.value())
utime.sleep(2)
button.init(button.IN, button.PULL_DOWN)
print(button.value())
utime.sleep(2)
button.init(button.IN, button.PULL_UP)
print(button.value())
