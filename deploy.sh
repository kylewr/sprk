cmake ./build-rpi && cmake ../src && cmake --build . --clean-first
ssh pi@192.168.1.50 "sudo rm /home/pi/robot"
scp ./build-rpi/robot pi@192.168.1.50:/home/pi/
ssh pi@192.168.1.50 "sudo chmod +x /home/pi/robot"