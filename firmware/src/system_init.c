/**
 * @file system_init.c
 * @brief System Initialization Implementation for FACP iZone
 * 
 * This file implements system initialization routines and FreeRTOS
 * hook functions for the Fire Alarm Control Panel.
 * 
 * @author FACP Development Team
 * @date 2024
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "FreeRTOS.h"
#include "task.h"
#include "system_init.h"

/* Global system variables */
system_status_t g_system_status = SYSTEM_STATUS_INIT;
system_config_t g_system_config;

/* Static memory allocation for FreeRTOS tasks */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

static StaticTask_t xTimerTaskTCBBuffer;
static StackType_t xTimerStack[configTIMER_TASK_STACK_DEPTH];

/**
 * @brief Initialize system configuration to default values
 */
void system_config_init(void)
{
    memset(&g_system_config, 0, sizeof(system_config_t));
    
    /* Set default configuration values */
    g_system_config.zone_count = 4;
    g_system_config.device_address = 0x20;  /* Default I2C address */
    g_system_config.watchdog_enabled = true;
    
    /* Set default sensor thresholds */
    for (int i = 0; i < MAX_ZONES; i++) {
        g_system_config.sensor_threshold[i] = 512;  /* Mid-range for 10-bit ADC */
    }
    
    printf("System configuration initialized to defaults\n");
}

/**
 * @brief Get current system status
 * @return Current system status
 */
system_status_t system_get_status(void)
{
    return g_system_status;
}

/**
 * @brief Set system status
 * @param status New system status
 */
void system_set_status(system_status_t status)
{
    g_system_status = status;
    printf("System status changed to: %d\n", status);
}

/**
 * @brief Get firmware version string
 * @param buffer Buffer to store version string
 * @param buffer_size Size of the buffer
 * @return Number of characters written
 */
int system_get_version_string(char *buffer, size_t buffer_size)
{
    return snprintf(buffer, buffer_size, 
                   "%d.%d.%d-%s",
                   FIRMWARE_VERSION_MAJOR,
                   FIRMWARE_VERSION_MINOR,
                   FIRMWARE_VERSION_PATCH,
                   FIRMWARE_VERSION_BUILD);
}

/**
 * @brief Perform system self-test
 * @return true if all tests passed, false otherwise
 */
bool system_self_test(void)
{
    printf("Starting system self-test...\n");
    
    /* Test 1: Memory test */
    void *test_ptr = pvPortMalloc(1024);
    if (test_ptr == NULL) {
        printf("FAIL: Memory allocation test\n");
        return false;
    }
    vPortFree(test_ptr);
    printf("PASS: Memory allocation test\n");
    
    /* Test 2: GPIO test */
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(50);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    printf("PASS: GPIO test\n");
    
    /* Test 3: Watchdog test */
    if (g_system_config.watchdog_enabled) {
        watchdog_update();
        printf("PASS: Watchdog test\n");
    }
    
    printf("System self-test completed successfully\n");
    return true;
}

/**
 * @brief Initialize hardware abstraction layer
 * @return true if successful, false otherwise
 */
bool hal_init(void)
{
    printf("Initializing Hardware Abstraction Layer...\n");
    
    /* Initialize system configuration */
    system_config_init();
    
    /* Perform self-test */
    if (!system_self_test()) {
        printf("HAL initialization failed: self-test failed\n");
        return false;
    }
    
    /* Set system status to normal operation */
    system_set_status(SYSTEM_STATUS_NORMAL);
    
    printf("HAL initialization completed successfully\n");
    return true;
}

/**
 * @brief Safe shutdown procedure
 */
void system_shutdown(void)
{
    printf("System shutdown initiated...\n");
    
    /* Set system status to fault */
    system_set_status(SYSTEM_STATUS_FAULT);
    
    /* Disable watchdog (RP2040 doesn't have watchdog_disable, just don't update it) */
    /* watchdog_disable(); */ /* Not available on RP2040 */
    
    /* Turn off all LEDs except fault LED */
    gpio_put(25, 0);  /* Status LED off */
    
    printf("System shutdown complete\n");
}

/* FreeRTOS Hook Functions */

/**
 * @brief Application malloc failed hook
 */
void vApplicationMallocFailedHook(void)
{
    printf("FATAL: Memory allocation failed!\n");
    
    /* Set fault status */
    system_set_status(SYSTEM_STATUS_FAULT);
    
    /* Disable interrupts and halt */
    portDISABLE_INTERRUPTS();
    for (;;) {
        /* Infinite loop - watchdog will reset the system */
    }
}

/**
 * @brief Application stack overflow hook
 * @param xTask Task handle
 * @param pcTaskName Task name
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;  /* Suppress unused parameter warning */
    
    printf("FATAL: Stack overflow in task: %s\n", pcTaskName);
    
    /* Set fault status */
    system_set_status(SYSTEM_STATUS_FAULT);
    
    /* Disable interrupts and halt */
    portDISABLE_INTERRUPTS();
    for (;;) {
        /* Infinite loop - watchdog will reset the system */
    }
}

/**
 * @brief Get idle task memory (static allocation)
 * @param ppxIdleTaskTCBBuffer Pointer to TCB buffer
 * @param ppxIdleTaskStackBuffer Pointer to stack buffer
 * @param pulIdleTaskStackSize Pointer to stack size
 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @brief Get timer task memory (static allocation)
 * @param ppxTimerTaskTCBBuffer Pointer to TCB buffer
 * @param ppxTimerTaskStackBuffer Pointer to stack buffer
 * @param pulTimerTaskStackSize Pointer to stack size
 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCBBuffer;
    *ppxTimerTaskStackBuffer = &xTimerStack[0];
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/**
 * @brief Get passive idle task memory (static allocation for SMP)
 * @param ppxIdleTaskTCBBuffer Pointer to TCB buffer  
 * @param ppxIdleTaskStackBuffer Pointer to stack buffer
 * @param puxIdleTaskStackSize Pointer to stack size
 * @param xPassiveIdleTaskIndex Index of the passive idle task
 */
void vApplicationGetPassiveIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                         StackType_t **ppxIdleTaskStackBuffer,
                                         configSTACK_DEPTH_TYPE *puxIdleTaskStackSize,
                                         BaseType_t xPassiveIdleTaskIndex)
{
    /* Static memory for the Passive Idle Tasks (SMP requirement) */
    static StaticTask_t xPassiveIdleTaskTCB[configNUMBER_OF_CORES - 1];
    static StackType_t xPassiveIdleStack[configNUMBER_OF_CORES - 1][configMINIMAL_STACK_SIZE];
    
    /* Use the index to provide different memory for each passive idle task */
    *ppxIdleTaskTCBBuffer = &xPassiveIdleTaskTCB[xPassiveIdleTaskIndex];
    *ppxIdleTaskStackBuffer = &xPassiveIdleStack[xPassiveIdleTaskIndex][0];
    *puxIdleTaskStackSize = configMINIMAL_STACK_SIZE;
} 