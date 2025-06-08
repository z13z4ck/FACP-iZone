# RP2040 Quick Reference - Fire Alarm System
## Essential GPIO Pin Map

**Quick Access:** Critical pins for firmware development

---

## 🔥 **FIRE DETECTION (Priority 1)**
```
GPIO4  - Fire Zone 1 Input    (TCMT4600)
GPIO5  - Fire Zone 2 Input    (TCMT4600)  
GPIO6  - Fault Zone 1 Input   (TCMT4600)
GPIO7  - Fault Zone 2 Input   (TCMT4600)
```

## 🚨 **ALARM OUTPUTS (Priority 1)**
```
GPIO14 - Zone 1 Alarm Output
GPIO15 - Zone 2 Alarm Output
```

## 💡 **STATUS LEDS (Priority 2)**
```
GPIO8  - Fire Zone 1 LED (Red)
GPIO9  - Fire Zone 2 LED (Red)
GPIO10 - Fault Zone 1 LED (Yellow)  
GPIO11 - Fault Zone 2 LED (Yellow)
```

## 🌐 **COMMUNICATION (Priority 2)**
```
GPIO2  - I2C SDA (Master/Slave)
GPIO3  - I2C SCL (Master/Slave)
GPIO0  - UART TX (Debug)
GPIO1  - UART RX (Debug)
```

## 🧪 **TESTING (Priority 3)**
```
GPIO12 - Test Switch Zone 1
GPIO13 - Test Switch Zone 2
GPIO22 - Power Good Monitor
```

## ⚡ **PROGRAMMING**
```
SWCLK  - SWD Clock (Pin 41)
SWDIO  - SWD Data (Pin 43)
RUN    - Reset (Pin 30)
```

---

## 🔧 **Quick Code Templates**

### Fire Detection Setup
```c
// Configure fire sensor inputs
gpio_init(4); gpio_set_dir(4, GPIO_IN); gpio_pull_up(4);
gpio_init(5); gpio_set_dir(5, GPIO_IN); gpio_pull_up(5);
gpio_init(6); gpio_set_dir(6, GPIO_IN); gpio_pull_up(6);
gpio_init(7); gpio_set_dir(7, GPIO_IN); gpio_pull_up(7);
```

### LED Control Setup
```c
// Configure status LEDs
gpio_init(8); gpio_set_dir(8, GPIO_OUT);   // Fire Zone 1
gpio_init(9); gpio_set_dir(9, GPIO_OUT);   // Fire Zone 2
gpio_init(10); gpio_set_dir(10, GPIO_OUT); // Fault Zone 1
gpio_init(11); gpio_set_dir(11, GPIO_OUT); // Fault Zone 2
```

### I2C Communication Setup
```c
// Configure I2C interface
i2c_init(i2c1, 100000); // 100kHz
gpio_set_function(2, GPIO_FUNC_I2C); // SDA
gpio_set_function(3, GPIO_FUNC_I2C); // SCL
```

---

**💡 Remember:** Fire detection inputs (GPIO4-7) are highest priority!  
**⚠️ Safety:** Always fail to alarm state on any error condition. 