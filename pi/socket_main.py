import socket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('localhost', 8008))

s.listen(1)

while True:
    bigBreak = False
    print("\033[93mAwaiting connection...\033[0m")
    conn, addr = s.accept()
    print(f'\033[92mConnected. {addr}\033[0m')

    while True:
        data = conn.recv(256)
        if not data:
            break
        msg = data.decode('utf-8')
        if msg == "exit":
            bigBreak = True
            break
        print(msg)

    print(f"\033[91mSocket lost connection! {addr}\033[0m")
    if (bigBreak):
        break

print("Socket closed. Exiting...")