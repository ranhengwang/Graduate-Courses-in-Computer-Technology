#include <cstdint>
#include <stdio.h>
#include "lengthdef.h"
#include "consumer.h"
#include "macros.h"
#include "pipe_buffer.h"
#include "thread_params.h"
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <cstring>
#include <time.h>

using namespace std;

pthread_t *kConsumerThreads;
ThreadParams **kConsumerParams;
uint32_t kConsumerThreadCount = 0;

void *consume_thread_func(void *args)
{
    ThreadParams *thread_args = NULL;
    if (args == nullptr)
    {
        WARNING("%s", "args is nullptr");
    }
    else
    {
        thread_args = (ThreadParams *)args;
    }
    
    int n = 0;
    // char *buffer = new char[thread_args->buf_size];

    PipeBuffer *buffer;
    int fd = open(thread_args->filename, O_RDWR | O_CREAT);
    // thread_args->print_info();
    clock_t start_time, finish_time;
    do
    {
        start_time = clock();
        n = read(thread_args->fd, &buffer, sizeof(PipeBuffer *));
        if (n > 0)
        {
            pwrite(fd, buffer->data, buffer->len, buffer->offset);
            finish_time = clock();
            INFO("thread: %-33s buffer: %-20p spend time: %lf", thread_args->name, buffer, ((double)(finish_time-start_time))/CLOCKS_PER_SEC);
            delete buffer;
        }
        else if (n < 0)
        {
            ERROR("failed to read pipe");
        }
        else
        {
            INFO("fd is closed");
        }
        sleep(1);
    }while (n != 0);

    return NULL;
}

int init_consumer(uint32_t thread_count, int fd, char *filename)
{
    if (fd < 0)
    {
        ERROR("%s", "fd is invalid argument");
        return 1;
    }
    
    kConsumerThreadCount = thread_count;
    kConsumerThreads = new pthread_t[kConsumerThreadCount];
    kConsumerParams = new ThreadParams*[kConsumerThreadCount];

    char thread_name[FILENAME_LEN];
    for (uint32_t i = 0; i < kConsumerThreadCount; ++i)
    {
        memset(thread_name, 0, sizeof(thread_name));
        snprintf(thread_name, sizeof(thread_name) - 1, "%s%d", "consumer", i);
        kConsumerParams[i] = new ThreadParams(i, fd, kConsumerThreadCount, filename, BUFFER_SIZE * 100, thread_name);
        if (pthread_create(kConsumerThreads + i, NULL, consume_thread_func, (void *)(kConsumerParams[i])))
        {
            ERROR("failed to create the thread id %d", kConsumerParams[i]->id);
            return 2;
        }
    }
    

    return 0;
}

int uninit_consumer()
{

    for (uint32_t i = 0; i < kConsumerThreadCount; ++i)
    {
        if (pthread_join(kConsumerThreads[i], NULL))
        {
            ERROR("Failed to join the thread id %d", kConsumerParams[i]->id);
        }
        // ERROR("jointed");
        delete kConsumerParams[i];
    }
    delete[] kConsumerParams;
    return 0;
}




