#include "pipe_buffer.h"
#include <cstddef>
#include <unistd.h>
#include <cstring>

PipeBuffer::PipeBuffer(size_t offset, int len, int buffer_size) :
    offset(offset),
    len(len)
{
    data = new char[buffer_size];
    memset(data, 0, sizeof(*data) * buffer_size);
}

PipeBuffer::~PipeBuffer()
{
    delete[] data;
}
