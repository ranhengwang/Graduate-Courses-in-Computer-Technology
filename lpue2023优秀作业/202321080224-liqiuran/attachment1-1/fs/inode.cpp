#include "inode.h"
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include "../include/debug/macros.h"
#include "super.h"
#include <string.h>
#include <unistd.h>

int init_inodes()
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    // 写入inode 节点
    struct INode inodes[INODE_COUNT];
    memset(inodes, 0, sizeof(inodes));
    inodes[0].isValid = 1;
    inodes[0].mode = 0;
    inodes[0].block_num = 0;
    snprintf(inodes[0].filename, sizeof(inodes[0].filename) - 1, "%s", "manager");
    INFO("%s%d", "inode size: ", sizeof(inodes[0]));
    struct SuperBlock sb;
    read_super_block(&sb);
    lseek(fd, sb.super_block_size, SEEK_SET);
    for (int i = 0; i < INODE_COUNT; ++i)
    {
        write(fd, inodes + i, sizeof(*(inodes + i)));
    }
    close(fd);

    return 0;
}

int write_inode(const struct INode *in_ptr, int num)
{
    int fd = open_virt_file(O_RDWR);
    INFO("fd: %d, in_ptr: %p, num: %d", fd, in_ptr, num);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    if (in_ptr == NULL)
    {
        ERROR("%s", "sb_ptr is null");
        return 2;
    }

    if (num < 0 || num >= 64)
    {
        return 3;
    }

    INFO("inode size: %d, writing file: %s, offset: %lu", sizeof(*in_ptr), in_ptr->filename, SUPERBLOCK_SIZE + INODE_SIZE * num);
    size_t offset = SUPERBLOCK_SIZE + INODE_SIZE * num;
    lseek(fd, offset, SEEK_SET);
    write(fd, in_ptr, sizeof(*in_ptr));
    close(fd);

    return 0;
}

int read_inodes(const struct INode *in_ptr)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    if (in_ptr == NULL)
    {
        ERROR("%s", "sb_ptr is null");
        return 2;
    }

    read(fd, (void *)in_ptr, sizeof(*in_ptr));
    close(fd);

    return 0;
}

int read_specific_inode(unsigned int num, const struct INode *in_ptr)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    if (in_ptr == NULL)
    {
        ERROR("%s", "sb_ptr is null");
        return 2;
    }

    if (num < 0)
    {
        ERROR("%s", "now num <= 0, instead of num should be greater than 0");
        return 3;
    }

    struct SuperBlock sb;
    read_super_block(&sb);

    lseek(fd, sb.super_block_size + sb.inode_size * num, SEEK_SET);
    read(fd, (void *)in_ptr, sizeof(*in_ptr));
    close(fd);

    return 0;
}

int print_inode_info(const struct INode *in_ptr)
{
    if (in_ptr == NULL) 
    {
        ERROR("%s", "sb_ptr is NULL");
        return 1;
    }

    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> INode >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("isValid: %d\n", in_ptr->isValid);
    printf("mode: %d\n", in_ptr->mode);
    printf("block_num: %d\n", in_ptr->block_num);
    printf("filename: %s\n", in_ptr->filename);
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< INode <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    return 0;
}
