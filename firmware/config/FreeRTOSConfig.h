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
#define configNUM_CORES                         2
#define configUSE_CORE_AFFINITY                 1
#define configUSE_PASSIVE_IDLE_HOOK             0

/* Memory allocation related definitions. */
#define configSUPPORT_STATIC_ALLOCATION         1
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   (128*1024)
#define configAPPLICATION_ALLOCATED_HEAP        0

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
#define configKERNEL_INTERRUPT_PRIORITY         [configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS)]
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    [configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS)]

/* SMP port only */
#define configTICK_CORE                         0

/* Define to trap errors during development. */
#define configASSERT(x)                         if((x) == 0) {taskDISABLE_INTERRUPTS(); for(;;);}

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

/* Task Stack Sizes (in words) */
#define TASK_STACK_SIZE_SENSOR_MONITOR          512
#define TASK_STACK_SIZE_ALARM_CONTROL           256
#define TASK_STACK_SIZE_COMMUNICATION           384
#define TASK_STACK_SIZE_STATUS_LED              256
#define TASK_STACK_SIZE_DIAGNOSTICS             512
#define TASK_STACK_SIZE_WATCHDOG                256

/* Queue Sizes */
#define QUEUE_SIZE_SENSOR_DATA                  8
#define QUEUE_SIZE_ALARM_COMMANDS               4
#define QUEUE_SIZE_COMM_MESSAGES                8

/* Timeouts */
#define TIMEOUT_SENSOR_RESPONSE_MS              100
#define TIMEOUT_COMMUNICATION_MS                1000
#define TIMEOUT_WATCHDOG_RESET_MS               30000

#endif /* FREERTOS_CONFIG_H */ 