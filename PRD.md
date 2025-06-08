# Fire Alarm Control Panel - iZone System
## Product Requirements Document (PRD)

**Version:** 1.0  
**Date:** December 2024  
**Product Owner:** Fire Safety Systems Team  
**Status:** Initial Draft  

---

## 1. Executive Summary

The iZone Fire Alarm Control Panel is a distributed fire detection and alarm system built around RP2040-Zero microcontrollers operating as intelligent zone cards. The system employs a master-slave architecture with GSM connectivity for remote monitoring and notifications, providing a scalable, cost-effective solution for fire safety applications.

## 2. Product Vision

To create a reliable, scalable, and cost-effective fire alarm control panel system that provides real-time fire detection, immediate local alarms, and remote monitoring capabilities using modern microcontroller technology.

## 3. Business Objectives

- **Primary:** Develop a distributed fire alarm system with remote monitoring capabilities
- **Secondary:** Reduce hardware costs through efficient use of RP2040-Zero microcontrollers
- **Tertiary:** Ensure compliance with fire safety standards and regulations

## 4. Target Users

- **Primary Users:** Building owners, facility managers, security personnel
- **Secondary Users:** Fire departments, monitoring service providers
- **Technical Users:** System installers, maintenance technicians

## 5. System Overview

### 5.1 Architecture
The iZone system consists of:
- **Zone Cards:** RP2040-Zero based intelligent sensor monitoring units
- **Master Controller:** Coordinates zone cards and manages GSM communication  
- **GSM Module:** SIM9000A Mini v3.8.3 for remote connectivity
- **Sensors:** Fire detection sensors (smoke, heat, gas, etc.)
- **Indicators:** LED status indicators and alarm outputs

### 5.2 Key Features
- Real-time sensor monitoring
- Distributed processing with master-slave communication
- GSM-based remote notifications
- FreeRTOS for reliable real-time operation
- Scalable zone architecture
- Local and remote alarm management

## 6. Functional Requirements

### 6.1 Zone Card Requirements (FR-ZC)

| ID | Requirement | Priority | Acceptance Criteria |
|----|-------------|----------|-------------------|
| FR-ZC-001 | Sensor Monitoring | Critical | Zone card must monitor connected fire sensors with <100ms response time |
| FR-ZC-002 | Local Alarm Control | Critical | Zone card must activate local alarms when fire conditions detected |
| FR-ZC-003 | Status Indication | High | Zone card must provide visual status via LEDs (normal, alarm, fault) |
| FR-ZC-004 | Communication | Critical | Zone card must communicate with master controller via defined protocol |
| FR-ZC-005 | Self-Diagnostics | High | Zone card must perform self-tests and report health status |
| FR-ZC-006 | Configuration | Medium | Zone card must support field configuration via master controller |

### 6.2 Building Controller Requirements (FR-BC)

| ID | Requirement | Priority | Acceptance Criteria |
|----|-------------|----------|-------------------|
| FR-BC-001 | Zone Coordination | Critical | Building controller must poll all zone cards via I2C within 1 second cycle |
| FR-BC-002 | GSM Communication | Critical | Building controller must send notifications to main building via GSM within 30 seconds |
| FR-BC-003 | System Management | High | Building controller must provide building status via USB interface for GUI configuration tool |
| FR-BC-004 | Data Logging | Medium | Building controller must log events with timestamp and zone identification |
| FR-BC-005 | Fault Management | High | Building controller must detect and report zone card failures via I2C |

### 6.3 Communication Requirements (FR-COM)

| ID | Requirement | Priority | Acceptance Criteria |
|----|-------------|----------|-------------------|
| FR-COM-001 | I2C Protocol Reliability | Critical | I2C communication must have <1% message loss rate within building |
| FR-COM-002 | Message Types | High | I2C protocol must support status, alarm, configuration, and diagnostic messages |
| FR-COM-003 | Zone Addressing | High | I2C protocol must support up to 32 zone cards per building controller |
| FR-COM-004 | Error Handling | High | I2C protocol must include error detection and recovery mechanisms |
| FR-COM-005 | Building-to-Building GSM | Critical | GSM communication must enable reliable inter-building data exchange |

### 6.4 GSM Module Requirements (FR-GSM)

| ID | Requirement | Priority | Acceptance Criteria |
|----|-------------|----------|-------------------|
| FR-GSM-001 | Connectivity | Critical | GSM module must maintain network connection with >95% uptime |
| FR-GSM-002 | Building Notifications | Critical | Module must send building-to-building notifications for alarm events |
| FR-GSM-003 | Remote Configuration | Medium | Module must support remote configuration updates from main building |
| FR-GSM-004 | Network Fallback | Medium | Module must support multiple network operators |
| FR-GSM-005 | Multi-Building Support | High | System must support communication between main monitoring building and remote buildings |

