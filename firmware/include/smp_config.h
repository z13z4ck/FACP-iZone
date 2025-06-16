/**
 * @file smp_config.h
 * @brief SMP Configuration and Core Affinity Management for FACP iZone
 * 
 * This header provides utilities for managing task affinity and symmetric
 * multi-processing configuration on the RP2040 dual-core processor.
 * 
 * @author FACP Development Team
 * @date 2024
 */

#ifndef SMP_CONFIG_H
#define SMP_CONFIG_H

#include "FreeRTOS.h"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Core affinity assignment strategy for fire safety system
 * 
 * Core 0: Time-critical fire detection operations
 * - Sensor monitoring tasks
 * - Alarm control logic  
 * - Watchdog management
 * 
 * Core 1: Communication and management operations
 * - I2C master communication
 * - GSM/SMS operations
 * - Status LED management
 * - System diagnostics
 */

/**
 * @brief Create a task with specified core affinity
 * 
 * This function wraps xTaskCreate to include core affinity assignment
 * for optimal task placement in the fire safety system.
 * 
 * @param pxTaskCode Task function pointer
 * @param pcName Task name string
 * @param usStackDepth Stack size in words
 * @param pvParameters Task parameters
 * @param uxPriority Task priority
 * @param pxCreatedTask Task handle (can be NULL)
 * @param uxCoreAffinityMask Core affinity mask
 * @return pdPASS if successful, errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY if failed
 */
BaseType_t xTaskCreateWithAffinity(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    const configSTACK_DEPTH_TYPE usStackDepth,
    void * const pvParameters,
    UBaseType_t uxPriority,
    TaskHandle_t * const pxCreatedTask,
    UBaseType_t uxCoreAffinityMask
);

/**
 * @brief Create a fire safety sensor monitoring task
 * 
 * Creates a task optimized for time-critical sensor monitoring operations
 * with affinity to Core 0 for deterministic response times.
 * 
 * @param pxTaskCode Task function pointer
 * @param pcName Task name string  
 * @param pvParameters Task parameters
 * @param pxCreatedTask Task handle (can be NULL)
 * @return pdPASS if successful, error code if failed
 */
BaseType_t xCreateSensorTask(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask
);

/**
 * @brief Create a communication task
 * 
 * Creates a task optimized for I2C/GSM communication operations
 * with affinity to Core 1 to avoid interference with sensor timing.
 * 
 * @param pxTaskCode Task function pointer
 * @param pcName Task name string
 * @param pvParameters Task parameters  
 * @param pxCreatedTask Task handle (can be NULL)
 * @return pdPASS if successful, error code if failed
 */
BaseType_t xCreateCommunicationTask(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask
);

/**
 * @brief Create an alarm control task
 * 
 * Creates a task optimized for time-critical alarm control operations
 * with affinity to Core 0 for immediate response capability.
 * 
 * @param pxTaskCode Task function pointer
 * @param pcName Task name string
 * @param pvParameters Task parameters
 * @param pxCreatedTask Task handle (can be NULL)
 * @return pdPASS if successful, error code if failed
 */
BaseType_t xCreateAlarmTask(
    TaskFunction_t pxTaskCode,
    const char * const pcName,
    void * const pvParameters,
    TaskHandle_t * const pxCreatedTask
);

/**
 * @brief Get the current core number for the calling task
 * 
 * Returns the core number (0 or 1) that the current task is running on.
 * Useful for debugging and performance monitoring.
 * 
 * @return Core number (0 or 1)
 */
uint32_t ulGetCurrentCore(void);

/**
 * @brief Print SMP status and task distribution
 * 
 * Outputs debug information about current task distribution across cores
 * and SMP configuration status. Useful for system monitoring and debugging.
 */
void vPrintSMPStatus(void);

/**
 * @brief Validate SMP configuration
 * 
 * Performs a runtime check of the SMP configuration to ensure
 * both cores are operational and tasks are properly distributed.
 * 
 * @return pdTRUE if SMP is working correctly, pdFALSE if issues detected
 */
BaseType_t xValidateSMPConfiguration(void);

/**
 * @brief Create SMP test tasks for demonstration
 * 
 * Creates test tasks to demonstrate and validate SMP functionality.
 * Used for system validation and debugging.
 * 
 * @return pdTRUE if all test tasks created successfully, pdFALSE otherwise
 */
BaseType_t xCreateSMPTestTasks(void);

#ifdef __cplusplus
}
#endif

#endif /* SMP_CONFIG_H */ 