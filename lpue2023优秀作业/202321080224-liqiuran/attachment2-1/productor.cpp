#include <cstdint>
#include <stdio.h>
#include "consumer.h"
#include "lengthdef.h"
#include "pipe_buffer.h"
#include "productor.h"
#include "macros.h"
#include "thread_params.h"
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <cstring>
#include <time.h>

using namespace std;

pthread_t *kProductorThreads;
ThreadParams **kProductorParams;
uint32_t kProductorThreadCount = 0;

void *product_thread_func(void *args)
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

    PipeBuffer *buffer = NULL;

    int fd = open(thread_args->filename, O_RDWR | O_CREAT);
    INFO("productor: filename: %s", thread_args->filename);
    // thread_args->print_info();
    clock_t start_time, finish_time;
    do
    {
        start_time = clock();
        // memset(buffer, 0, sizeof(*buffer) * thread_args->buf_size);
        size_t offset = thread_args->buf_size * thread_args->thread_group_count * thread_args->epoch + thread_args->id * thread_args->buf_size;
        thread_args->epoch++;
        buffer = new PipeBuffer(offset, 0, thread_args->buf_size);
        n = pread(fd, buffer->data, thread_args->buf_size, offset);
        // printf("%s", buffer->data);
        if (n > 0)
        {
            buffer->len = n;
            write(thread_args->fd, &buffer, sizeof(PipeBuffer *));
            finish_time = clock();
            INFO("thread: %-33s buffer: %-20p spend time: %lf", thread_args->name, buffer, ((double)(finish_time-start_time))/CLOCKS_PER_SEC);
            // sleep(1);
            continue;
        }
        else if (n < 0)
        {
            ERROR("failed to read %s", thread_args->filename);
        }
        delete buffer;
    }while (n != 0);

    INFO("%s", "fd is closed");

    close(fd);

    return NULL;
}

int init_productor(uint32_t thread_count,int fd, char *filename)
{
    if (fd < 0)
    {
        ERROR("%s", "fd is invalid argument");
        return 1;
    }
    
    kProductorThreadCount = thread_count;
    kProductorThreads = new pthread_t[kProductorThreadCount];
    kProductorParams = new ThreadParams*[kProductorThreadCount];

    char thread_name[FILENAME_LEN];
    for (uint32_t i = 0; i < kProductorThreadCount; ++i)
    {
        memset(thread_name, 0, sizeof(thread_name));
        snprintf(thread_name, sizeof(thread_name) - 1, "%s%d", "productor", i);
        kProductorParams[i] = new ThreadParams(i, fd, kProductorThreadCount, filename, BUFFER_SIZE * 100, thread_name);
        if (pthread_create(kProductorThreads + i, NULL, product_thread_func, (void *)(kProductorParams[i])))
        {
            ERROR("failed to create the thread id %d", kProductorParams[i]->id);
            return 2;
        }
    }
    

    return 0;
}

int uninit_productor()
{

    for (uint32_t i = 0; i < kProductorThreadCount; ++i)
    {
        if (pthread_join(kProductorThreads[i], NULL))
        {
            ERROR("Failed to join the thread id %d", kProductorParams[i]->id);
        }
        delete kProductorParams[i];
    }
    return 0;
}




