#!/bin/bash
# FACP iZone Firmware Build Script for Linux/Unix
# This script automates the build process with proper SDK detection

set -e  # Exit on any error

echo "======================================================="
echo "FACP iZone Firmware Build Script"
echo "======================================================="

# Function to check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Set default Pico SDK path detection
if [ -z "$PICO_SDK_PATH" ]; then
    # Try common installation paths
    SDK_CANDIDATES=(
        "/opt/pico-sdk"
        "/usr/share/pico-sdk" 
        "/usr/local/pico-sdk"
        "$HOME/pico/pico-sdk"
        "$HOME/pico-sdk"
        "$(dirname "$0")/../pico-sdk"
    )
    
    for candidate in "${SDK_CANDIDATES[@]}"; do
        if [ -f "$candidate/pico_sdk_init.cmake" ]; then
            export PICO_SDK_PATH="$candidate"
            echo "Auto-detected Pico SDK: $PICO_SDK_PATH"
            break
        fi
    done
    
    if [ -z "$PICO_SDK_PATH" ]; then
        echo "Error: PICO_SDK_PATH not set and SDK not found in common locations"
        echo "Please set PICO_SDK_PATH environment variable or install Pico SDK"
        echo "Download from: https://github.com/raspberrypi/pico-sdk"
        echo ""
        echo "Example:"
        echo "  git clone https://github.com/raspberrypi/pico-sdk.git"
        echo "  cd pico-sdk"
        echo "  git submodule update --init"
        echo "  export PICO_SDK_PATH=\$(pwd)"
        exit 1
    fi
else
    echo "Using PICO_SDK_PATH: $PICO_SDK_PATH"
fi

# Verify SDK installation
if [ ! -f "$PICO_SDK_PATH/pico_sdk_init.cmake" ]; then
    echo "Error: Invalid Pico SDK path - pico_sdk_init.cmake not found"
    echo "Path: $PICO_SDK_PATH"
    exit 1
fi

# Check required tools
echo "Checking build tools..."
MISSING_TOOLS=()

if ! command_exists cmake; then
    MISSING_TOOLS+=("cmake")
fi

if ! command_exists arm-none-eabi-gcc; then
    MISSING_TOOLS+=("gcc-arm-none-eabi")
fi

if ! command_exists make; then
    MISSING_TOOLS+=("make")
fi

if [ ${#MISSING_TOOLS[@]} -ne 0 ]; then
    echo "Error: Missing required tools: ${MISSING_TOOLS[*]}"
    echo ""
    echo "Install on Ubuntu/Debian:"
    echo "  sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib"
    echo ""
    echo "Install on macOS:"
    echo "  brew install cmake gcc-arm-embedded"
    exit 1
fi

# Set build type
BUILD_TYPE="Release"
if [ "$1" = "debug" ] || [ "$1" = "Debug" ]; then
    BUILD_TYPE="Debug"
fi

echo "Build Type: $BUILD_TYPE"
echo ""

# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring project with CMake..."
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE"

# Build the project
echo ""
echo "Building firmware..."
cmake --build . --config "$BUILD_TYPE" --parallel "$(nproc 2>/dev/null || echo 4)"

echo ""
echo "======================================================="
echo "Build completed successfully!"
echo "======================================================="
echo ""
echo "Output files in build/:"
[ -f facp_izone.uf2 ] && echo "  - facp_izone.uf2 (for drag-and-drop programming)"
[ -f facp_izone.elf ] && echo "  - facp_izone.elf (for debugging)"
[ -f facp_izone.bin ] && echo "  - facp_izone.bin (binary file)"
[ -f facp_izone.hex ] && echo "  - facp_izone.hex (Intel HEX file)"
echo ""
echo "To program RP2040-Zero:"
echo "  1. Hold BOOTSEL button while connecting USB"
echo "  2. Copy facp_izone.uf2 to the mounted drive"
echo "  3. Device will reboot and run firmware"
echo ""

# Show size information
if [ -f facp_izone.elf ] && command_exists arm-none-eabi-size; then
    echo "Memory usage:"
    arm-none-eabi-size facp_izone.elf
    echo ""
fi

echo "Build script completed successfully!" 