#include "stream_info.h"
#include <cstddef>
#include <cstdio>
// #include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include "../include/debug/macros.h"
#include "../include/linux/fs/fs_size_def.h"
#include "stream_reciever.h"

StreamInfo::StreamInfo() :
    data(NULL)
{
    id = 0;
    memset(name, 0, sizeof(name));
    data_size = 0;
    time(&time_second);
}

StreamInfo::StreamInfo(int fd) :
    data(NULL),
    write_file_des(fd)
{
    id = 0;
    memset(name, 0, sizeof(name));
    data_size = 0;
    time(&time_second);
}

StreamInfo::StreamInfo(int fd, int id, char *name) :
    data(NULL),
    write_file_des(fd)
{
    this->id = id;
    snprintf(this->name, sizeof(this->name), "%s", name);
    data_size = 0;
    time(&time_second);
}

StreamInfo::~StreamInfo()
{
    if (this->data != NULL)
    {
        delete[] data;
        data = NULL;
    }
}

int StreamInfo::print_info()
{
    printf("\n>>>>>>>>>>>>>>>>> stream info >>>>>>>>>>>>>>>>>>\n");
    printf("id: %d\n", id);
    printf("data: %p\n", this->data);
    printf("name: %p\n", name);
    printf("<<<<<<<<<<<<<<<<< stream info <<<<<<<<<<<<<<<<<<\n\n");
    return 0;
}

int StreamInfo::set_id(int id)
{
    this->id = id;
    return 0;
}

int StreamInfo::set_name(char *name)
{
    if (name == NULL)
    {
        memset(this->name, 0, sizeof(this->name));
        return 1;
    }

    snprintf(this->name, sizeof(this->name) - 1, "%s", name);
    return 0;
}

int StreamInfo::append_data(char *data, uint64_t len)
{
    if (data == NULL || len >= STREAM_BUFFER_DATA_LENGTH)
    {
        return 1;
    }
    
    if (this->data == NULL)
    {
        this->data = new char[STREAM_BUFFER_DATA_LENGTH];
    }

    uint64_t remain_volume = STREAM_BUFFER_DATA_LENGTH - data_size;
    if (remain_volume > len)
    {
        memcpy(this->data + data_size, data, len);
        data_size += len;
        return 0;
    }

    // 超过大小后
    INFO("writing the file info to the pipe");
    memcpy(this->data + data_size, data, remain_volume);

    if (write_file_des < 0)
    {
        ERROR("%s, data: %s", "fd is unavailable", this->data);
        return 1;
    }
    else
    {
        time(&time_second);
        write(write_file_des, this, sizeof(*this));
        this->data = NULL;
    }

    data_size = len - remain_volume;
    if (data_size > 0)
    {
        this->data = new char[STREAM_BUFFER_DATA_LENGTH];
        snprintf(this->data, data_size, "%s", data + remain_volume);
    }
    return 0;
}

int StreamInfo::set_write_file_des(int fd)
{
    if (fd < 0)
    {
        ERROR("%s", "fd is unavailable");
        return 1;
    }

    write_file_des = fd;
    return 0;
}
