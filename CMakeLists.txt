cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set path to the Pico SDK
# set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
include(pico_sdk_import.cmake)

project(rpi-pico-openrails-controller C CXX ASM)

# Initialize the SDK
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
pico_sdk_init()
# Add executable and sources
add_executable(rpi-pico-openrails-controller
    src/main.cpp
    src/blink.cpp
)

# Include directories
target_include_directories(rpi-pico-openrails-controller PRIVATE src)

# Link with standard Pico SDK libraries
target_link_libraries(rpi-pico-openrails-controller pico_stdlib)

# Generate UF2 and related binaries
pico_add_extra_outputs(rpi-pico-openrails-controller)

pico_enable_stdio_usb(rpi-pico-openrails-controller 1)
pico_enable_stdio_uart(rpi-pico-openrails-controller 0)
