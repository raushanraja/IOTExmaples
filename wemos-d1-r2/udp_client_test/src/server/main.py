import socket

UDP_IP = "esp8266.local"
UDP_PORT = 4210
MESSAGE = b"Hello, World!"

print("UDP target IP: %s" % UDP_IP)
print("UDP target PORT: %s" % UDP_PORT)
print("Sending Message: %s" % MESSAGE)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
