# FACP iZone Firmware Build Guide

This document provides detailed instructions for building the FACP iZone firmware with the updated CMake configuration.

## Quick Start

### Windows
```batch
# Automatic build (recommended)
build.bat

# Debug build
build.bat debug
```

### Linux/macOS
```bash
# Make script executable (first time only)
chmod +x build.sh

# Automatic build (recommended)
./build.sh

# Debug build  
./build.sh debug
```

### Manual Build (All Platforms)
```bash
# Set up environment (if not auto-detected)
export PICO_SDK_PATH="C:\Program Files\Raspberry Pi\Pico SDK v1.5.1"  # Windows
export PICO_SDK_PATH="/opt/pico-sdk"                                   # Linux

# Configure and build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

## Build Configuration Features

### Enhanced SDK Detection
The CMake configuration automatically detects Pico SDK installations in common locations:

**Windows:**
- `C:\Program Files\Raspberry Pi\Pico SDK v1.5.1` (Official installer)
- `C:\Program Files\Raspberry Pi\Pico SDK v1.5.0`
- `C:\pico\pico-sdk`
- Environment variable `PICO_SDK_PATH`

**Linux/macOS:**
- `/opt/pico-sdk`
- `/usr/share/pico-sdk`
- `/usr/local/pico-sdk`
- `$HOME/pico/pico-sdk`
- Environment variable `PICO_SDK_PATH`

### Fire Safety System Configuration
The build system includes fire safety specific optimizations:

- **Compiler Flags**: `-Wall -Wextra -Werror` for robust code quality
- **Stack Monitoring**: `-Wstack-usage=2048` to prevent stack overflow
- **Memory Optimization**: Section garbage collection for minimal footprint
- **Real-time Performance**: Optimized for <100ms response requirement

### Build Types
- **Release** (default): Optimized for production use (-O2)
- **Debug**: Includes debug symbols and reduced optimization (-Og -g3)
- **RelWithDebInfo**: Release optimization with debug information

## Output Files

After successful build, these files are generated in `build/`:

| File | Purpose | Usage |
|------|---------|--------|
| `facp_izone.uf2` | UF2 bootloader format | Drag-and-drop programming |
| `facp_izone.elf` | ELF debug format | Debugging with GDB/OpenOCD |
| `facp_izone.bin` | Raw binary | Direct flash programming |
| `facp_izone.hex` | Intel HEX format | Programming tools |

## Custom Build Targets

The CMake configuration provides additional targets:

```bash
# Show memory usage
cmake --build . --target size

# Display flash instructions
cmake --build . --target flash

# Run fire safety validation
cmake --build . --target validate
```

## Programming RP2040-Zero

### Method 1: UF2 (Recommended)
1. Hold **BOOTSEL** button while connecting USB
2. Device appears as mass storage device (RPI-RP2)
3. Copy `facp_izone.uf2` to the mounted drive
4. Device automatically reboots and runs firmware

### Method 2: SWD Debug Interface
1. Connect SWD debugger (Pico Probe, J-Link, etc.)
2. Use OpenOCD or similar tool:
   ```bash
   openocd -f interface/picoprobe.cfg -f target/rp2040.cfg
   gdb-multiarch facp_izone.elf
   ```

## Troubleshooting

### SDK Not Found
```
Error: Directory 'C:\Program Files\Raspberry Pi\Pico SDK v1.5.1' not found
```

**Solutions:**
1. Install official Pico SDK from [Pico Setup Windows](https://github.com/raspberrypi/pico-setup-windows)
2. Set `PICO_SDK_PATH` environment variable manually
3. Clone SDK manually: `git clone https://github.com/raspberrypi/pico-sdk.git`

### CMake Configuration Fails
```
CMake Error: Could not find cmake module file: pico_sdk_init.cmake
```

**Solutions:**
1. Verify `PICO_SDK_PATH` points to valid SDK installation
2. Check that `pico_sdk_init.cmake` exists in SDK directory
3. Ensure SDK submodules are initialized: `git submodule update --init`

### Build Fails with Missing Tools
```
Error: Missing required tools: gcc-arm-none-eabi
```

**Windows Solutions:**
```bash
# Using Scoop
scoop install gcc-arm-none-eabi

# Using Chocolatey  
choco install gcc-arm-embedded
```

**Linux Solutions:**
```bash
# Ubuntu/Debian
sudo apt install gcc-arm-none-eabi libnewlib-arm-none-eabi

# CentOS/RHEL
sudo yum install gcc-arm-none-eabi-newlib
```

**macOS Solutions:**
```bash
# Using Homebrew
brew install gcc-arm-embedded
```

### Stack Usage Warnings
```
warning: 'function_name' uses 2048 bytes of stack space [-Wstack-usage=]
```

This is expected for fire safety validation. Review function to optimize stack usage if needed.

## Development Workflow

1. **Initial Setup**:
   ```bash
   git submodule update --init --recursive
   ```

2. **Development Cycle**:
   ```bash
   # Edit source files
   ./build.sh debug        # Test build
   # Test on hardware
   ./build.sh              # Release build for deployment
   ```

3. **Fire Safety Validation**:
   ```bash
   cmake --build build --target validate
   ```

## Memory Usage Guidelines

Target memory constraints for fire safety requirements:
- **Flash**: <2MB (RP2040 has 2MB)
- **RAM**: <256KB (RP2040 has 264KB)
- **Stack per task**: <2KB (monitored by compiler)

Use `cmake --build build --target size` to monitor usage.

## Integration with FreeRTOS

The build system is configured for FreeRTOS SMP operation:
- Dual-core Cortex-M0+ utilization
- Real-time task scheduling
- 1ms tick resolution
- Stack overflow detection
- Memory protection features

This ensures the <100ms fire detection response requirement is met.

## Fire Safety Compliance

The build configuration supports fire safety standards:
- **Deterministic build**: Reproducible binaries
- **Error detection**: Comprehensive compiler warnings
- **Stack monitoring**: Prevents overflow conditions
- **Memory optimization**: Minimal resource usage
- **Real-time guarantees**: Task priority management

## Next Steps

After successful build, proceed with:
1. Hardware testing on RP2040-Zero
2. FreeRTOS task validation
3. Sensor integration testing
4. I2C communication verification
5. System integration with building controller 