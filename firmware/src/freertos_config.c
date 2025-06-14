/**
 * @file freertos_config.c
 * @brief FreeRTOS Configuration Implementation for RP2040
 * 
 * This file provides port-specific implementations for FreeRTOS
 * configuration on the RP2040 platform.
 * 
 * @author FACP Development Team
 * @date 2024
 */

#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"

/**
 * @brief FreeRTOS port-specific configuration
 * 
 * This function can be used to perform any additional port-specific
 * configuration that cannot be done through FreeRTOSConfig.h
 */
void vPortConfigureTimerForRunTimeStats(void)
{
    /* This function is called if configGENERATE_RUN_TIME_STATS is set to 1 */
    /* Configure a timer for run time stats generation if needed */
    /* For now, we don't need run time stats, so this is empty */
}

/**
 * @brief Get the current timer value for run time stats
 * 
 * @return Current timer value in ticks
 */
uint32_t ulGetRunTimeCounterValue(void)
{
    /* This function is called if configGENERATE_RUN_TIME_STATS is set to 1 */
    /* Return the current timer value for run time stats */
    /* For now, we use the system clock */
    return (uint32_t)time_us_32();
}

/**
 * @brief Assert function for debugging
 * 
 * This function is called when a FreeRTOS assertion fails.
 * It provides debugging information and halts the system.
 * 
 * @param file Source file where assertion failed
 * @param line Line number where assertion failed
 */
void vAssertCalled(const char *file, int line)
{
    printf("ASSERTION FAILED: %s:%d\n", file, line);
    
    /* Disable interrupts */
    taskDISABLE_INTERRUPTS();
    
    /* Halt system */
    for (;;) {
        /* Infinite loop - watchdog will reset the system */
        tight_loop_contents();
    }
}

/**
 * @brief Pre-sleep processing
 * 
 * This function is called before the processor enters sleep mode.
 * It can be used to perform any necessary operations before sleeping.
 * 
 * @param expected_idle_time Expected time the processor will be idle
 */
void vApplicationSleep(TickType_t expected_idle_time)
{
    (void)expected_idle_time;  /* Suppress unused parameter warning */
    
    /* For fire safety applications, we typically don't want to use
     * deep sleep modes as they may affect response times.
     * This function is provided for completeness but remains empty.
     */
}

/**
 * @brief Initialize any additional hardware required by FreeRTOS
 * 
 * This function is called during FreeRTOS initialization to set up
 * any additional hardware that the RTOS might need.
 */
void vPortSetupTimerInterrupt(void)
{
    /* The timer interrupt setup is handled by the port layer */
    /* This function is provided for any additional setup if needed */
}

/**
 * @brief Core-specific idle hook
 * 
 * This function is called when a specific core becomes idle.
 * It can be used for power management or other core-specific operations.
 * 
 * @param core_num Core number that became idle
 */
void vApplicationCoreIdleHook(uint32_t core_num)
{
    (void)core_num;  /* Suppress unused parameter warning */
    
    /* For fire safety applications, we keep cores active for
     * immediate response capability. This function is provided
     * for completeness but remains empty.
     */
}

/**
 * @brief Tick hook implementation
 * 
 * This function is called on every system tick if configUSE_TICK_HOOK
 * is set to 1 in FreeRTOSConfig.h.
 */
void vApplicationTickHook(void)
{
    /* This function is called on every tick interrupt */
    /* It should be kept as short as possible */
    /* Currently not used, but provided for future functionality */
} 