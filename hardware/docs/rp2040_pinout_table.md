# RP2040 Pinout Table - Fire Alarm Control Panel
## Hardware Pin Assignment Documentation

**Version:** 1.0  
**Date:** December 2024  
**Purpose:** Firmware development reference  
**Hardware:** Fire Alarm Zone Card with RP2040  

---

## Pin Assignment Summary

Based on the implemented schematic, this table documents all RP2040 GPIO connections for firmware development reference.

### 📋 **Complete Pinout Table**

| GPIO Pin | Physical Pin | Function | Connected To | Direction | Notes |
|----------|--------------|----------|--------------|-----------|--------|
| GPIO0 | 1 | UART0 TX | Debug/Programming | Output | Serial communication |
| GPIO1 | 2 | UART0 RX | Debug/Programming | Input | Serial communication |
| GPIO2 | 4 | I2C1 SDA | I2C Communication | I/O | Master/Slave communication |
| GPIO3 | 5 | I2C1 SCL | I2C Communication | Output | Clock line |
| GPIO4 | 6 | Fire Zone 1 Input | TCMT4600 Output | Input | Optocoupler sensor input |
| GPIO5 | 7 | Fire Zone 2 Input | TCMT4600 Output | Input | Optocoupler sensor input |
| GPIO6 | 8 | Fault Zone 1 Input | TCMT4600 Output | Input | Optocoupler fault detection |
| GPIO7 | 9 | Fault Zone 2 Input | TCMT4600 Output | Input | Optocoupler fault detection |
| GPIO8 | 11 | LED Status 1 | Status LED | Output | Fire zone 1 indicator |
| GPIO9 | 12 | LED Status 2 | Status LED | Output | Fire zone 2 indicator |
| GPIO10 | 13 | LED Fault 1 | Fault LED | Output | Fault zone 1 indicator |
| GPIO11 | 14 | LED Fault 2 | Fault LED | Output | Fault zone 2 indicator |
| GPIO12 | 15 | Test Switch 1 | SW1 | Input | Zone 1 test input |
| GPIO13 | 16 | Test Switch 2 | SW2 | Input | Zone 2 test input |
| GPIO14 | 17 | Alarm Output 1 | External Alarm | Output | Zone 1 alarm trigger |
| GPIO15 | 18 | Alarm Output 2 | External Alarm | Output | Zone 2 alarm trigger |
| GPIO16 | 21 | Additional Input | Expansion | Input | Future use |
| GPIO17 | 22 | Additional Output | Expansion | Output | Future use |
| GPIO18 | 23 | SPI SCK | Programming/Debug | Output | SPI clock (if used) |
| GPIO19 | 24 | SPI MOSI | Programming/Debug | Output | SPI data out |
| GPIO20 | 25 | SPI MISO | Programming/Debug | Input | SPI data in |
| GPIO21 | 26 | SPI CS | Programming/Debug | Output | SPI chip select |
| GPIO22 | 27 | Power Good | Power Monitor | Input | Power status monitoring |
| GPIO26 | 31 | ADC0 | Analog Input | Input | Analog sensor reading |
| GPIO27 | 32 | ADC1 | Analog Input | Input | Analog sensor reading |
| GPIO28 | 34 | ADC2 | Analog Input | Input | Analog sensor reading |

### 🔧 **Special Function Pins**

| Pin Name | Physical Pin | Function | Notes |
|----------|--------------|----------|--------|
| SWCLK | 41 | SWD Clock | Programming/Debug interface |
| SWDIO | 43 | SWD Data | Programming/Debug interface |
| RUN | 30 | Reset | System reset (active low) |
| 3V3 | 36, 3 | Power | 3.3V supply from LT8640S-3V3 |
| GND | Multiple | Ground | System ground reference |
| VBUS | 40 | USB Power | 5V (if USB connected) |
| VSYS | 39 | System Power | Main power input |

### 🌐 **Communication Interfaces**

#### I2C Interface
```
GPIO2 (SDA) - I2C Data Line
GPIO3 (SCL) - I2C Clock Line
Purpose: Master/Slave communication with building controller
Pull-up resistors: External (typically 4.7kΩ)
```

#### UART Interface  
```
GPIO0 (TX) - Transmit Data
GPIO1 (RX) - Receive Data
Purpose: Debug/Programming/Monitoring
Baud Rate: Configurable (typically 115200)
```

#### SWD Programming Interface
```
SWCLK - Programming Clock
SWDIO - Programming Data
Purpose: Firmware upload and debugging
Tool: Raspberry Pi Pico Probe or similar
```

### 🔥 **Fire Detection Inputs**

