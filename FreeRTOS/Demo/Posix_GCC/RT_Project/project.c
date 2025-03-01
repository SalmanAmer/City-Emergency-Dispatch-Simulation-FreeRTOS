#include "FreeRTOS.h"
#include "project.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

QueueHandle_t AmbulanceQueue = NULL;
QueueHandle_t FireQueue = NULL;
QueueHandle_t LoggerQueue = NULL;
QueueHandle_t PoliceQueue = NULL;

TimerHandle_t eventGeneratorTimer = NULL;
QueueHandle_t dispatcherQueue = NULL;

long long Globalid = 0;

char *StringCode(EventCode code)
{
    switch (code)
    {
    case POLICE:
        return "police";
        break;

    case AMBULANCE:
        return "ambulance";
        break;

    case FIRE_DEPARTMENT:
        return "fire department";
        break;

    default:
        break;
    }
}

void print_log(Event event, char str[], bool error)
{

    Log_Msg msg = {.event = event, .error = error};
    snprintf(msg.description, ERROR_DESCRIPTION_LENGTH, "%s", str);
    if (xQueueSend(LoggerQueue, &msg, 0) != pdPASS)
    {
        printf("Logger Failure!!");
    }
}

void RedirectEvent(Event event)
{

    if (xQueueSend(AmbulanceQueue, (void *)&event, 0) != pdPASS)
    {
        if (xQueueSend(PoliceQueue, (void *)&event, 0) != pdPASS)
        {
            if (xQueueSend(FireQueue, (void *)&event, 0) != pdPASS)
            {
                // print_log(event, "All queues are full event dropped!", true);
                print_log(event, "All queues are full, event will wait !", true);

                if (xQueueSendToFront(dispatcherQueue, (void *)&event, 0) != pdPASS)
                {
                }
                vTaskDelay(pdMS_TO_TICKS(TICKS_PER_SECOND));
            }
        }
    }
}
/* Event Generator Callback */
void EventGeneratorCallback(TimerHandle_t xTimer)
{
    Event event;
    if (uxQueueSpacesAvailable(dispatcherQueue) > 1)
    {

        event.id = Globalid++;
        printf("id %d\n", event.id);
        event.code = (EventCode)((rand() % 3) + 1); // Random code between 1 and 3

        event.delay = ((rand() % (MAX_DELAY)) + 1);
        if (xQueueSend(dispatcherQueue, &event, 0) == pdPASS)
        {

            printf("Generated Event: Code %d\n", event.code);
        }
    }
    else
    {
        print_log(event, "Dispatcher Queue is full!\n Waiting...", true);
    }
    TickType_t randomTicks = ((rand() % (MAX_INTERVAL)) + 1);
    xTimerChangePeriod(eventGeneratorTimer, randomTicks * TICKS_PER_SECOND, 0);
}

void Police_Task(void *params)
{
    Event receivedEvent;
    while (1)
    {

        if (xQueueReceive(PoliceQueue, &receivedEvent, portMAX_DELAY) == pdTRUE)
        {
            printf("Police: Handling task...\n");

            vTaskDelay(pdMS_TO_TICKS(receivedEvent.delay * TICKS_PER_SECOND));
            printf("Police: Task completed.\n");
        }
        Log_Msg msg = {.event = receivedEvent};
        if (xQueueSend(LoggerQueue, &msg, 0) != pdPASS)
        {
            printf("Logger Failure!!");
        }
    }
}

void Ambulance_Task(void *params)
{
    Event receivedEvent;
    while (1)
    {

        if (xQueueReceive(AmbulanceQueue, &receivedEvent, portMAX_DELAY) == pdTRUE)
        {
            printf("Ambulance: Handling task...\n");

            vTaskDelay(pdMS_TO_TICKS(receivedEvent.delay * TICKS_PER_SECOND));
            printf("Ambulance: Task completed.\n");
        }
        Log_Msg msg = {.event = receivedEvent};
        if (xQueueSend(LoggerQueue, &msg, 0) != pdPASS)
        {
            printf("Logger Failure!!");
        }
    }
}

void Fire_Task(void *params)
{
    Event receivedEvent;
    while (1)
    {

        if (xQueueReceive(FireQueue, &receivedEvent, portMAX_DELAY) == pdTRUE)
        {
            printf("Fire Department: Handling task...\n");

            vTaskDelay(pdMS_TO_TICKS(receivedEvent.delay * TICKS_PER_SECOND));
            printf("Fire Department: Task completed.\n");
        }
        Log_Msg msg = {.event = receivedEvent};
        if (xQueueSend(LoggerQueue, &msg, 0) != pdPASS)
        {

            printf("Logger Failure!!");
        }
    }
}

