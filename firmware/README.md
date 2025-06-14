# FACP iZone Firmware

This directory contains the firmware for the Fire Alarm Control Panel (FACP) iZone system running on RP2040-Zero with FreeRTOS.

## Overview

The firmware implements a distributed fire detection and alarm system using:
- **Hardware**: RP2040-Zero microcontroller
- **RTOS**: FreeRTOS SMP (Symmetric Multi-Processing) for dual-core operation
- **Architecture**: Master-slave with I2C communication
- **Safety**: Real-time response <100ms for fire detection

## Directory Structure

```
firmware/
├── CMakeLists.txt          # Main CMake build configuration
├── pico_sdk_import.cmake   # Pico SDK import script
├── README.md               # This file
├── config/
│   └── FreeRTOSConfig.h    # FreeRTOS configuration
├── include/
│   └── system_init.h       # System initialization header
├── src/
│   ├── main.c              # Main application entry point
│   ├── system_init.c       # System initialization implementation
│   └── freertos_config.c   # FreeRTOS port-specific configuration
└── lib/
    └── FreeRTOS-Kernel/    # FreeRTOS kernel (submodule)
```

## Build Requirements

### Prerequisites
- CMake 3.13 or later
- GCC ARM cross-compiler (`gcc-arm-none-eabi`)
- Raspberry Pi Pico SDK
- Git (for submodules)

### Windows Setup
```bash
# Install required tools (using scoop or similar)
scoop install cmake gcc-arm-none-eabi git

# Set environment variable for Pico SDK
$env:PICO_SDK_PATH = "C:\path\to\pico-sdk"
```

### Linux Setup
```bash
sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib
export PICO_SDK_PATH=/path/to/pico-sdk
```

## Building

1. **Clone Pico SDK** (if not already done):
```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
export PICO_SDK_PATH=$(pwd)
```

2. **Initialize submodules** (from project root):
```bash
git submodule update --init --recursive
```

3. **Build the firmware**:
```bash
cd firmware
mkdir build
cd build
cmake ..
make
```

## Output Files

After successful build, you'll find:
- `facp_izone.elf` - ELF file for debugging
- `facp_izone.uf2` - UF2 file for drag-and-drop programming
- `facp_izone.bin` - Binary file
- `facp_izone.hex` - Intel HEX file

## Programming the RP2040-Zero

1. **Enter BOOTSEL mode**:
   - Hold BOOTSEL button while connecting USB
   - Device should appear as mass storage device

2. **Program via UF2**:
   - Copy `facp_izone.uf2` to the mounted drive
   - Device will automatically reboot and run firmware

3. **Alternative - SWD Programming**:
   - Use OpenOCD or similar with SWD debugger
   - Load the `.elf` file for debugging

## Configuration

### FreeRTOS Configuration
The system is configured in `config/FreeRTOSConfig.h` with:
- **Dual-core SMP**: Both RP2040 cores utilized
- **Tick rate**: 1000 Hz (1ms resolution)
- **Memory**: 128KB heap, static allocation support
- **Safety**: Stack overflow detection, malloc failure hooks

### Fire Safety Priorities
Task priorities are defined for fire safety requirements:
- **Watchdog**: Highest priority (31)
- **Sensor Monitor**: Critical priority (30)
- **Alarm Control**: High priority (29)
- **Communication**: High priority (28)
- **Status LED**: Medium priority (24)
- **Diagnostics**: Low priority (22)

### Hardware Pin Assignments
Based on RP2040-Zero and custom hardware:
- **GPIO 25**: Status LED (built-in)
- **GPIO 2**: Power status LED
- **GPIO 3**: Normal operation LED
- **GPIO 4**: Alarm status LED
- **GPIO 5**: Fault status LED

## Testing

The basic firmware includes:
1. **LED Blink Task**: Blinks status LED every 500ms
2. **System Monitor**: Feeds watchdog, monitors system health
3. **Self-Test**: Memory, GPIO, and watchdog tests
4. **USB Serial**: Debug output via USB CDC

### Expected Behavior
- Power LED stays on (system alive)
- Status LED blinks every 500ms
- USB serial output shows system status

## Development Notes

### Current Implementation Status
✅ **Completed (Subtask 2.1)**:
- CMake build system setup
- Pico SDK integration
- FreeRTOS SMP configuration
- Basic LED blink example
- System initialization framework
- Hardware abstraction layer foundation

### Next Steps (Remaining Subtasks)
- **2.2**: Configure FreeRTOS for SMP operation
- **2.3**: Implement task management and synchronization
- **2.4**: Develop interrupt handling and timer framework
- **2.5**: Optimize and validate configuration

### Safety Considerations
- Watchdog timer enabled (30-second timeout)
- Stack overflow detection enabled
- Malloc failure handling
- Assert handling for debugging
- Real-time response requirements

## Troubleshooting

### Build Issues
- Ensure `PICO_SDK_PATH` is set correctly
- Verify FreeRTOS submodule is initialized
- Check CMake version (3.13+ required)

### Runtime Issues
- Check USB connection for debug output
- Verify RP2040-Zero is programmed correctly
- Monitor LED patterns for system status

## Contributing

When modifying the firmware:
1. Follow the existing code style and documentation
2. Test all changes thoroughly
3. Update this README if configuration changes
4. Ensure compliance with fire safety requirements
5. Maintain real-time response guarantees 