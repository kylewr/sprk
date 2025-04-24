import tkinter as tk
import socket
import threading

class DS(tk.Frame):
    def __init__(self, parent):
        tk.Frame.__init__(self, parent, width=400,  height=400)

        self.label = tk.Label(self, text="last key pressed:  ", width=20)
        self.label.pack(fill="both", padx=100, pady=100)

        self.label.bind("<w>", self.on_wasd)
        self.label.bind("<a>", self.on_wasd)
        self.label.bind("<s>", self.on_wasd)
        self.label.bind("<d>", self.on_wasd)
        self.label.bind("<e>", lambda event: self.sendMessage("exit"))
        self.label.bind("<space>", lambda event: self.sendMessage("te-s"))

        self.label.bind("<Return>", lambda event: self.sendMessage("dis"))
        self.label.bind("<BackSpace>", lambda event: self.sendMessage("tele"))

        self.label.bind("<Escape>", self.close)

        self.label.focus_set()
        self.label.bind("<1>", lambda event: self.label.focus_set())
        self.socket = None
        self.x = 0
        self.y = 0

    def pass_socket(self, socket):
        self.socket = socket

    def sendMessage(self, message: str):
        if self.socket:
            self.socket.sendall(message.encode('utf-8'))

    def close(self, event):
        self.socket.close()
        self.quit()

    def on_wasd(self, event):
        self.label.configure(text="last key pressed: " + event.keysym)
        match event.keysym:
            case "w":
                self.y = 1
            case "a":
                self.x = -1
            case "s":
                self.y = -1
            case "d":
                self.x = 1
        pack = f"te-d,{self.x},{self.y},0"
        print(pack)
        self.sendMessage(pack)
                        
if __name__ == "__main__":
    root = tk.Tk()
    root.title("S.H.A.R.K. Controller")

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 8008))

    win = DS(root)
    win.pack(fill="both", expand=True)
    win.pass_socket(s)

    root.mainloop()