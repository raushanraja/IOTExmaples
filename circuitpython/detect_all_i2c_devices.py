import time
import board
import bitbangio

bus = bitbangio.I2C(board.D5, board.D6)
bus.unlock()

while True:
    while not bus.try_lock():
        pass
    print( "I2C devices found:", [hex(address) for address in bus.scan()],)
    bus.unlock()
    time.sleep(0.5)
