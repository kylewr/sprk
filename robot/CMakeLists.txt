cmake_minimum_required(VERSION 3.30)
project(robot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    main.cpp
)

# Automatically find all .cpp files in subdirectories (if any)
file(GLOB_RECURSE EXTRA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/subsystem/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/base/*.cpp"
)

list(APPEND SOURCES ${EXTRA_SOURCES})

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
# Add parent directory so #include "robot/base/..." works correctly
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()