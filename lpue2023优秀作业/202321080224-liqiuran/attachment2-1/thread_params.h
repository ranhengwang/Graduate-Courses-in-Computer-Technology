#ifndef _THREAD_PARAMS_H_
#define _THREAD_PARAMS_H_

#include "lengthdef.h"
#include <cstddef>
#include <cstdint>

class ThreadParams
{
public:
    ThreadParams(int id, int fd, uint32_t thread_group_count, char *filename=NULL, size_t buf_size=BUFFER_SIZE, char *name=NULL);
    virtual ~ThreadParams() {}
    int print_info();
public:
    // 线程ID
    uint32_t id;
    // 线程名
    char name[FILENAME_LEN];
    // 线程通信句柄
    int fd;
    // 线程偏移量
    // int offset;
    // 该类线程组的数量
    uint32_t thread_group_count;
    // 当前轮数
    int epoch;
    // buffer 大小
    size_t buf_size;
    // 写入的文件名
    char filename[FILENAME_LEN];
private:
    ThreadParams() {}
};


#endif




