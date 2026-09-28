#include "threadpool.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void *worker(void *arg)
{
    ThreadPool *pool = (ThreadPool *)arg;

    while (1)
    {
        char *path = queue_pop(pool->queue);

        if (path == NULL)
            break;

        pool->worker_function(path);

        free(path);
    }

    return NULL;
}

int threadpool_init(ThreadPool *pool,
                    int num_threads,
                    TaskQueue *queue,
                    void (*worker_function)(const char *))
{
    pool->num_threads = num_threads;
    pool->queue = queue;
    pool->worker_function = worker_function;

    pool->threads =
        malloc(sizeof(pthread_t) * num_threads);

    if (pool->threads == NULL)
        return -1;

    for (int i = 0; i < num_threads; i++)
    {
        if (pthread_create(&pool->threads[i],
                           NULL,
                           worker,
                           pool) != 0)
        {
            return -1;
        }
    }

    return 0;
}

void threadpool_join(ThreadPool *pool)
{
    for (int i = 0; i < pool->num_threads; i++)
    {
        pthread_join(pool->threads[i], NULL);
    }
}

void threadpool_destroy(ThreadPool *pool)
{
    free(pool->threads);
}
