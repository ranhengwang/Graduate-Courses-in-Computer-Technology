#ifndef _PIPE_BUFFER_H_
#define _PIPE_BUFFER_H_
#include <cstddef>
#include <cstdint>

class PipeBuffer
{
public:
    PipeBuffer(size_t offset, int len, int buffer_size);
    ~PipeBuffer();
public:
    size_t offset;
    uint32_t len;
    char *data;
};

#endif

