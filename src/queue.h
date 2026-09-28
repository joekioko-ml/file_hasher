#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

typedef struct Task {
    char *path;
    struct Task *next;
} Task;

typedef struct {
    Task *front;
    Task *rear;

    pthread_mutex_t mutex;
    pthread_cond_t cond;

    int shutdown;
} TaskQueue;

void queue_init(TaskQueue *queue);

void queue_push(TaskQueue *queue, const char *path);

char *queue_pop(TaskQueue *queue);

void queue_shutdown(TaskQueue *queue);

void queue_destroy(TaskQueue *queue);

#endif
