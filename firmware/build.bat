@echo off
REM FACP iZone Firmware Build Script for Windows
REM This script automates the build process with proper SDK detection

echo =======================================================
echo FACP iZone Firmware Build Script
echo =======================================================

REM Set default Pico SDK path for this installation
if not defined PICO_SDK_PATH (
    if exist "C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\pico-sdk" (
        set "PICO_SDK_PATH=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\pico-sdk"
        echo Using official Pico SDK installation: %PICO_SDK_PATH%
    ) else (
        echo Warning: PICO_SDK_PATH not set and official installation not found
        echo Please set PICO_SDK_PATH environment variable or install Pico SDK
        echo Download from: https://github.com/raspberrypi/pico-setup-windows
        pause
        exit /b 1
    )
) else (
    echo Using PICO_SDK_PATH: %PICO_SDK_PATH%
)

REM Verify SDK exists
if not exist "%PICO_SDK_PATH%\pico_sdk_init.cmake" (
    echo Error: Invalid Pico SDK path - pico_sdk_init.cmake not found
    echo Path: %PICO_SDK_PATH%
    pause
    exit /b 1
)

REM Set up ARM toolchain path if not already in PATH
set "ARM_TOOLCHAIN_PATH=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\gcc-arm-none-eabi\bin"
if exist "%ARM_TOOLCHAIN_PATH%\arm-none-eabi-gcc.exe" (
    echo Found ARM toolchain at: %ARM_TOOLCHAIN_PATH%
    set "PATH=%ARM_TOOLCHAIN_PATH%;%PATH%"
    set "PICO_TOOLCHAIN_PATH=%ARM_TOOLCHAIN_PATH%"
) else (
    echo Warning: ARM toolchain not found at expected location
    echo Please ensure arm-none-eabi-gcc is installed and in PATH
)

REM Set build type (Release or Debug)
set BUILD_TYPE=Release
if "%1"=="debug" set BUILD_TYPE=Debug
if "%1"=="Debug" set BUILD_TYPE=Debug

echo Build Type: %BUILD_TYPE%
echo.

REM Create and enter build directory
if not exist build mkdir build
cd build

REM Set up additional build tools
set "NINJA_PATH=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\ninja"
if exist "%NINJA_PATH%\ninja.exe" (
    set "PATH=%NINJA_PATH%;%PATH%"
    echo Found Ninja build system
)

set "CMAKE_PATH=C:\Program Files\Raspberry Pi\Pico SDK v1.5.1\cmake\bin"
if exist "%CMAKE_PATH%\cmake.exe" (
    set "PATH=%CMAKE_PATH%;%PATH%"
    echo Found CMake in Pico SDK
)

REM Configure with CMake using Ninja generator
echo Configuring project with CMake...
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if errorlevel 1 (
    echo.
    echo CMake configuration failed!
    echo Check that you have:
    echo   - CMake 3.13 or later
    echo   - GCC ARM cross-compiler
    echo   - Valid Pico SDK installation
    echo.
    pause
    exit /b 1
)

REM Build the project
echo.
echo Building firmware...
cmake --build . --config %BUILD_TYPE% --parallel
if errorlevel 1 (
    echo.
    echo Build failed!
    pause
    exit /b 1
)

echo.
echo =======================================================
echo Build completed successfully!
echo =======================================================
echo.
echo Output files in build/:
if exist facp_izone.uf2 echo   - facp_izone.uf2 (for drag-and-drop programming)
if exist facp_izone.elf echo   - facp_izone.elf (for debugging)
if exist facp_izone.bin echo   - facp_izone.bin (binary file)
if exist facp_izone.hex echo   - facp_izone.hex (Intel HEX file)
echo.
echo To program RP2040-Zero:
echo   1. Hold BOOTSEL button while connecting USB
echo   2. Copy facp_izone.uf2 to the mounted drive
echo   3. Device will reboot and run firmware
echo.

REM Show size information
if exist facp_izone.elf (
    echo Memory usage:
    arm-none-eabi-size facp_izone.elf 2>nul || echo Size tool not available
    echo.
)

pause 