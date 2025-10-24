import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.1.31', 8008))
def send_message(msg):
    msg = input("m: ")
    s.sendall(msg.encode('utf-8'))

while True:
    send_message(s)