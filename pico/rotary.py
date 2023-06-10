import board
import digitalio
import adafruit_debouncer

# Define pins for the rotary encoder
encoder_pin_a = digitalio.DigitalInOut(board.GP20)
encoder_pin_b = digitalio.DigitalInOut(board.GP21)
encoder_pin_a.direction = digitalio.Direction.INPUT
encoder_pin_b.direction = digitalio.Direction.INPUT

# Define pin for the switch
switch_pin = digitalio.DigitalInOut(board.GP19)
switch_pin.direction = digitalio.Direction.INPUT
switch_pin.pull = digitalio.Pull.UP

# Initialize debouncers for each encoder pin and the switch pin
debouncer_a = adafruit_debouncer.Debouncer(encoder_pin_a)
debouncer_b = adafruit_debouncer.Debouncer(encoder_pin_b)
debouncer_switch = adafruit_debouncer.Debouncer(switch_pin)

# Initialize variables to track the state of the rotary encoder and the switch
prev_state_a = debouncer_a.value
prev_state_b = debouncer_b.value
counter = 0
switch_pressed = False
button_counter = 0

while True:
    # Update the debouncer for each encoder pin and the switch pin
    debouncer_a.update()
    debouncer_b.update()
    debouncer_switch.update()

    # Check if the state of either encoder pin has changed
    if debouncer_a.value != prev_state_a or debouncer_b.value != prev_state_b:
        # Check the direction of the rotary encoder
        if switch_pressed:
            if prev_state_a == debouncer_b.value:
                button_counter += 1/4
            else:
                button_counter -= 1/4
        else:
            if prev_state_a == debouncer_b.value:
                counter += 1/4
            else:
                counter -= 1/4

        # Check if the switch is pressed

        # Print the current count
        print(f"Count:{counter}, Button Count:{button_counter}")

        # Update the previous state variables
        prev_state_a = debouncer_a.value
        prev_state_b = debouncer_b.value

    # Check if the state of the switch pin has changed
    if debouncer_switch.fell:
        switch_pressed = True
        print("Switch pressed")
    elif debouncer_switch.rose:
        switch_pressed = False
        print("Switch released")
