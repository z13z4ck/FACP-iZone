cmake_minimum_required(VERSION 3.13)

# Pull in PICO SDK (if not already in system path)
include(pico_sdk_import.cmake)

# Define project name and languages
project(facp_izone_firmware C CXX ASM)

# Set C/C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
pico_sdk_init()

# Include FreeRTOS
add_subdirectory(lib/FreeRTOS-Kernel freertos)

# Create main executable
add_executable(facp_izone
    src/main.c
    src/freertos_config.c
    src/system_init.c
)

# Include directories
target_include_directories(facp_izone PRIVATE
    include
    config
)

# Link required libraries
target_link_libraries(facp_izone 
    pico_stdlib
    pico_multicore
    hardware_gpio
    hardware_i2c
    hardware_watchdog
    hardware_timer
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# Enable USB output, disable UART output for this project
pico_enable_stdio_usb(facp_izone 1)
pico_enable_stdio_uart(facp_izone 0)

# Create map/bin/hex file etc.
pico_add_extra_outputs(facp_izone) 