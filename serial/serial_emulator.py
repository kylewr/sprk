from serial import Serial
from threading import Thread

def parse_commands(command, port):
    match command:
        case "help":
            printf("No help options are avaliable.")
        case "cls":
            port.write(b'\033[H\033[2J')
        case "shutdown":
            printn("Are you sure? ")
            def retFunc(inp):
                if inp.lower() == "yes":
                    printf("Shutdown.")
                    p.close()
                else:
                    printf("");
                return True
            return (True, retFunc)
        case "":
            port.write(b'\n')
        case _:
            printf('\033[31mUnknown Command!\033[0m')
    write_prompt()
    return (False, None)

def respond_worker(port):
    command = ''
    do_hold = False
    currentHoldRunner = lambda: None
    while True:
        inp = port.read(1)
        decoded = inp.decode("utf-8")
        if not (decoded == '\x7f' and command == ''):
            port.write(inp)
        
        # print(repr(decoded))

        if decoded == '\x7f': # backslash character
            command = command[:-1]
        elif decoded == '\r': # new line (carriage return)
            # print(command)
            if do_hold:
                can_return = currentHoldRunner(command)
                if can_return:
                    write_prompt()
                    do_hold = False
            else:
                do_hold, currentHoldRunner = parse_commands(command, port)
                
            command = ''
        else:
            command += decoded

p = Serial('COM10', 9600, timeout=50)

write_prompt = lambda: p.write(b'\r\033[36mQuack CMD\033[0m> ')
printn = lambda t: p.write((f'\n{t}').encode())
printf = lambda t: printn(t + "\n\r")

threads = [
    Thread(None, lambda: respond_worker(port=p), daemon=True)
]

write_prompt()

p.reset_input_buffer()
p.reset_output_buffer()

[i.start() for i in threads]

while True:
    i = input("cmd: ")
    to_encode = i + "\r"
    p.write(to_encode.encode())
    warn, _ = parse_commands(i, p)
    if warn:
        printf("\r\033[93mWarning! A command callback was supposed to run, but was not allowed to!\033[0m")
    write_prompt()
