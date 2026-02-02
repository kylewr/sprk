# S.P.R.K.

## Parts of the project
1. The SPRK robot program (this repository) running on a Raspberry Pi.
2. The ESP-32 that runs [custom software](https://github.com/kylewr/esp32-sprk) that handles all low level IO on the robot and communicates to the Pi via SPI.
3. The [teleoperator console](https://github.com/kylewr/sprk-controller) that connects to the Pi via a network socket.

## Purpose
The purpose of this was to create a general purpose robot framework, mainly for use on the SPRK robot. Hence, why `src/base/` exists.

## Building the project
1. For a `x64` linux target, run
```bash
mkdir ./build && cd ./build && cmake ./src && cmake --build . --clean-first
```
The output will be an executable `./robot`

2. For an `arm` target, use the `toolchain-rpi.cmake` file by running
```bash
mkdir ./build-arm && cd ./build-arm && cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-rpi.cmake ../src/ && cmake --build . --clean-first
```
### Deploying to RPi
Yes, the IP is hardcoded in `deploy.sh`. There is a VS code task that runs it.


## If you read this far...
Don't ask me what SPRK stands for. I don't know either. Originally, the robot was named SHARK for "Self Height Adjusting Robot with Kinematics", and when I realized I didn't have CAD experience at the time, was quickly scrapped. We put off the renaming the project until the day of the STEM showcase.