### 6.5 GUI Configuration Tool Requirements (FR-GUI)

| ID | Requirement | Priority | Acceptance Criteria |
|----|-------------|----------|-------------------|
| FR-GUI-001 | USB Communication | Critical | GUI tool must communicate reliably with building controller via USB CDC interface |
| FR-GUI-002 | Zone ID Assignment | High | GUI tool must provide graphical interface for assigning zone card IDs |
| FR-GUI-003 | Firmware Programming | High | GUI tool must support board programming and firmware updates |
| FR-GUI-004 | System Configuration | High | GUI tool must allow configuration of sensor thresholds, alarm parameters, and GSM settings |
| FR-GUI-005 | Real-time Monitoring | Medium | GUI tool must provide real-time diagnostics and system monitoring |
| FR-GUI-006 | Backup/Restore | Medium | GUI tool must support configuration backup and restore functionality |
| FR-GUI-007 | Cross-Platform | Medium | GUI tool must run on Windows, macOS, and Linux operating systems |

## 7. Non-Functional Requirements

### 7.1 Performance Requirements (NFR-PERF)

| ID | Requirement | Target | Measurement |
|----|-------------|--------|-------------|
| NFR-PERF-001 | Sensor Response Time | <100ms | Time from sensor trigger to zone card response |
| NFR-PERF-002 | System Response Time | <1 second | Time from sensor trigger to master notification |
| NFR-PERF-003 | GSM Notification Time | <30 seconds | Time from alarm to remote notification |
| NFR-PERF-004 | Zone Card Polling Rate | 1 Hz minimum | Building controller I2C polling frequency |

### 7.2 Reliability Requirements (NFR-REL)

| ID | Requirement | Target | Measurement |
|----|-------------|--------|-------------|
| NFR-REL-001 | System Uptime | >99.9% | Continuous operation time |
| NFR-REL-002 | False Alarm Rate | <0.1% | False alarms per sensor per year |
| NFR-REL-003 | Communication Reliability | >99% | Successful message delivery rate |
| NFR-REL-004 | Power Consumption | <100mA per zone | Average current consumption |

### 7.3 Safety Requirements (NFR-SAFE)

| ID | Requirement | Compliance |
|----|-------------|------------|
| NFR-SAFE-001 | Fail-Safe Operation | System must default to alarm state on failure |
| NFR-SAFE-002 | Redundancy | Critical functions must have backup mechanisms |
| NFR-SAFE-003 | Environmental | System must operate in -10°C to +60°C range |
| NFR-SAFE-004 | EMC Compliance | System must meet electromagnetic compatibility standards |

## 8. Hardware Specifications

### 8.1 Zone Card Hardware
- **Microcontroller:** RP2040-Zero
- **Sensors:** Configurable fire detection sensors
- **Indicators:** Status LEDs (Power, Normal, Alarm, Fault)
- **Communication:** Master-slave protocol interface
- **Power:** 12V DC input with local regulation
- **Enclosure:** Fire-rated housing suitable for zone installation

### 8.2 Building Controller Hardware
- **Microcontroller:** RP2040-Zero (enhanced variant or additional processing)
- **GSM Module:** SIM9000A Mini v3.8.3 (for building-to-building communication)
- **I2C Master:** Interface for up to 32 zone cards within building
- **USB Interface:** CDC interface for GUI configuration tool connection
- **Storage:** Non-volatile memory for configuration and logs
- **Power:** Primary and backup power supplies

### 8.3 GUI Configuration Tool
- **Platform:** Cross-platform PC/Laptop application (Windows/Linux/Mac)
- **Connection:** USB CDC interface to building controller
- **Features:** Zone ID assignment, board programming, system configuration
- **Diagnostics:** Real-time monitoring and troubleshooting capabilities
- **Backup:** Configuration backup and restore functionality

### 8.4 Communication Interface
- **Local Building Communication:** I2C bus for zone communication within building
- **Topology:** I2C master-slave with building controller polling
- **I2C Speed:** 100kHz standard mode, upgradeable to 400kHz fast mode
- **Building-to-Building:** GSM network using SIM9000A modules
- **Distance:** I2C with bus extenders for building coverage, GSM for unlimited range

## 9. Software Architecture

### 9.1 Zone Card Software Stack
```
┌─────────────────────────┐
│   Application Layer     │
│  - Sensor Management    │
│  - Alarm Control        │
│  - Communication        │
├─────────────────────────┤
│     FreeRTOS RTOS       │
├─────────────────────────┤
│   Hardware Abstraction  │
│      (RP2040 SDK)       │
└─────────────────────────┘
```

