import board
import digitalio
import time

# Define pins for the rotary encoder
encoder_pin_a = digitalio.DigitalInOut(board.GP20)
encoder_pin_b = digitalio.DigitalInOut(board.GP21)
encoder_pin_a.direction = digitalio.Direction.INPUT
encoder_pin_b.direction = digitalio.Direction.INPUT

# Define pin for the switch
switch_pin = digitalio.DigitalInOut(board.GP19)
switch_pin.direction = digitalio.Direction.INPUT
switch_pin.pull = digitalio.Pull.UP

# Initialize variables to track the state of the rotary encoder and the switch
prev_state_a = encoder_pin_a.value
prev_state_b = encoder_pin_b.value
counter = 0
switch_pressed = False

# Initialize variables for time-based debouncing
debounce_time = 0.001 # debounce time in seconds
debounce_timer_a = time.monotonic()
debounce_timer_b = time.monotonic()

while True:
    # Read the state of each encoder pin
    current_state_a = encoder_pin_a.value
    current_state_b = encoder_pin_b.value
    
    # Check if the state of either encoder pin has changed
    if current_state_a != prev_state_a:
        # Check if enough time has passed since the last debounce
        if time.monotonic() - debounce_timer_a > debounce_time:
            # Check the direction of the rotary encoder
            if prev_state_a == current_state_b:
                counter += 1
            else:
                counter -= 1

            # Print the current count
            print("Count: ", counter)

            # Update the previous state variables and debounce timer
            prev_state_a = current_state_a
            debounce_timer_a = time.monotonic()
    
    if current_state_b != prev_state_b:
        # Check if enough time has passed since the last debounce
        if time.monotonic() - debounce_timer_b > debounce_time:
            # Check the direction of the rotary encoder
            if prev_state_b == current_state_a:
                counter -= 1
            else:
                counter += 1

            # Print the current count
            print("Count: ", counter)

            # Update the previous state variables and debounce timer
            prev_state_b = current_state_b
            debounce_timer_b = time.monotonic()

    # Check if the state of the switch pin has changed
    if not switch_pin.value and not switch_pressed:
        switch_pressed = True
        print("Switch pressed")
        # Do something when the switch is pressed
    elif switch_pin.value and switch_pressed:
        switch_pressed = False
        print("Switch released")

