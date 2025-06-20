#include "stream_reciever.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <ctime>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../include/linux/fs/fs.h"
#include "../fs/inode.h"
#include "../fs/super.h"
#include "../tools/thread_manager.h"
#include "../include/debug/macros.h"
#include "../include/linux/basic_type_redef.h"
#include "stream_info.h"

// 流处理需要的线程数
#define STREAM_TASK_COUNT 5

ThreadPool stream_thread_pool;
ThreadTask steam_thread_tasks[STREAM_TASK_COUNT];

// 流管道
int stream_pfd[2];

void clean_stream_info_memory(void *args)
{
    INFO("%s", "success to clean stream info memory");
    StreamInfo ** stream_ptr = (StreamInfo **)args;
    if (*stream_ptr != NULL)
        delete (*stream_ptr);
    return;
}

/// \brief listen_thread_func 流监听线程, 负责监听并将流写入管道
///
/// \param args 参数
///
/// \return 
void *listen_thread_func(void *args)
{
    if (args == NULL)
    {
        ERROR("args is NULL.");
        return NULL;
    }

    StreamInfo *stream_ptr = (StreamInfo *)args;
    
    // 接收数据
    int epoch = STREAM_BUFFER_DATA_LENGTH / 1048576;
    for (int i = 0; i < epoch; i++)
    {
        char data[1048576];
        memset(data, -1, sizeof(data));
        stream_ptr->append_data(data, 1048576);
    }
    sleep(1);

    stream_ptr = NULL;
    return NULL;
}

void *write_steam_thread_func(void *args)
{
    if (args != NULL)
    {
        ERROR("args isn't NULL, but it could ben't used.");
    }

    clock_t start_time, finish_time;
    start_time = clock();
    StreamInfo *stream_ptr = new StreamInfo;
    int n = read(stream_pfd[0], (void *)stream_ptr, sizeof(*stream_ptr));
    if (n > 0)
    {
        // stream_ptr->print_info(); // 程序异常结束可能会有 data 的地址遗漏在 pipe 内
        char filename[FILENAME_LEN];
        memset(filename, 0, sizeof(filename));
        snprintf(filename, sizeof(filename) - 1, "%s-%lu", stream_ptr->name, stream_ptr->time_second);
        write_file(filename, FILESIZE, stream_ptr->data, FILESIZE);
        finish_time = clock();
        printf("%s: %s, spend time %lf seconds\n", "writing stream to the file", filename, ((double)(finish_time - start_time)) / CLOCKS_PER_SEC);
        // list_files();
    }
    else if (n == 0)
    {
        WARNING("pfd is closed");
    }
    else
    {
        ERROR("failed to read stream pfd");
    }

    if (stream_ptr != NULL)
    {
        delete stream_ptr;
        stream_ptr = NULL;
    }
    return NULL;
}

int init_stream_relative()
{
    // 初始化管道
    if (pipe(stream_pfd) < 0)
    {
        ERROR("%s", "Failed to init pipe stream_pfd");
        return 1;
    }

    // 初始化任务
    memset(steam_thread_tasks, 0, sizeof(steam_thread_tasks));
    int listen_task_count = STREAM_BUFFER_COUNT;
    int write_task_count = STREAM_TASK_COUNT - listen_task_count;

    for (int i = 0; i < listen_task_count; ++i)
    {
        StreamInfo *stream_ptr = new StreamInfo(stream_pfd[1]);
        stream_ptr->set_id(i);
        char filename[FILENAME_LEN];
        memset(filename, 0, sizeof(filename));
        snprintf(filename, sizeof(filename) - 1, "%s%d", "filename", i);
        stream_ptr->set_name(filename);
        steam_thread_tasks[i].exec_func = listen_thread_func;
        steam_thread_tasks[i].exec_args = (void *)stream_ptr;
    }

    for (int i = 0; i < write_task_count; ++i)
    {
        steam_thread_tasks[listen_task_count + i].exec_func = write_steam_thread_func;
    }

    // 初始化线程
    stream_thread_pool.init(steam_thread_tasks, STREAM_TASK_COUNT);
    return 0;
}

int uninit_stream_relative()
{
    close(stream_pfd[1]);
    close(stream_pfd[0]);
    stream_thread_pool.uninit();
    return 0;
}