#### Zone Sensor Inputs (Optically Isolated)
```
GPIO4 - Fire Zone 1 (via TCMT4600 optocoupler)
GPIO5 - Fire Zone 2 (via TCMT4600 optocoupler)
GPIO6 - Fault Zone 1 (via TCMT4600 optocoupler)
GPIO7 - Fault Zone 2 (via TCMT4600 optocoupler)

Input Characteristics:
- Optical isolation provides safety and noise immunity
- Logic levels: 0V (inactive) / 3.3V (active)
- Response time: <1ms typical
- Protection: Built-in via optocoupler design
```

#### Test Inputs
```
GPIO12 - Test Switch Zone 1
GPIO13 - Test Switch Zone 2

Input Characteristics:
- Active low (pulled high when not pressed)
- Debouncing required in software
- Used for manual system testing
```

### 💡 **Status Indicators**

#### LED Outputs
```
GPIO8  - Fire Zone 1 Status LED (Red)
GPIO9  - Fire Zone 2 Status LED (Red)
GPIO10 - Fault Zone 1 LED (Yellow)
GPIO11 - Fault Zone 2 LED (Yellow)

Output Characteristics:
- Current limiting via external resistors
- Logic high = LED ON
- Typical current: ~10mA per LED
- Visibility: Industrial environment rated
```

#### Alarm Outputs
```
GPIO14 - Zone 1 Alarm Output
GPIO15 - Zone 2 Alarm Output

Output Characteristics:
- Can drive external alarm devices
- Logic levels: 0V/3.3V
- Maximum current: Per RP2040 specifications
- Protection: External circuits as needed
```

### 📊 **Analog Inputs**

#### ADC Channels
```
GPIO26 (ADC0) - Analog Sensor Input 1
GPIO27 (ADC1) - Analog Sensor Input 2  
GPIO28 (ADC2) - Analog Sensor Input 3

ADC Characteristics:
- Resolution: 12-bit (0-4095)
- Reference: 3.3V (VREF)
- Sample rate: Configurable
- Use: Analog sensor monitoring
```

### ⚡ **Power Management**

#### Power Monitoring
```
GPIO22 - Power Good Signal
Input from LT8640S-3V3 regulator
Purpose: Monitor power supply health
Logic: High = Power OK, Low = Power fault
```

### 🔧 **Firmware Development Notes**

#### Pin Configuration Recommendations
```c
// Fire sensor inputs (with pull-up)
gpio_init(4); gpio_set_dir(4, GPIO_IN); gpio_pull_up(4);
gpio_init(5); gpio_set_dir(5, GPIO_IN); gpio_pull_up(5);
gpio_init(6); gpio_set_dir(6, GPIO_IN); gpio_pull_up(6);
gpio_init(7); gpio_set_dir(7, GPIO_IN); gpio_pull_up(7);

// LED outputs
gpio_init(8); gpio_set_dir(8, GPIO_OUT);
gpio_init(9); gpio_set_dir(9, GPIO_OUT);
gpio_init(10); gpio_set_dir(10, GPIO_OUT);
gpio_init(11); gpio_set_dir(11, GPIO_OUT);

// Test switches (with pull-up)
gpio_init(12); gpio_set_dir(12, GPIO_IN); gpio_pull_up(12);
gpio_init(13); gpio_set_dir(13, GPIO_IN); gpio_pull_up(13);

// Alarm outputs
gpio_init(14); gpio_set_dir(14, GPIO_OUT);
gpio_init(15); gpio_set_dir(15, GPIO_OUT);

// I2C interface
i2c_init(i2c1, 100000); // 100kHz
gpio_set_function(2, GPIO_FUNC_I2C); // SDA
gpio_set_function(3, GPIO_FUNC_I2C); // SCL
```

#### Interrupt Priorities
```
Fire Sensor Inputs: Highest priority (GPIO 4-7)
Test Switches: Medium priority (GPIO 12-13)
Communication: Normal priority (I2C, UART)
Status Updates: Low priority (LEDs, monitoring)
```

### 📝 **Usage Guidelines**

#### Critical Functions
- **Fire Detection**: GPIO4-7 must be monitored continuously
- **Response Time**: <100ms from sensor input to alarm output
- **Safety**: Always fail to alarm state on error conditions
- **Testing**: GPIO12-13 for manual system verification

#### Development Sequence
1. **Basic GPIO**: Configure and test LED outputs
2. **Sensor Inputs**: Implement optocoupler reading
3. **Communication**: Setup I2C slave protocol
4. **Fire Logic**: Implement detection algorithms
5. **Testing**: Add test switch functionality
6. **Integration**: Full system testing

### 🚨 **Safety Considerations**

#### Fail-Safe Design
- Input monitoring must be continuous
- Default to alarm state on any fault
- Watchdog timer implementation required
- Power monitoring for system health

#### EMC Compliance
- Optical isolation protects against noise
- Proper grounding via hardware design
- Software filtering for input debouncing
- Communication error handling

---

**Document Status: Complete**  
**Next Step: Use this table for FreeRTOS task development**  
**Validation: Test each pin assignment during hardware validation** 