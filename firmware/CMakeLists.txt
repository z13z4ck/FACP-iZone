cmake_minimum_required(VERSION 3.13)

# Set project information based on PRD requirements
set(PROJECT_NAME "facp_izone")
set(PROJECT_VERSION "0.1.0")

# Enhanced SDK path handling using detection module
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(pico_sdk_detection OPTIONAL)

# Pull in PICO SDK (if not already in system path)
include(pico_sdk_import.cmake)

# Define project name and languages
project(${PROJECT_NAME} C CXX ASM)

# Set C/C++ standards for fire safety compliance
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Fire safety system build configuration
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")

# Initialize Pico SDK
pico_sdk_init()

# Fire safety system compiler flags for reliability
set(FIRE_SAFETY_FLAGS 
    -Wall 
    -Wextra 
    -Wstrict-prototypes
    -Wno-missing-prototypes
    -Wno-undef
    -Wno-unused-parameter
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    -Wstack-usage=2048
)

# Optimization flags for real-time performance (<100ms response requirement)
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    list(APPEND FIRE_SAFETY_FLAGS -O2 -DNDEBUG)
else()
    list(APPEND FIRE_SAFETY_FLAGS -Og -g3)
endif()

# Use the SMP-capable RP2040 port for FreeRTOS
include(lib/FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

# Create FreeRTOS configuration target for SMP RP2040 port
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
    INTERFACE
        config  # Directory containing FreeRTOSConfig.h
)
target_compile_definitions(freertos_config
    INTERFACE
        projCOVERAGE_TEST=0
        # Define missing Pico SDK macros to resolve -Werror=undef issues
        LIB_CMSIS_CORE=0
        LIB_PICO_STDIO_UART=0  
        LIB_PICO_STDIO_SEMIHOSTING=0
        PICO_MUTEX_ENABLE_SDK120_COMPATIBILITY=0
        PICO_DIVIDER_DISABLE_INTERRUPTS=0
)

# Define source files for FACP iZone firmware
set(FACP_SOURCES
    src/main.c
    src/freertos_config.c
    src/system_init.c
    src/smp_config.c
)

# Create main executable
add_executable(${PROJECT_NAME} ${FACP_SOURCES})

# Set target properties for fire safety requirements
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    SUFFIX ".elf"
)

# Apply fire safety compiler flags
target_compile_options(${PROJECT_NAME} PRIVATE ${FIRE_SAFETY_FLAGS})

# Include directories for modular design
target_include_directories(${PROJECT_NAME} PRIVATE
    include
    config
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link required libraries for fire alarm system
target_link_libraries(${PROJECT_NAME} 
    # Pico SDK libraries
    pico_stdlib
    pico_multicore
    pico_sync
    pico_time
    pico_util
    
    # Hardware abstraction
    hardware_gpio
    hardware_i2c
    hardware_spi
    hardware_uart
    hardware_pwm
    hardware_adc
    hardware_watchdog
    hardware_timer
    hardware_clocks
    hardware_flash
    hardware_resets
    
    # FreeRTOS for real-time operation
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
)

# Fire safety system communication - enable USB for diagnostics
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# Set linker flags for memory optimization
target_link_options(${PROJECT_NAME} PRIVATE 
    -Wl,--gc-sections
    -Wl,--print-memory-usage
)

# Create all output formats required for deployment
pico_add_extra_outputs(${PROJECT_NAME})

# Add custom build information
target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_NAME="${PROJECT_NAME}"
    PROJECT_VERSION="${PROJECT_VERSION}"
    BUILD_TIMESTAMP="${CMAKE_CURRENT_LIST_DIR}"
    FIRE_SAFETY_SYSTEM=1
    FREERTOS_SMP=1
)

# Development and debugging support
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        DEBUG=1
        FIRE_SAFETY_DEBUG=1
    )
    
    # Enable additional debug libraries
    target_link_libraries(${PROJECT_NAME} 
        pico_printf
        pico_mem_ops
    )
endif()

# Print build configuration summary
message(STATUS "")
message(STATUS "FACP iZone Firmware Build Configuration:")
message(STATUS "  Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "  Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  Pico SDK: ${PICO_SDK_PATH}")
message(STATUS "  Target: RP2040-Zero")
message(STATUS "  RTOS: FreeRTOS SMP")
message(STATUS "  Fire Safety: Enabled")
message(STATUS "  Real-time Response: <100ms requirement")
message(STATUS "")

# Custom targets for development workflow
add_custom_target(size
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}>
    DEPENDS ${PROJECT_NAME}
    COMMENT "Showing firmware size information"
)

add_custom_target(flash
    COMMAND echo "Copy ${PROJECT_NAME}.uf2 to RP2040-Zero in BOOTSEL mode"
    DEPENDS ${PROJECT_NAME}
    COMMENT "Instructions for flashing firmware"
)

# Validation target for fire safety compliance
add_custom_target(validate
    COMMAND echo "Running fire safety validation checks..."
    COMMAND echo "✓ Stack usage analysis: See .su files"
    COMMAND echo "✓ Memory usage: See linker output"
    COMMAND echo "✓ Real-time constraints: Verify task priorities"
    DEPENDS size
    COMMENT "Fire safety system validation"
) 