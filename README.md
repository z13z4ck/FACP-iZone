# iZone Fire Alarm Control Panel

A distributed fire alarm control panel system built with RP2040-Zero microcontrollers, featuring intelligent zone cards with GSM connectivity for remote monitoring.

## Overview

The iZone system provides a scalable, cost-effective fire detection solution using:
- **Zone Cards**: RP2040-Zero based intelligent sensor monitoring units
- **Building Controller**: Coordinates zone cards via I2C and manages GSM communication
- **GSM Module**: SIM9000A Mini v3.8.3 for building-to-building connectivity
- **FreeRTOS**: Real-time operating system for reliable operation

## Key Features

- ⚡ **Fast Response**: <100ms fire detection response time
- 🌐 **Multi-Building Monitoring**: GSM-based building-to-building communication
- 📡 **Scalable**: Support for up to 32 zone cards per building controller via I2C
- 🔗 **Reliable Communication**: I2C for local zones, GSM for building-to-building
- 🔍 **Self-Diagnostics**: Comprehensive health monitoring
- 🔋 **Low Power**: <100mA consumption per zone card

## System Architecture

```
Building A                           Building B                        Main Monitoring
┌─────────────────┐                ┌─────────────────┐                    Building
│    Zone Card    │                │    Zone Card    │               ┌─────────────────┐
│   (RP2040-Zero) │                │   (RP2040-Zero) │               │   Main Control  │
│   + Sensors     │                │   + Sensors     │               │     Center      │
│   + LEDs        │                │   + LEDs        │               │                 │
└─────────┬───────┘                └─────────┬───────┘               │  + Monitoring   │
          │ I2C                              │ I2C                   │  + Management   │
┌─────────┼───────┐                 ┌────────┼───────┐               │  + Reports      │
│   Building      │                 │   Building     │               └─────────┬───────┘
│   Controller    │◄──────GSM──────►│   Controller   │                         │
│  (RP2040-Zero)  │     Network     │  (RP2040-Zero) │◄────────GSM Network─────┘
│ + SIM9000A GSM  │                 │ + SIM9000A GSM │
│ + USB Interface │                 │ + USB Interface│
└─────────┬───────┘                 └────────┬───────┘
          │                                  │
          ▼                                  ▼
┌─────────────────┐                 ┌────────────────┐
│ PC/Laptop with  │                 │ PC/Laptop with │
│ GUI Config Tool │                 │ GUI Config Tool│
│ - Zone ID Setup │                 │ - Zone ID Setup│
│ - Programming   │                 │ - Programming  │
│ - Diagnostics   │                 │ - Diagnostics  │
└─────────────────┘                 └────────────────┘
```

## Project Structure

```
FACP-iZone/
├── .taskmaster/                 # Task management system
│   ├── docs/
│   │   └── prd.txt             # Product Requirements Document
│   ├── tasks/
│   │   ├── tasks.json          # Master task list
│   │   └── task_*.txt          # Individual task files
│   └── config.json             # Taskmaster configuration
├── PRD.md                      # Detailed Product Requirements Document
└── README.md                   # This file
```

## Development Workflow

This project uses [Task Master AI](https://www.npmjs.com/package/task-master-ai) for structured development workflow:

### Getting Started

1. **Check Current Tasks**
   ```bash
   # View all available tasks
   npx task-master-ai list
   
   # Find the next task to work on
   npx task-master-ai next
   ```

2. **Working on Tasks**
   ```bash
   # Mark a task as in progress
   npx task-master-ai set-status --id=1 --status=in-progress
   
   # View detailed task information
   npx task-master-ai show 1
   ```

3. **Task Completion**
   ```bash
   # Mark task as complete
   npx task-master-ai set-status --id=1 --status=done
   ```

### Development Phases

1. **Phase 1: Foundation (Tasks 1-4)**
   - Hardware platform setup
   - FreeRTOS integration  
   - Core sensor monitoring
   - LED status system

2. **Phase 2: Communication (Tasks 5-9)**
   - Master controller hardware
   - Communication protocol
   - Multi-zone operation
   - Alarm coordination

3. **Phase 3: GSM Integration (Tasks 10-12)**
   - GSM module integration
   - Remote notifications
   - Network management

4. **Phase 4: Configuration & Integration (Tasks 13-16)**
   - USB communication protocol
   - GUI configuration tool
   - System diagnostics
   - Comprehensive testing

## Hardware Requirements

### Zone Card
- **Microcontroller**: RP2040-Zero
- **Sensors**: Fire detection sensors (smoke, heat, gas)
- **Indicators**: Status LEDs (Power, Normal, Alarm, Fault)
- **Communication**: I2C slave interface with configurable address
- **Power**: 12V DC input, <100mA consumption

### Building Controller
- **Microcontroller**: RP2040-Zero (enhanced variant)
- **GSM Module**: SIM9000A Mini v3.8.3 (for building-to-building communication)
- **I2C Master**: Interface for up to 32 zone cards within building
- **USB Interface**: CDC interface for GUI configuration tool
- **Storage**: EEPROM/Flash for configuration
- **Power**: 12V DC with battery backup

### GUI Configuration Tool
- **Platform**: Cross-platform PC/Laptop application (Windows/Linux/Mac)
- **Connection**: USB CDC interface to building controller
- **Features**: Zone ID assignment, board programming, system configuration
- **Diagnostics**: Real-time monitoring and troubleshooting
- **Backup**: Configuration backup and restore capabilities

## Software Architecture

### FreeRTOS Task Structure
- **Sensor Task**: Monitor and process sensor inputs
- **Communication Task**: Handle master-slave protocol
- **Alarm Task**: Control alarm outputs and LEDs
- **Diagnostics Task**: Self-tests and health monitoring
- **GSM Task**: (Master only) Manage GSM communications

### Development Tools
- **SDK**: RP2040 SDK with CMake build system
- **RTOS**: FreeRTOS kernel
- **Toolchain**: arm-none-eabi-gcc cross-compiler
- **Debug**: SWD interface with logic analyzer support

## Performance Specifications

| Metric | Target | Measurement |
|--------|--------|-------------|
| Sensor Response Time | <100ms | Sensor trigger to zone response |
| System Response Time | <1 second | Sensor trigger to master notification |
| GSM Notification Time | <30 seconds | Alarm to remote notification |
| Zone Polling Rate | 1 Hz minimum | Building controller I2C frequency |
| Communication Reliability | >99% | Successful message delivery |
| System Uptime | >99.9% | Continuous operation |

## Safety & Compliance

- **Operating Temperature**: -10°C to +60°C
- **Humidity**: 5% to 95% RH non-condensing
- **Standards**: NFPA 72, UL 864, EN 54 compliance
- **EMC**: FCC Part 15 Class B, EN 55032
- **Power**: IEC 61000-4-5 Level 3 surge protection

## Contributing

1. Check the task list for available work items
2. Select a task based on dependencies and priority
3. Follow the test strategy defined for each task
4. Update task status as work progresses
5. Document any changes or deviations from the plan

## License

[Specify your license here]

## Support

For technical questions or support, please refer to the detailed PRD document and individual task files in the `.taskmaster/` directory. 