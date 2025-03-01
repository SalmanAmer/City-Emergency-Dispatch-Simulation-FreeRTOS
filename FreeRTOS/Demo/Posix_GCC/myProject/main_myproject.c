#include <stdio.h>
#include <unistd.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

typedef struct args
{
    char id[10];
    char name[100];
    FILE *file
} args_t;
TaskHandle_t *task;
SemaphoreHandle_t mutex;
void task1(void *args)
{

    int cnt = 0;
    while (1)
    {
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(20)) != pdPASS)
            continue;
        ((args_t *)args)->file = fopen("output.txt", "a");
        if (((args_t *)args)->file == NULL)
        {
            perror("Failed to open file");
        }

        // Write data to the file
        fprintf(((args_t *)args)->file, " Task 1: id = %s, name = %s \n", ((args_t *)args)->id, ((args_t *)args)->name);
        fflush(((args_t *)args)->file);
        // Close the file
        if (fclose(((args_t *)args)->file) != 0)
        {
            perror("Failed to close file");
        }
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(2000));
        // xSemaphoreGive(mutex);
        if (++cnt == 10)
        {
            printf("del %s \n", pcTaskGetName(task));
            vTaskDelete(NULL);
        }
    }
}
void task2(void *args)
{

    while (1)
    {

        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(20)) != pdPASS)
            continue;
        ((args_t *)args)->file = fopen("output.txt", "a");
        if (((args_t *)args)->file == NULL)
        {
            perror("Failed to open file");
        }

        // Write data to the file
        fprintf(((args_t *)args)->file, " Task 2: id = %s, name = %s \n", ((args_t *)args)->id, ((args_t *)args)->name);
        fflush(((args_t *)args)->file);
        // Close the file
        if (fclose(((args_t *)args)->file) != 0)
        {
            perror("Failed to close file");
        }
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(2000));
        // xSemaphoreGive(mutex);
    }
}
void task3(void *args)
{

    while (1)
    {
        if (xSemaphoreTake(mutex, pdMS_TO_TICKS(20)) != pdPASS)
            continue;
        ((args_t *)args)->file = fopen("output.txt", "a");
        if (((args_t *)args)->file == NULL)
        {
            perror("Failed to open file");
        }

        // Write data to the file
        fprintf(((args_t *)args)->file, " Task 3: id = %s, name = %s \n", ((args_t *)args)->id, ((args_t *)args)->name);
        fflush(((args_t *)args)->file);
        // Close the file
        if (fclose(((args_t *)args)->file) != 0)
        {
            perror("Failed to close file");
        }
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(2000));
        }
}

void main_myproject(void)
{
    mutex = xSemaphoreCreateMutex();

    if (xTaskCreate(task1, "Task One", PTHREAD_STACK_MIN, (void *)&(args_t){.id = "322701509", .name = "Salman Amer"}, 1, task) != pdPASS)
    {
    }
    if (xTaskCreate(task2, "Task Two", PTHREAD_STACK_MIN, (void *)&(args_t){.id = "322701509", .name = "Salman Amer"}, 1, NULL) != pdPASS)
    {
    }
    if (xTaskCreate(task3, "Task Three", PTHREAD_STACK_MIN, (void *)&(args_t){.id = "322701509", .name = "Salman Amer"}, 1, NULL) != pdPASS)
    {
    }
    vTaskStartScheduler();
    while (1)
    {
    }
}
