#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <pthread.h>
#include "queue.h"

typedef struct ThreadPool
{
    pthread_t *threads;
    int num_threads;

    TaskQueue *queue;

    void (*worker_function)(const char *path);

} ThreadPool;

int threadpool_init(ThreadPool *pool,
                    int num_threads,
                    TaskQueue *queue,
                    void (*worker_function)(const char *));

void threadpool_join(ThreadPool *pool);

void threadpool_destroy(ThreadPool *pool);

#endif
