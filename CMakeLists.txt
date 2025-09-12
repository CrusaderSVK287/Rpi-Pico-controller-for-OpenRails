# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set path to the Pico SDK
# set(PICO_SDK_PATH "$ENV{PICO_SDK_PATH}")
include(pico_sdk_import.cmake)

project(rpi-pico-openrails-controller C CXX ASM)

# Initialize the SDK
pico_sdk_init()
# Add executable and sources
add_executable(rpi-pico-openrails-controller
    src/main.c
)

# Include directories
target_include_directories(rpi-pico-openrails-controller PRIVATE src)

# Enable TinyUSB device
target_compile_definitions(rpi-pico-openrails-controller PRIVATE
    PICO_STDIO_USB=1
)

# Link with standard Pico SDK libraries
target_link_libraries(rpi-pico-openrails-controller 
    pico_stdlib
    hardware_adc
    tinyusb_device
    tinyusb_board
)

# Generate UF2 and related binaries
pico_add_extra_outputs(rpi-pico-openrails-controller)

# pico_enable_stdio_usb(rpi-pico-openrails-controller 1)
# pico_enable_stdio_uart(rpi-pico-openrails-controller 0)
