/**
 * @file system_init.h
 * @brief System Initialization Header for FACP iZone
 * 
 * This header file contains function prototypes and definitions for 
 * system initialization routines used in the Fire Alarm Control Panel.
 * 
 * @author FACP Development Team
 * @date 2024
 */

#ifndef SYSTEM_INIT_H
#define SYSTEM_INIT_H

#include <stdint.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Include FreeRTOS configuration */
#include "FreeRTOSConfig.h"

/* System version information */
#define FIRMWARE_VERSION_MAJOR  1
#define FIRMWARE_VERSION_MINOR  0
#define FIRMWARE_VERSION_PATCH  0
#define FIRMWARE_VERSION_BUILD  "dev"

/* Hardware configuration */
#define HARDWARE_VERSION        "RP2040-Zero Fire Safety v1.0"
#define MAX_ZONES               4    /* Maximum number of fire zones supported */

/* System status definitions */
typedef enum {
    SYSTEM_STATUS_INIT = 0,
    SYSTEM_STATUS_NORMAL,
    SYSTEM_STATUS_ALARM,
    SYSTEM_STATUS_FAULT,
    SYSTEM_STATUS_TEST
} system_status_t;

/* Zone status definitions */
typedef enum {
    ZONE_STATUS_NORMAL = 0,
    ZONE_STATUS_ALARM,
    ZONE_STATUS_FAULT,
    ZONE_STATUS_DISABLED
} zone_status_t;

/* System configuration structure */
typedef struct {
    uint8_t zone_count;                 /* Number of configured zones */
    uint8_t device_address;             /* I2C slave address */
    bool watchdog_enabled;              /* Watchdog timer enable flag */
    uint16_t sensor_threshold[MAX_ZONES]; /* Sensor trigger thresholds */
} system_config_t;

/* Global system variables */
extern system_status_t g_system_status;
extern system_config_t g_system_config;

/* Function prototypes */

/**
 * @brief Initialize system configuration to default values
 */
void system_config_init(void);

/**
 * @brief Get current system status
 * @return Current system status
 */
system_status_t system_get_status(void);

/**
 * @brief Set system status
 * @param status New system status
 */
void system_set_status(system_status_t status);

/**
 * @brief Get firmware version string
 * @param buffer Buffer to store version string
 * @param buffer_size Size of the buffer
 * @return Number of characters written
 */
int system_get_version_string(char *buffer, size_t buffer_size);

/**
 * @brief Perform system self-test
 * @return true if all tests passed, false otherwise
 */
bool system_self_test(void);

/**
 * @brief Initialize hardware abstraction layer
 * @return true if successful, false otherwise
 */
bool hal_init(void);

/**
 * @brief Safe shutdown procedure
 */
void system_shutdown(void);

/* FreeRTOS hook functions */

/**
 * @brief Application malloc failed hook
 */
void vApplicationMallocFailedHook(void);

/**
 * @brief Application stack overflow hook
 * @param xTask Task handle
 * @param pcTaskName Task name
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName);

/**
 * @brief Get idle task memory (static allocation)
 * @param ppxIdleTaskTCBBuffer Pointer to TCB buffer
 * @param ppxIdleTaskStackBuffer Pointer to stack buffer
 * @param pulIdleTaskStackSize Pointer to stack size
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

/**
 * @brief Get timer task memory (static allocation)
 * @param ppxTimerTaskTCBBuffer Pointer to TCB buffer
 * @param ppxTimerTaskStackBuffer Pointer to stack buffer
 * @param pulTimerTaskStackSize Pointer to stack size
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize);

#endif /* SYSTEM_INIT_H */ 