void Logger(void *args)
{
    printf(" Logger is Working");
    FILE *file = fopen("Log.txt", "a");
    FILE *Errorfile = fopen("Errors.txt", "a");
    Log_Msg msg;
    while (1)
    {
        if (xQueueReceive(LoggerQueue, &msg, portMAX_DELAY) == pdTRUE)
        {

            if (msg.error)
            {
                fprintf(Errorfile, "%d) %s: %s !\n", msg.event.id, StringCode(msg.event.code), msg.description);
                fflush(Errorfile);
            }
            else
            {
                fprintf(file, "Received call with ID %d and  Code %d ( %s ) . A  free resource was allocated for the task, which lasted %d ticks. The task was completed.\n", msg.event.id, msg.event.code, StringCode(msg.event.code), msg.event.delay);
                fflush(file);
            }
        }
    }
    fclose(file);
    fclose(Errorfile);
}

/* Dispatcher Task */
void DispatcherTask(void *params)
{
    Event event;

    while (1)
    {
        if (xQueueReceive(dispatcherQueue, &event, portMAX_DELAY) == pdTRUE)
        {
            printf("Dispatcher: Processing Event Code %d\n", event.code);

            switch (event.code)
            {
            case POLICE:

                printf("Police: Handling event...\n");
                if (xQueueSend(PoliceQueue, (void *)&event, 0) != pdPASS)
                {
                    print_log(event, "Couldn't send to queue! event is redirected", true);
                    RedirectEvent(event);
                }
                break;

            case AMBULANCE:

                printf("Ambulance: Handling event...\n");
                if (xQueueSend(AmbulanceQueue, (void *)&event, 0) != pdPASS)
                {
                    print_log(event, "Couldn't send to queue! event is redirected", true);
                    RedirectEvent(event);
                }

                break;

            case FIRE_DEPARTMENT:

                printf("Fire: Handling event...\n");
                if (xQueueSend(FireQueue, (void *)&event, 0) != pdPASS)
                {
                    print_log(event, "Couldn't send to queue! event is redirected", true);
                    RedirectEvent(event);
                }

                break;

            default:

                break;
            }
        }
    }
}

/* Resource Initialization */
void ResourceInitialization()
{

    FireQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Event));
    AmbulanceQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Event));
    PoliceQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Event));
    dispatcherQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Event));
    LoggerQueue = xQueueCreate(LOGGER_QUEUE_LENGTH, sizeof(Log_Msg));

    if (!FireQueue || !AmbulanceQueue || !PoliceQueue)
    {
        printf("Failed to create queues!\n");
        exit(1);
    }

    EventCode police = POLICE, ambulance = AMBULANCE, fire = FIRE_DEPARTMENT;
    xTaskCreate(Logger, "Logger Task", TASK_STACK_SIZE, NULL, 2, NULL);
    for (size_t i = 0; i < FIRE_TRUCKS; i++)
    {
        xTaskCreate(Fire_Task, "Fire Task", TASK_STACK_SIZE, NULL, 2, NULL);
    }

    for (size_t i = 0; i < AMBULANCES; i++)
    {
        xTaskCreate(Ambulance_Task, "Ambulance Task", TASK_STACK_SIZE, NULL, 2, NULL);
    }

    for (size_t i = 0; i < POLICE_CARS; i++)
    {
        xTaskCreate(Police_Task, "Police Task", TASK_STACK_SIZE, NULL, 2, NULL);
    }

    // TaskParams policeParams = {
    //     .semaphore = policeSemaphore,
    //     .eventQueue = PoliceQueue,
    //     .departmentCode = POLICE};

    // TaskParams ambulanceParams = {
    //     .semaphore = ambulanceSemaphore,
    //     .eventQueue = AmbulanceQueue,
    //     .departmentCode = AMBULANCE};

    // TaskParams fireParams = {
    //     .semaphore = fireSemaphore,
    //     .eventQueue = FireQueue,
    //     .departmentCode = FIRE_DEPARTMENT};

    // xTaskCreate(DepartmentTask, "Police_Task", TASK_STACK_SIZE, &policeParams, 2, NULL);
    // xTaskCreate(DepartmentTask, "Ambulance_Task", TASK_STACK_SIZE, &ambulanceParams, 2, NULL);
    // xTaskCreate(DepartmentTask, "Fire_Task", TASK_STACK_SIZE, &fireParams, 2, NULL);
}

/* Main Function */
void final_project(void)
{
    /* Initialize Random Seed */
    srand(time(NULL));

    /* Initialize Resources */
    ResourceInitialization();

    /* Create Dispatcher Task */
    xTaskCreate(DispatcherTask, "Dispatcher", PTHREAD_STACK_MIN, NULL, 2, NULL);

    /* Create Timer for Event Generation */
    eventGeneratorTimer = xTimerCreate(
        "EventGenerator",
        pdMS_TO_TICKS(TICKS_PER_SECOND), // Timer period (1 second)
        pdTRUE,                          // Auto-reload
        NULL,
        EventGeneratorCallback);

    if (eventGeneratorTimer != NULL)
    {
        xTimerStart(eventGeneratorTimer, 0);
    }
    else
    {
        printf("Failed to create event generator timer!\n");
        exit(1);
    }

    /* Start Scheduler */
    vTaskStartScheduler();

    /* This point should never be reached */
    while (1)
        ;
}