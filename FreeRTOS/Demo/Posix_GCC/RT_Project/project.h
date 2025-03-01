#ifndef EMERGENCY_SYSTEM_H
#define EMERGENCY_SYSTEM_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

/**
 * @file project.h
 * @brief Header file for the emergency event handling system using FreeRTOS.
 *
 * This file contains the necessary constants, type definitions, function prototypes,
 * and global variable declarations required for managing emergency events
 * in a real-time system.
 */

/** @defgroup Constants System Constants */
/** @{ */
#define QUEUE_LENGTH 5               /**< Default queue length for event processing */
#define LOGGER_QUEUE_LENGTH 100      /**< Logger queue size */
#define ERROR_DESCRIPTION_LENGTH 100 /**< Maximum length for error description */
#define MAX_DELAY 20                 /**< Maximum event delay in seconds , The defined interval is too long to effectively test redirections.*/
#define MAX_INTERVAL 1               /**< Maximum interval between event generations */
#define TASK_STACK_SIZE 1000         /**< Stack size for tasks */
#define TICKS_PER_SECOND 1000        /**< FreeRTOS tick rate per second */
#define POLICE_CARS 3                /**< Number of police cars available */
#define AMBULANCES 4                 /**< Number of ambulances available */
#define FIRE_TRUCKS 2                /**< Number of fire trucks available */
/** @} */

/**
 * @enum EventCode
 * @brief Enumeration of event types in the system.
 */
typedef enum
{
    POLICE = 1,         /**< Police event */
    AMBULANCE = 2,      /**< Ambulance event */
    FIRE_DEPARTMENT = 3 /**< Fire department event */
} EventCode;

/**
 * @struct Event
 * @brief Represents an emergency event.
 */
typedef struct
{
    int id;         /**< Unique identifier for the event */
    uint8_t delay;  /**< Delay (in seconds) before handling the event */
    EventCode code; /**< Event type (Police, Ambulance, or Fire Department) */
} Event;

/**
 * @struct Log_Msg
 * @brief Structure for storing log messages.
 */
typedef struct
{
    Event event;                                /**< Event details */
    bool error;                                 /**< Indicates if the log message is an error */
    char description[ERROR_DESCRIPTION_LENGTH]; /**< Description of the log message */
} Log_Msg;

/**
 * @defgroup GlobalVariables Global Variables
 * @brief Global variables shared across multiple source files.
 * @{
 */
extern QueueHandle_t dispatcherQueue; /**< Dispatcher queue */
extern QueueHandle_t FireQueue;       /**< Fire department queue */
extern QueueHandle_t AmbulanceQueue;  /**< Ambulance queue */
extern QueueHandle_t PoliceQueue;     /**< Police queue */
extern QueueHandle_t LoggerQueue;     /**< Logger queue */

extern TimerHandle_t eventGeneratorTimer; /**< Timer for event generation */
extern long long Globalid;                /**< Global event ID counter */
/** @} */

/**
 * @brief Generates a random event and places it in the dispatcher queue.
 * @param xTimer Handle to the FreeRTOS timer.
 */
void EventGeneratorCallback(TimerHandle_t xTimer);

/**
 * @brief Logs an event message to the logger queue.
 * @param event The event associated with the log.
 * @param str Description of the log message.
 * @param error Indicates whether the log message represents an error.
 */
void print_log(Event event, char str[], bool error);

/**
 * @brief Redirects an event to available emergency service queues.
 * @param event The event to be redirected.
 */
void RedirectEvent(Event event);

/**
 * @brief Task function for handling police events.
 * @param params Pointer to task parameters (unused).
 */
void Police_Task(void *params);

/**
 * @brief Task function for handling ambulance events.
 * @param params Pointer to task parameters (unused).
 */
void Ambulance_Task(void *params);

/**
 * @brief Task function for handling fire department events.
 * @param params Pointer to task parameters (unused).
 */
void Fire_Task(void *params);

/**
 * @brief Logger task that logs and writes messages to files.
 * @param args Unused parameter.
 */
void Logger(void *args);

/**
 * @brief Dispatcher task responsible for distributing events to appropriate queues.
 * @param params Unused parameter.
 */
void DispatcherTask(void *params);

/**
 * @brief Initializes resources such as queues and semaphores.
 */
void ResourceInitialization();

/**
 * @brief The main function for the emergency system, setting up tasks and starting FreeRTOS scheduler.
 */
void final_project(void);

/**
 * @brief Converts an EventCode to a string representation.
 * @param code The event code to convert.
 * @return Pointer to a string representing the event type.
 */
char *StringCode(EventCode code);

#endif /* EMERGENCY_SYSTEM_H */