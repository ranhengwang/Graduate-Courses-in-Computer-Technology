#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <typeinfo>
#include <unistd.h>
#include "include/linux/fs/fs.h"
#include "fs/inode.h"
#include "fs/super.h"
#include "include/linux/common_length_protocol.h"
#include "include/debug/macros.h"
#include "net/stream_reciever.h"
#include "tools/thread_manager.h"

using namespace std;

#define TASK_COUNT 3

int main()
{
    init_fs();
    format_fs();
    struct SuperBlock sb;
    read_super_block(&sb);
    print_super_block(&sb);

    init_stream_relative();
    sleep(20);
    uninit_stream_relative();

    return 0;
}