### 9.2 Building Controller Software Stack
```
┌─────────────────────────┐
│   Application Layer     │
│  - System Coordination  │
│  - GSM Management       │
│  - USB Communication    │
├─────────────────────────┤
│     FreeRTOS RTOS       │
├─────────────────────────┤
│   Hardware Abstraction  │
│    (RP2040 SDK + GSM)   │
└─────────────────────────┘
```

### 9.3 Task Architecture
- **Sensor Task:** Monitor and process sensor inputs
- **Communication Task:** Handle master-slave protocol
- **Alarm Task:** Control alarm outputs and LEDs
- **Diagnostics Task:** Perform self-tests and health monitoring
- **GSM Task:** (Building Controller only) Manage GSM communications
- **USB Task:** (Building Controller only) Handle GUI configuration tool communication

## 10. Development Phases

### Phase 1: Foundation (Weeks 1-4)
- [ ] Hardware prototype development
- [ ] Basic FreeRTOS setup on RP2040-Zero
- [ ] Core sensor monitoring functionality
- [ ] Initial LED control implementation

### Phase 2: Communication (Weeks 5-8)
- [ ] Master-slave protocol design and implementation
- [ ] Multi-zone communication testing
- [ ] Basic alarm coordination between zones
- [ ] Error handling and recovery mechanisms

### Phase 3: GSM Integration (Weeks 9-12)
- [ ] SIM9000A module integration
- [ ] GSM communication protocols
- [ ] Remote notification system
- [ ] Network connectivity management

### Phase 4: Configuration Tools & Integration (Weeks 13-16)
- [ ] USB communication protocol implementation
- [ ] GUI configuration tool development
- [ ] Full system testing
- [ ] Performance optimization
- [ ] Safety and reliability testing
- [ ] Documentation and user manuals

### Phase 5: Certification & Deployment (Weeks 17-20)
- [ ] Compliance testing
- [ ] Field testing and validation
- [ ] Manufacturing preparation
- [ ] Deployment procedures

## 11. Success Metrics

### 11.1 Technical Metrics
- **Response Time:** <100ms sensor to zone card response
- **Reliability:** >99.9% system uptime
- **Scalability:** Support for 32 zones per master
- **Communication:** <1% message loss rate

### 11.2 Business Metrics
- **Cost Reduction:** 30% lower cost vs. traditional systems
- **Installation Time:** 50% faster installation vs. conventional systems
- **Maintenance:** Remote diagnostics reducing service calls by 40%

## 12. Risk Assessment

| Risk | Impact | Probability | Mitigation |
|------|--------|-------------|------------|
| RP2040 supply shortage | High | Medium | Identify alternative microcontrollers |
| GSM network reliability | High | Low | Implement multiple carrier support |
| Sensor compatibility | Medium | Medium | Extensive sensor testing program |
| Regulatory compliance | High | Low | Early engagement with certification bodies |
| Power consumption | Medium | Medium | Optimize software and hardware design |

## 13. Dependencies

### 13.1 Hardware Dependencies
- RP2040-Zero availability and pricing
- SIM9000A Mini v3.8.3 module availability
- Fire sensor compatibility and certification
- PCB manufacturing and assembly capabilities

### 13.2 Software Dependencies
- FreeRTOS compatibility with RP2040
- RP2040 SDK stability and features
- GSM module firmware and AT command set
- Development toolchain (GCC, CMake, etc.)

### 13.3 External Dependencies
- Cellular network coverage and reliability
- Fire safety regulation compliance
- Component supply chain stability
- Testing and certification facilities

## 14. Acceptance Criteria

### 14.1 System Level
- [ ] System detects fire conditions within 100ms
- [ ] All zone cards communicate reliably with master
- [ ] GSM notifications sent within 30 seconds of alarm
- [ ] System operates continuously for 30 days without failure
- [ ] False alarm rate <0.1% over 1000 hours of operation

### 14.2 Zone Card Level
- [ ] Zone card monitors sensors at 10Hz minimum
- [ ] LED indicators accurately reflect system status
- [ ] Self-diagnostics detect 95% of hardware faults
- [ ] Power consumption <100mA average
- [ ] Operating temperature range -10°C to +60°C

### 14.3 Master Controller Level
- [ ] Polls all zone cards within 1 second cycle
- [ ] GSM module maintains network connection >95% uptime
- [ ] Event logging with accurate timestamps
- [ ] USB interface allows field setup via GUI configuration tool
- [ ] Fault detection and isolation capabilities

---

## Document History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | Dec 2024 | Development Team | Initial PRD creation |

---

## Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Product Owner | | | |
| Technical Lead | | | |
| Project Manager | | | | 