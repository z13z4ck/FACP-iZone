# Pico SDK Detection Module for FACP iZone Project
# This module provides robust detection of Pico SDK installations across platforms

# Function to detect Pico SDK installation
function(detect_pico_sdk)
    # If already set via environment or cache, use that
    if(DEFINED PICO_SDK_PATH AND EXISTS "${PICO_SDK_PATH}")
        message(STATUS "Using existing PICO_SDK_PATH: ${PICO_SDK_PATH}")
        return()
    endif()

    # Check environment variable first
    if(DEFINED ENV{PICO_SDK_PATH})
        set(candidate_path "$ENV{PICO_SDK_PATH}")
        if(EXISTS "${candidate_path}")
            set(PICO_SDK_PATH "${candidate_path}" CACHE PATH "Path to Pico SDK" FORCE)
            message(STATUS "Found Pico SDK from environment: ${PICO_SDK_PATH}")
            return()
        endif()
    endif()

    # Platform-specific detection
    set(sdk_candidates)
    
    if(WIN32)
        # Windows - Official installer paths
        list(APPEND sdk_candidates
            "C:/Program Files/Raspberry Pi/Pico SDK v1.5.1"
            "C:/Program Files/Raspberry Pi/Pico SDK v1.5.0" 
            "C:/Program Files/Raspberry Pi/Pico SDK v1.4.0"
            "C:/Program Files (x86)/Raspberry Pi/Pico SDK v1.5.1"
            "C:/pico/pico-sdk"
            "C:/pico-sdk"
            "C:/dev/pico-sdk"
            "${CMAKE_SOURCE_DIR}/../pico-sdk"
        )
    elseif(APPLE)
        # macOS paths
        list(APPEND sdk_candidates
            "/opt/pico-sdk"
            "/usr/local/pico-sdk"
            "/Users/$ENV{USER}/pico/pico-sdk"
            "${CMAKE_SOURCE_DIR}/../pico-sdk"
        )
    else()
        # Linux paths
        list(APPEND sdk_candidates
            "/opt/pico-sdk"
            "/usr/share/pico-sdk"
            "/usr/local/pico-sdk"
            "$ENV{HOME}/pico/pico-sdk"
            "$ENV{HOME}/pico-sdk"
            "${CMAKE_SOURCE_DIR}/../pico-sdk"
        )
    endif()

    # Search for valid SDK installation
    foreach(candidate IN LISTS sdk_candidates)
        if(EXISTS "${candidate}/pico_sdk_init.cmake")
            set(PICO_SDK_PATH "${candidate}" CACHE PATH "Path to Pico SDK" FORCE)
            message(STATUS "Auto-detected Pico SDK: ${PICO_SDK_PATH}")
            return()
        endif()
    endforeach()

    # If not found, provide helpful error message
    message(FATAL_ERROR 
        "Pico SDK not found. Please:\n"
        "  1. Set PICO_SDK_PATH environment variable\n"
        "  2. Install using official Raspberry Pi installer\n"
        "  3. Clone SDK manually: git clone https://github.com/raspberrypi/pico-sdk\n"
        "  Searched paths:\n    ${sdk_candidates}"
    )
endfunction()

# Validate SDK installation
function(validate_pico_sdk)
    if(NOT DEFINED PICO_SDK_PATH OR NOT EXISTS "${PICO_SDK_PATH}")
        message(FATAL_ERROR "PICO_SDK_PATH not set or invalid: ${PICO_SDK_PATH}")
    endif()

    # Check for required files
    set(required_files
        "pico_sdk_init.cmake"
        "pico_sdk_version.cmake"
        "src/common/pico_stdlib/include/pico/stdlib.h"
    )

    foreach(required_file IN LISTS required_files)
        set(full_path "${PICO_SDK_PATH}/${required_file}")
        if(NOT EXISTS "${full_path}")
            message(WARNING "Missing SDK file: ${full_path}")
        endif()
    endforeach()

    # Try to determine SDK version
    if(EXISTS "${PICO_SDK_PATH}/pico_sdk_version.cmake")
        include("${PICO_SDK_PATH}/pico_sdk_version.cmake")
        if(DEFINED PICO_SDK_VERSION_STRING)
            message(STATUS "Pico SDK Version: ${PICO_SDK_VERSION_STRING}")
        endif()
    endif()
endfunction()

# Main detection logic
detect_pico_sdk()
validate_pico_sdk() 