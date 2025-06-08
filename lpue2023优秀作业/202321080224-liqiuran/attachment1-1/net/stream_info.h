#ifndef _STREAM_INFO_H_
#define _STREAM_INFO_H_

#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "../include/linux/common_length_protocol.h"

class StreamInfo
{
public:
    int set_id(int id);
    int set_name(char *name);
    int append_data(char *data, uint64_t len);
    int set_write_file_des(int fd);
    int print_info();
public:
    StreamInfo();
    StreamInfo(int fd);
    StreamInfo(int fd, int id, char *name);
    virtual ~StreamInfo();

public:
    int id;
    char name[NAME_LENGTH];
    char *data;
    uint64_t data_size;
    int write_file_des;
    time_t time_second;
};


#endif


