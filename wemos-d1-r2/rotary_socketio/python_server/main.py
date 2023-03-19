import os, eventlet
import subprocess
import socketio
from collections import deque

# Create a Socket.IO server.
sio = socketio.Server(cors_allowed_origins='*')

# Define the commands that the server can handle.
commands = {
    'volume_up': 'amixer set Master 10%+',
    'volume_down': 'amixer set Master 10%-',
    'discord': 'discord',
}

# Define a handler function for each command.
def volume_up():
    subprocess.Popen(['amixer', 'set', 'Master', '5%+'])
    notify("Volume Up")

def volume_down():
    subprocess.Popen(['amixer', 'set', 'Master', '5%-'])
    notify("Volume Down")

def open_discord():
    subprocess.Popen(['discord'])

# Notify Envet: notify-send "Title" "Message"
def notify(message, title='Rotary Notification'):
    subprocess.Popen(['notify-send', title, message])

# Register the handler functions with Socket.IO.
@sio.event
def connect(sid, environ):
    print('Client connected')

@sio.event
def disconnect(sid):
    print('Client disconnected')

@sio.on('*')
def catch_all(event, sid, data):
    print(event)


# Methods to scroll through the menus
def scroll_up(q):
    q.rotate(1)
    return q[0]


def scroll_down(q):
    q.rotate(-1)
    return q[0]


# Define the two menus
primary = deque(["Volume", "AppSelect"])
secondary = deque(["Discord", "Spotify", "Firefox", "Exit"])


# Varaibles to keep track of the current selection
sapp = None
menu =  primary
menu_stack = [primary]
scroll = None



# Function to handle the CW rotation

# Function to handle the CCW rotation
def rotate_cw():
    global menu
    global sapp
    global scroll

    if sapp is None and scroll is None:
        scroll = scroll_up(menu)
        notify(f"Cursor at {scroll}")
    
    elif sapp is None and scroll is not None:
        scroll = scroll_up(menu)
        notify(f"Cursor at {scroll}")

    elif app is not None:
        if sapp == "Volume":
            volume_up()
        elif sapp == "Discord":
            notify(f"{sapp} cw")
        elif sapp == "Spotify":
            notify(f"{sapp} cw")
        elif sapp == "Firefox":
            notify(f"{sapp} cw")

    

# Function to handle the CCW rotation
def rotate_ccw():
    global menu
    global sapp
    global scroll

    if sapp is None and scroll is None:
        scroll = scroll_down(menu)
        notify(f"Cursor at {scroll}")
 
    elif sapp is None and scroll is not None:
        scroll = scroll_down(menu)
        notify(f"Cursor at {scroll}")
       
    elif sapp is not None:
        if sapp == "Volume":
            volume_down()
        elif sapp == "Discord":
            notify(f"{sapp} ccw")
        elif sapp == "Spotify":
            notify(f"{sapp} ccw")
        elif sapp == "Firefox":
            notify(f"{sapp} ccw")


def back_menu():
    global menu_stack
    global menu
    global sapp
    global scroll

    if menu is not primary and sapp is None:
        menu_stack.pop()  # pop the current sub-menu off the stack
        menu = menu_stack[-1]  # update the current menu
        scroll = menu[0]
        notify(f"Back to previous menu, available opts: {menu}")

        if menu is primary:
            sapp = None
    
    else:
        sapp = None
        scroll = menu[0]
        notify(f"Back to previous menu, available opts: {menu}")




# Function to handle the button press
def button_press():
    global menu
    global sapp
    global scroll

    if sapp is None and scroll is not None:
        if scroll == "Volume":
            sapp = scroll
            notify(f"{sapp} selected")
        elif scroll == "AppSelect":
            sapp = None
            menu_stack.append(secondary)
            menu = secondary
            scroll = menu[0]
            notify(f"Cursor at {scroll}")
        
        elif scroll == "Exit":
            back_menu()


        else:
            sapp = scroll
            notify(f"{sapp} selected")
            if sapp == "Discord":
                open_discord()

    elif sapp is None and scroll is None:
        notify("No app selected, please scroll to select an app")
    
    elif sapp is not None:
        back_menu()


@sio.on('message')
def handle_command(sid, msg):
    print(f'Received message: {msg}')
    dir = msg["dir"]
    if dir == "CW":
        rotate_cw()
    elif dir == "CCW":
        rotate_ccw()
    elif dir == "Button" and msg["counter"] == "1":
        button_press()

# Start the Socket.IO server.
if __name__ == '__main__':
    app = socketio.WSGIApp(sio)
    app = socketio.Middleware(sio, app)
    port = 5000
    print(f'Server listening on port {port}')
    eventlet.wsgi.server(eventlet.listen(('', port)), app)
