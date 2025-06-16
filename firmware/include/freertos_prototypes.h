/**
 * @file freertos_prototypes.h
 * @brief Function prototypes for FreeRTOS port-specific functions
 * 
 * This header provides missing function prototypes for FreeRTOS port functions
 * to resolve -Werror=missing-prototypes compilation errors.
 * 
 * @author FACP Development Team
 * @date 2024
 */

#ifndef FREERTOS_PROTOTYPES_H
#define FREERTOS_PROTOTYPES_H

#include "FreeRTOS.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Configure timer for run-time statistics collection
 */
void vPortConfigureTimerForRunTimeStats(void);

/**
 * @brief Get run-time counter value for statistics
 * @return Current run-time counter value
 */
uint32_t ulGetRunTimeCounterValue(void);

/**
 * @brief Called when an assertion fails
 * @param file Source file where assertion failed
 * @param line Line number where assertion failed
 */
void vAssertCalled(const char *file, int line);

/**
 * @brief Called when the system goes to sleep
 * @param expected_idle_time Expected idle time in ticks
 */
void vApplicationSleep(TickType_t expected_idle_time);

/**
 * @brief Setup timer interrupt for FreeRTOS tick
 */
void vPortSetupTimerInterrupt(void);

/**
 * @brief Called when a core enters idle state
 * @param core_num Core number that is going idle
 */
void vApplicationCoreIdleHook(uint32_t core_num);

/**
 * @brief Called on each FreeRTOS tick
 */
void vApplicationTickHook(void);

#ifdef __cplusplus
}
#endif

#endif /* FREERTOS_PROTOTYPES_H */ 