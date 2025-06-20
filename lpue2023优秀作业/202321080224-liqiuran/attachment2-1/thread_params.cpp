#include "thread_params.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include "lengthdef.h"
#include "macros.h"

// ThreadParams::ThreadParams() :
//     id(0),
//     fd(-1),
//     thread_group_count(0),
//     epoch(0)
// {
//     memset(this->name, 0, sizeof(this->name));
//     memset(this->filename, 0, sizeof(this->filename));
// }

ThreadParams::ThreadParams(int id, int fd, uint32_t thread_group_count, char *filename, size_t buf_size, char *name) :
    id(id),
    fd(fd),
    thread_group_count(thread_group_count),
    epoch(0)
{
    if (fd < 0)
    {
        ERROR("%s", "fd passed is invalid");
    }

    if (buf_size <= BUFFER_MAX_SIZE)
    {
        this->buf_size = buf_size;
    }

    memset(this->name, 0, sizeof(this->name));
    if (name != NULL)
    {
        snprintf(this->name, sizeof(this->name) - 1, "%s", name);
    }

    memset(this->filename, 0, sizeof(this->filename));
    if (filename != NULL)
    {
        snprintf(this->filename, sizeof(this->filename) - 1, "%s", filename);
    }
}


int ThreadParams::print_info()
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>> params >>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("id: %u\n", id);
    printf("name: %s\n", name);
    printf("fd: %d\n", fd);
    printf("thread_group_count: %u\n", thread_group_count);
    printf("epoch: %d\n", epoch);
    printf("buf_size: %lu\n", buf_size);
    printf("filename: %s\n", filename);
    printf("<<<<<<<<<<<<<<<<<<<<<<<< params <<<<<<<<<<<<<<<<<<<<<<<<\n");
    return 0;
}

