import time
import board
import bitbangio

# Define the I2C bus using bitbangio for ESP32-S3-Zero module
# But same code can be used for other boards as well
# Update the bus= bitbangio.I2C to select specific pins for SDA and SCL
bus = bitbangio.I2C(board.D5, board.D6)

# Unlock the I2C bus to enable configuration
bus.unlock()

while True:
    while not bus.try_lock():
        pass
    
    # Scan for I2C devices and print their addresses
    print("I2C devices found:", [hex(address) for address in bus.scan()])
    
    # Unlock the bus and wait for 0.5 seconds before scanning again
    bus.unlock()
    time.sleep(0.5)
