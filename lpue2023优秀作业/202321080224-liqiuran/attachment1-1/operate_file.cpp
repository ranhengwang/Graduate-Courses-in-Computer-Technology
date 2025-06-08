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
#include "include/linux/fs/fs_size_def.h"
#include "net/stream_reciever.h"
#include "tools/thread_manager.h"

using namespace std;

int main(int argc, char *args[])
{
    if (argc != 2)
    {
        printf("%s\n", "args formation is invalid.");
        return 0;
    }

    char filename[FILENAME_LEN];
    memset(filename, 0, sizeof(filename));
    snprintf(filename, sizeof(filename) - 1, "%s", args[1]);
    printf("%s\n", filename);

    init_fs();

    list_files();
    printf("未删除之前的目录: \n");
    read_file(filename);
    delete_file(filename);
    printf("删除后的目录: \n");
    list_files();
    printf("读取删除的文件的信息: \n");
    read_file(filename);

    return 0;
}

