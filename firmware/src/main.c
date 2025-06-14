/**
 * @file main.c
 * @brief FACP iZone Fire Alarm Control Panel - Main Application
 * 
 * This file implements the main application entry point for the Fire Alarm
 * Control Panel iZone system running on RP2040-Zero with FreeRTOS.
 * 
 * Hardware: RP2040-Zero with custom fire safety peripherals
 * RTOS: FreeRTOS SMP for dual-core operation
 * 
 * @author FACP Development Team
 * @date 2024
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/watchdog.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

/* Project includes */
#include "system_init.h"

/* Pin definitions based on RP2040-Zero and custom hardware */
#define LED_STATUS_PIN      25      /* Built-in LED on RP2040-Zero */
#define LED_POWER_PIN       2       /* Power status LED */
#define LED_NORMAL_PIN      3       /* Normal operation LED */
#define LED_ALARM_PIN       4       /* Alarm status LED */
#define LED_FAULT_PIN       5       /* Fault status LED */

/* Task handles */
static TaskHandle_t xLedBlinkTaskHandle = NULL;
static TaskHandle_t xSystemMonitorTaskHandle = NULL;

/* Function prototypes */
static void prvLedBlinkTask(void *pvParameters);
static void prvSystemMonitorTask(void *pvParameters);
static void prvSetupHardware(void);

/**
 * @brief LED blink task for basic functionality test
 * 
 * This task blinks the status LED to indicate the system is running.
 * It demonstrates basic FreeRTOS task functionality.
 * 
 * @param pvParameters Task parameters (unused)
 */
static void prvLedBlinkTask(void *pvParameters)
{
    (void)pvParameters;  /* Suppress unused parameter warning */
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(500);  /* 500ms blink rate */
    
    printf("LED Blink Task started on core %d\n", get_core_num());
    
    for (;;)
    {
        /* Toggle status LED */
        gpio_put(LED_STATUS_PIN, !gpio_get(LED_STATUS_PIN));
        
        /* Wait for the next cycle */
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief System monitor task for watchdog and health checks
 * 
 * This task performs basic system monitoring and watchdog feeding.
 * It demonstrates higher priority task operation.
 * 
 * @param pvParameters Task parameters (unused)
 */
static void prvSystemMonitorTask(void *pvParameters)
{
    (void)pvParameters;  /* Suppress unused parameter warning */
    
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(1000);  /* 1 second monitoring */
    
    printf("System Monitor Task started on core %d\n", get_core_num());
    
    for (;;)
    {
        /* Feed the watchdog */
        watchdog_update();
        
        /* Perform basic health checks */
        printf("System OK - Free heap: %d bytes\n", xPortGetFreeHeapSize());
        
        /* Update power LED to show system is alive */
        gpio_put(LED_POWER_PIN, 1);
        
        /* Wait for the next cycle */
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief Setup hardware peripherals
 */
static void prvSetupHardware(void)
{
    /* Initialize stdio */
    stdio_init_all();
    
    /* Initialize GPIO pins for LEDs */
    gpio_init(LED_STATUS_PIN);
    gpio_set_dir(LED_STATUS_PIN, GPIO_OUT);
    gpio_put(LED_STATUS_PIN, 0);
    
    gpio_init(LED_POWER_PIN);
    gpio_set_dir(LED_POWER_PIN, GPIO_OUT);
    gpio_put(LED_POWER_PIN, 0);
    
    gpio_init(LED_NORMAL_PIN);
    gpio_set_dir(LED_NORMAL_PIN, GPIO_OUT);
    gpio_put(LED_NORMAL_PIN, 0);
    
    gpio_init(LED_ALARM_PIN);
    gpio_set_dir(LED_ALARM_PIN, GPIO_OUT);
    gpio_put(LED_ALARM_PIN, 0);
    
    gpio_init(LED_FAULT_PIN);
    gpio_set_dir(LED_FAULT_PIN, GPIO_OUT);
    gpio_put(LED_FAULT_PIN, 0);
    
    /* Initialize watchdog */
    if (watchdog_caused_reboot()) {
        printf("System rebooted by watchdog!\n");
    }
    
    /* Enable watchdog with 30 second timeout */
    watchdog_enable(TIMEOUT_WATCHDOG_RESET_MS, 1);
    
    printf("Hardware initialization complete\n");
}

/**
 * @brief Main application entry point
 * 
 * This function initializes the hardware, creates FreeRTOS tasks,
 * and starts the scheduler.
 * 
 * @return Never returns (scheduler takes over)
 */
int main(void)
{
    BaseType_t xReturned;
    
    /* Setup hardware peripherals */
    prvSetupHardware();
    
    printf("\n=== FACP iZone Fire Alarm Control Panel ===\n");
    printf("Firmware Version: 1.0.0-dev\n");
    printf("Build Date: %s %s\n", __DATE__, __TIME__);
    printf("Hardware: RP2040-Zero with FreeRTOS SMP\n");
    printf("===========================================\n\n");
    
    /* Create the LED blink task */
    xReturned = xTaskCreate(
        prvLedBlinkTask,                    /* Task function */
        "LED_Blink",                        /* Task name */
        TASK_STACK_SIZE_STATUS_LED,         /* Stack size */
        NULL,                               /* Parameters */
        TASK_PRIORITY_STATUS_LED,           /* Priority */
        &xLedBlinkTaskHandle                /* Task handle */
    );
    
    if (xReturned != pdPASS) {
        printf("Failed to create LED Blink task\n");
        return -1;
    }
    
    /* Create the system monitor task */
    xReturned = xTaskCreate(
        prvSystemMonitorTask,               /* Task function */
        "SysMonitor",                       /* Task name */
        TASK_STACK_SIZE_WATCHDOG,           /* Stack size */
        NULL,                               /* Parameters */
        TASK_PRIORITY_WATCHDOG,             /* Priority */
        &xSystemMonitorTaskHandle           /* Task handle */
    );
    
    if (xReturned != pdPASS) {
        printf("Failed to create System Monitor task\n");
        return -1;
    }
    
    printf("Starting FreeRTOS scheduler...\n");
    
    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();
    
    /* Should never reach here if scheduler starts successfully */
    printf("ERROR: FreeRTOS scheduler failed to start!\n");
    
    /* Infinite loop in case of scheduler failure */
    for (;;) {
        gpio_put(LED_FAULT_PIN, 1);
        sleep_ms(100);
        gpio_put(LED_FAULT_PIN, 0);
        sleep_ms(100);
    }
    
    return 0;
} 