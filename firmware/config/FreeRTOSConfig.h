#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Scheduler Related */
#define configUSE_PREEMPTION                    1
#define configUSE_TICKLESS_IDLE                 0
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configTICK_RATE_HZ                      1000
#define configMAX_PRIORITIES                    32
#define configMINIMAL_STACK_SIZE                (configSTACK_DEPTH_TYPE)256
#define configUSE_16_BIT_TICKS                  0

/* SMP Configuration for RP2040 dual-core */
#define configNUMBER_OF_CORES                   2
#define configUSE_CORE_AFFINITY                 1
#define configUSE_PASSIVE_IDLE_HOOK             0

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   (96*1024)  /* Optimized for RP2040-Zero */
#define configAPPLICATION_ALLOCATED_HEAP        0
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP 0

/* Memory optimization for fire safety critical operations */
#define configUSE_HEAP_SCHEME                   4  /* Use heap_4 for better fragmentation handling */

/* Hook function related definitions. */
#define configCHECK_FOR_STACK_OVERFLOW          2
#define configUSE_MALLOC_FAILED_HOOK            1
#define configUSE_DAEMON_TASK_STARTUP_HOOK      0

/* Run time and task stats gathering related definitions. */
#define configGENERATE_RUN_TIME_STATS           0
#define configUSE_TRACE_FACILITY                1
#define configUSE_STATS_FORMATTING_FUNCTIONS    1

/* Co-routine related definitions. */
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         1

/* Software timer related definitions. */
#define configUSE_TIMERS                        1
#define configTIMER_TASK_PRIORITY               (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH                10
#define configTIMER_TASK_STACK_DEPTH            1024

/* Interrupt nesting behaviour configuration. */
/* These are defined by the Pico SDK port */
#define configKERNEL_INTERRUPT_PRIORITY         255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    192

/* SMP Configuration Options - Handled by RP2040 SMP Port */

/* Core Affinity Definitions for Fire Safety Tasks */
#define CORE_AFFINITY_SENSORS                   (1 << 0)  /* Core 0 for critical sensor tasks */
#define CORE_AFFINITY_COMMUNICATION             (1 << 1)  /* Core 1 for I2C/GSM communication */
#define CORE_AFFINITY_ANY                       ((1 << configNUMBER_OF_CORES) - 1)  /* Any core */

/* Define to trap errors during development. */
#define configASSERT(x)                         if((x) == 0) {portDISABLE_INTERRUPTS(); for(;;);}

/* FreeRTOS MPU specific definitions. */
#define configINCLUDE_APPLICATION_DEFINED_PRIVILEGED_FUNCTIONS 0

/* Optional functions - most linkers will remove unused functions anyway. */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_xResumeFromISR                  1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetCurrentTaskHandle       1
#define INCLUDE_uxTaskGetStackHighWaterMark     1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xEventGroupSetBitFromISR        1
#define INCLUDE_xTimerPendFunctionCall          1
#define INCLUDE_xTaskAbortDelay                 0
#define INCLUDE_xTaskGetHandle                  0
#define INCLUDE_xTaskResumeFromISR              1

/* A header file that defines trace macros can be included here. */

/* Fire Safety System Specific Priorities */
#define TASK_PRIORITY_WATCHDOG                  (configMAX_PRIORITIES - 1)  /* Highest */
#define TASK_PRIORITY_SENSOR_MONITOR            (configMAX_PRIORITIES - 2)  /* Critical */
#define TASK_PRIORITY_ALARM_CONTROL             (configMAX_PRIORITIES - 3)  /* High */
#define TASK_PRIORITY_COMMUNICATION             (configMAX_PRIORITIES - 4)  /* High */
#define TASK_PRIORITY_STATUS_LED                (configMAX_PRIORITIES - 8)  /* Medium */
#define TASK_PRIORITY_DIAGNOSTICS               (configMAX_PRIORITIES - 10) /* Low */

/* Task Stack Sizes (in words) - Optimized for SMP operation */
#define TASK_STACK_SIZE_SENSOR_MONITOR          512  /* Core 0 - Critical sensor processing */
#define TASK_STACK_SIZE_ALARM_CONTROL           256  /* Core 0 - Time-critical alarm logic */
#define TASK_STACK_SIZE_COMMUNICATION           384  /* Core 1 - I2C/GSM communication */
#define TASK_STACK_SIZE_STATUS_LED              256  /* Core 1 - Non-critical UI operations */
#define TASK_STACK_SIZE_DIAGNOSTICS             512  /* Core 1 - System diagnostics */
#define TASK_STACK_SIZE_WATCHDOG                256  /* Core 0 - Critical safety monitor */

/* Core Affinity Task Assignments */
#define TASK_CORE_AFFINITY_SENSOR_MONITOR       CORE_AFFINITY_SENSORS
#define TASK_CORE_AFFINITY_ALARM_CONTROL        CORE_AFFINITY_SENSORS
#define TASK_CORE_AFFINITY_COMMUNICATION        CORE_AFFINITY_COMMUNICATION
#define TASK_CORE_AFFINITY_STATUS_LED           CORE_AFFINITY_COMMUNICATION
#define TASK_CORE_AFFINITY_DIAGNOSTICS          CORE_AFFINITY_COMMUNICATION
#define TASK_CORE_AFFINITY_WATCHDOG             CORE_AFFINITY_SENSORS

/* Queue Sizes */
#define QUEUE_SIZE_SENSOR_DATA                  8
#define QUEUE_SIZE_ALARM_COMMANDS               4
#define QUEUE_SIZE_COMM_MESSAGES                8

/* Timeouts */
#define TIMEOUT_SENSOR_RESPONSE_MS              100
#define TIMEOUT_COMMUNICATION_MS                1000
#define TIMEOUT_WATCHDOG_RESET_MS               30000

#endif /* FREERTOS_CONFIG_H */ 