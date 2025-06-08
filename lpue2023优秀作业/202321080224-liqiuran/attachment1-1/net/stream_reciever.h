#ifndef _STREAM_RECIEVER_H_
#define _STREAM_RECIEVER_H_

#include <pthread.h>
#include "../include/linux/common_length_protocol.h"

int init_stream_relative();
int uninit_stream_relative();

// 流监听的缓冲区个数
#define STREAM_BUFFER_COUNT 2

extern pthread_mutex_t steam_info_mutexs[STREAM_BUFFER_COUNT];


#endif



