import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('localhost', 8008))

while True:
    msg = input("Enter message: ")
    s.sendall(msg.encode('utf-8'))
    if (msg == "exit"):
        break