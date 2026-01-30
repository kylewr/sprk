cd ./build-rpi
cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake ../src/ && cmake --build . --clean-first
ssh pi@192.168.1.111 "rm /home/pi/robot"
scp ./robot pi@192.168.1.111:/home/pi/
ssh pi@192.168.1.111 "chmod +x /home/pi/robot"