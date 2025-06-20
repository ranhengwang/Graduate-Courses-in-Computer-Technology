#include "super.h"
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include "../include/debug/macros.h"
#include <unistd.h>
#include <string.h>

int init_super_block()
{
    struct SuperBlock __super_block;
    memset(&__super_block, 0, sizeof(__super_block));
    __super_block.fs_magic_num = 0x12;
    __super_block.super_block_size = SUPERBLOCK_SIZE;
    __super_block.fs_block_size = FS_BLOCK_SIZE;
    __super_block.inode_count = INODE_COUNT;
    __super_block.inode_size = INODE_SIZE;
    __super_block.inode_remaining_count = INODE_SIZE - 1;
    __super_block.filesize = FILESIZE;
    __super_block.bitmap_size = BITMAP_SIZE;
    __super_block.bitmap_bit_count = BITMAP_SIZE * BYTE_SIZE;
    INFO("%s%d", "superblock size: ", sizeof(__super_block));
    write_super_blcok(&__super_block);
    print_super_block(&__super_block);
    return 0;
}

int read_super_block(struct SuperBlock *sb_ptr)
{
    int fd = open_virt_file(O_RDONLY);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    if (sb_ptr == NULL)
    {
        ERROR("%s", "sb_ptr is null");
        return 2;
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, sb_ptr, sizeof(*sb_ptr));
    close(fd);
    return 0;
}

int write_super_blcok(struct SuperBlock *sb_ptr)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0)
    {
        ERROR("%s", "fd < 0");
        return 1;
    }

    if (sb_ptr == NULL)
    {
        ERROR("%s", "sb_ptr is null");
        return 2;
    }

    lseek(fd, 0, SEEK_SET);
    write(fd, sb_ptr, sizeof(*sb_ptr));
    close(fd);

    return 0;
}

int print_super_block(const struct SuperBlock *sb_ptr)
{
    if (sb_ptr == NULL)
    {
        ERROR("%s", "sb_ptr is NULL");
        return 1;
    }

    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> super block >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("fs_magic_num: %d\n", sb_ptr->fs_magic_num);
    printf("fs_block_size: %d\n", sb_ptr->fs_block_size);
    printf("inode_count: %d\n", sb_ptr->inode_count);
    printf("inode_size: %d\n", sb_ptr->inode_size);
    printf("inode_remaining_count: %d\n", sb_ptr->inode_remaining_count);
    printf("filesize: %d\n", sb_ptr->filesize);
    printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< super block <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
    return 0;
}
