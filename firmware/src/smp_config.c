/**
 * @file smp_config.c
 * @brief SMP Configuration Implementation for FACP iZone
 * 
 * Implementation of symmetric multi-processing utilities for optimal
 * task distribution across RP2040 dual cores.
 * 
 * @author FACP Development Team
 * @date 2024
 */

#include <stdio.h>
#include "smp_config.h"
#include "pico/multicore.h"
#include "hardware/sync.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief Create a task with specified core affinity
 */
BaseType_t xTaskCreateWithAffinity(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    const configSTACK_DEPTH_TYPE usStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t * const pxCreatedTask,
    UBaseType_t uxCoreAffinityMask
)
{
    BaseType_t xResult;
    
    /* Create the task using standard FreeRTOS function */
    xResult = xTaskCreate(
        pxTaskCode,
        pcName,
        usStackDepth,
        pvParameters,
        uxPriority,
        pxCreatedTask
    );
    
    /* Set core affinity if task creation was successful */
    if ((xResult == pdPASS) && (pxCreatedTask != NULL) && (*pxCreatedTask != NULL))
    {
        vTaskCoreAffinitySet(*pxCreatedTask, uxCoreAffinityMask);
        
        printf("Task '%s' created with core affinity: 0x%02X\n", 
               pcName, (unsigned int)uxCoreAffinityMask);
    }
    
    return xResult;
}

/**
 * @brief Create a fire safety sensor monitoring task
 */
BaseType_t xCreateSensorTask(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask
)
{
    return xTaskCreateWithAffinity(
        pxTaskCode,
        pcName,
        TASK_STACK_SIZE_SENSOR_MONITOR,
        pvParameters,
        TASK_PRIORITY_SENSOR_MONITOR,
        pxCreatedTask,
        TASK_CORE_AFFINITY_SENSOR_MONITOR
    );
}

/**
 * @brief Create a communication task
 */
BaseType_t xCreateCommunicationTask(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask
)
{
    return xTaskCreateWithAffinity(
        pxTaskCode,
        pcName,
        TASK_STACK_SIZE_COMMUNICATION,
        pvParameters,
        TASK_PRIORITY_COMMUNICATION,
        pxCreatedTask,
        TASK_CORE_AFFINITY_COMMUNICATION
    );
}

/**
 * @brief Create an alarm control task
 */
BaseType_t xCreateAlarmTask(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask
)
{
    return xTaskCreateWithAffinity(
        pxTaskCode,
        pcName,
        TASK_STACK_SIZE_ALARM_CONTROL,
        pvParameters,
        TASK_PRIORITY_ALARM_CONTROL,
        pxCreatedTask,
        TASK_CORE_AFFINITY_ALARM_CONTROL
    );
}

/**
 * @brief Get the current core number for the calling task
 */
uint32_t ulGetCurrentCore(void)
{
    return get_core_num();
}

/**
 * @brief Print SMP status and task distribution
 */
void vPrintSMPStatus(void)
{
    printf("\n=== SMP Configuration Status ===\n");
    printf("Number of cores: %d\n", configNUMBER_OF_CORES);
    printf("Core affinity enabled: %s\n", 
           configUSE_CORE_AFFINITY ? "YES" : "NO");
    printf("Time slicing enabled: %s\n",
           configUSE_TIME_SLICING ? "YES" : "NO");
    printf("Current core: %lu\n", ulGetCurrentCore());
    printf("Free heap size: %d bytes\n", xPortGetFreeHeapSize());
    
    /* Print task distribution */
    printf("\nTask Core Affinity Strategy:\n");
    printf("Core 0 (Sensors): 0x%02X\n", CORE_AFFINITY_SENSORS);
    printf("Core 1 (Communication): 0x%02X\n", CORE_AFFINITY_COMMUNICATION);
    printf("Any Core: 0x%02X\n", CORE_AFFINITY_ANY);
    
    printf("==============================\n\n");
}

/**
 * @brief Validate SMP configuration
 */
BaseType_t xValidateSMPConfiguration(void)
{
    BaseType_t xResult = pdTRUE;
    
    printf("Validating SMP configuration...\n");
    
    /* Check if we have the expected number of cores */
    if (configNUMBER_OF_CORES != 2)
    {
        printf("ERROR: Expected 2 cores, configured for %d\n", configNUMBER_OF_CORES);
        xResult = pdFALSE;
    }
    
    /* Check if core affinity is enabled */
    if (!configUSE_CORE_AFFINITY)
    {
        printf("WARNING: Core affinity is disabled\n");
        xResult = pdFALSE;
    }
    
    /* Verify both cores are accessible */
    uint32_t ulCurrentCore = ulGetCurrentCore();
    printf("Current core: %lu\n", ulCurrentCore);
    
    if (ulCurrentCore >= configNUMBER_OF_CORES)
    {
        printf("ERROR: Invalid core number: %lu\n", ulCurrentCore);
        xResult = pdFALSE;
    }
    
    /* Check memory availability */
    size_t xFreeHeap = xPortGetFreeHeapSize();
    printf("Free heap: %d bytes\n", (int)xFreeHeap);
    
    if (xFreeHeap < 8192)  /* Minimum 8KB free heap */
    {
        printf("WARNING: Low memory - %d bytes free\n", (int)xFreeHeap);
    }
    
    if (xResult == pdTRUE)
    {
        printf("SMP configuration validation: PASSED\n");
    }
    else
    {
        printf("SMP configuration validation: FAILED\n");
    }
    
    return xResult;
}

/**
 * @brief Task function to demonstrate SMP operation
 * 
 * This is a utility function that can be used to test SMP functionality
 * by creating tasks that report which core they're running on.
 */
void vSMPTestTask(void *pvParameters)
{
    const char *pcTaskName = (const char *)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(5000);  /* 5 second interval */
    
    printf("SMP Test Task '%s' started on core %lu\n", 
           pcTaskName ? pcTaskName : "Unknown", ulGetCurrentCore());
    
    for (;;)
    {
        printf("Task '%s' running on core %lu, free heap: %d\n",
               pcTaskName ? pcTaskName : "Unknown",
               ulGetCurrentCore(),
               xPortGetFreeHeapSize());
        
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/**
 * @brief Create SMP test tasks for validation
 * 
 * Creates test tasks on both cores to validate SMP operation.
 * Should be called during system initialization for testing.
 */
BaseType_t xCreateSMPTestTasks(void)
{
    TaskHandle_t xTestTask0 = NULL;
    TaskHandle_t xTestTask1 = NULL;
    BaseType_t xResult = pdTRUE;
    
    /* Create test task for Core 0 */
    if (xTaskCreateWithAffinity(
            vSMPTestTask,
            "SMPTest_Core0",
            256,
            (void *)"Core0_Test",
            tskIDLE_PRIORITY + 1,
            &xTestTask0,
            CORE_AFFINITY_SENSORS
        ) != pdPASS)
    {
        printf("Failed to create SMP test task for Core 0\n");
        xResult = pdFALSE;
    }
    
    /* Create test task for Core 1 */
    if (xTaskCreateWithAffinity(
            vSMPTestTask,
            "SMPTest_Core1", 
            256,
            (void *)"Core1_Test",
            tskIDLE_PRIORITY + 1,
            &xTestTask1,
            CORE_AFFINITY_COMMUNICATION
        ) != pdPASS)
    {
        printf("Failed to create SMP test task for Core 1\n");
        xResult = pdFALSE;
    }
    
    if (xResult == pdTRUE)
    {
        printf("SMP test tasks created successfully\n");
    }
    
    return xResult;
} 