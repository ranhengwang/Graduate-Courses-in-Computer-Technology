#ifndef _CONSUMER_H_
#define _CONSUMER_H_

#include <cstdint>

int init_consumer(uint32_t thread_count, int fd, char *filename);
int uninit_consumer();

#endif

