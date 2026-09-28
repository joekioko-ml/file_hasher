#include "queue.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void queue_init(TaskQueue *queue)
{
    queue->front = NULL;
    queue->rear = NULL;
    queue->shutdown = 0;

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);
}

void queue_push(TaskQueue *queue, const char *path)
{
    Task *task = malloc(sizeof(Task));

    if (!task)
        return;

    task->path = strdup(path);
    task->next = NULL;

    pthread_mutex_lock(&queue->mutex);

    if (queue->rear == NULL)
    {
        queue->front = task;
        queue->rear = task;
    }
    else
    {
        queue->rear->next = task;
        queue->rear = task;
    }

    pthread_cond_signal(&queue->cond);

    pthread_mutex_unlock(&queue->mutex);
}

char *queue_pop(TaskQueue *queue)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->front == NULL && !queue->shutdown)
    {
        pthread_cond_wait(&queue->cond,
                          &queue->mutex);
    }

    if (queue->shutdown)
    {
        pthread_mutex_unlock(&queue->mutex);
        return NULL;
    }

    Task *task = queue->front;

    queue->front = task->next;

    if (queue->front == NULL)
        queue->rear = NULL;

    pthread_mutex_unlock(&queue->mutex);

    char *path = task->path;

    free(task);

    return path;
}

void queue_shutdown(TaskQueue *queue)
{
    pthread_mutex_lock(&queue->mutex);

    queue->shutdown = 1;

    pthread_cond_broadcast(&queue->cond);

    pthread_mutex_unlock(&queue->mutex);
}

void queue_destroy(TaskQueue *queue)
{
    Task *current = queue->front;

    while (current)
    {
        Task *next = current->next;

        free(current->path);
        free(current);

        current = next;
    }

    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}
