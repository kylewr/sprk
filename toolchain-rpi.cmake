# Toolchain file for cross-compiling to Raspberry Pi

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Specify the cross compiler
# For 32-bit RPi (Zero, 1, 2, 3):
# set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
# set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# For 64-bit RPi (3B+, 4, 5) use:
set(CMAKE_C_COMPILER aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER aarch64-linux-gnu-g++)

# Where to look for the target environment
set(CMAKE_FIND_ROOT_PATH /usr/aarch64-linux-gnu)

# Search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# Search for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Compiler flags for Raspberry Pi
# For RPi 3/4 (ARMv8):
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a -mtune=cortex-a53")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv8-a -mtune=cortex-a53")

# For RPi Zero/1 (ARMv6):
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv6 -mfpu=vfp -mfloat-abi=hard")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=armv6 -mfpu=vfp -mfloat-abi=hard")
