#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <exception>
#include <pthread.h>
#include <regex>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "../include/linux/fs/fs.h"
#include "../include/debug/macros.h"
#include "../tools/thread_manager.h"
#include "inode.h"
#include "bitmap.h"
#include "super.h"

pthread_mutex_t bitmap_mutex;
pthread_mutex_t inodes_mutex[INODE_COUNT];

int open_virt_file(int mode)
{
    char virt_filename[32];
    memset(virt_filename, 0, sizeof(virt_filename));
    snprintf(virt_filename, sizeof(virt_filename) - 1, "%s", "virt_big_file");
    int fd = open(virt_filename, mode);
    if (fd < 0)
    {
        ERROR("%s", "failed to open virt_big_file");
    }

    return fd;
}

int create_big_file(const char *filename, int size, unsigned short scale)
{
    if (scale > GB_FILE_SCALE_SIZE)
    {
        ERROR("文件规模太大, 无法满足");
        return 1;
    }

    long file_size = size;
    for (int i = 0; i < scale; ++i)
    {
        file_size <<= 10;
    }

    INFO("runing create_big_file");

    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        ERROR("无法打开/创建文件");
        return 1;
    }

    if (ftruncate(fd, file_size) == -1)
    {
        ERROR("failed to call ftruncate");
        close(fd);
        return 1;
    }

    INFO("文件%s已创建, 大小: %ldB", filename, file_size);
    close(fd);

    return 0;
}

int init_fs()
{
    pthread_mutex_init(&bitmap_mutex, NULL);
    for (int i = 0; i < INODE_COUNT; ++i)
    {
        pthread_mutex_init(inodes_mutex + i, NULL);
    }

    return 0;
}

int format_fs()
{
    // 虚拟文件名
    char virt_filename[32];
    memset(virt_filename, 0, sizeof(virt_filename));
    snprintf(virt_filename, sizeof(virt_filename) - 1, "%s", "virt_big_file");

    int fd = open(virt_filename, O_RDWR);
    if (fd > 0)
    {
        close(fd);
        return 0;
    }

    while (fd < 0)
    {
        INFO("creating big file");
        create_big_file(virt_filename, 4, GB_FILE_SCALE_SIZE);
        fd = open(virt_filename, O_RDWR);
    }
    close(fd);

    init_super_block();
    init_inodes();
    init_bitmap();

    return 0;
}

int list_files()
{
    ERROR("%s", "enter list_files");
    int fd = open_virt_file(O_RDONLY);
    if (fd < 0) return 1;

    struct SuperBlock sb;
    read_super_block(&sb);
    size_t offset = sb.super_block_size + sb.inode_size * sb.inode_count;
    lseek(fd, offset, SEEK_SET);
    uint8_t *bitmap_array = new uint8_t[sb.bitmap_size];
    int bitmap_size = sb.bitmap_size;
    pthread_mutex_lock(&bitmap_mutex);
    read_bitmap(&sb, bitmap_array);

    int cur_inode_num = 0;
    printf(">>>>>>>>>>>>>>>>>>>>Dirs>>>>>>>>>>>>>>>>>>>>>>\n");
    for (int i = bitmap_size - 1; i >= 0; i--)
    {
        uint8_t flag = 1;
        for (uint j = 0; j < 8; j++)
        {
            // 跳过 manager inode
            if (i == bitmap_size - 1 && j == 0)
            {
                ++cur_inode_num;
                flag <<= 1;
                continue;
            }
            if ((bitmap_array[i] & flag) != 0)
            {
                struct INode cur_inode;
                read_specific_inode(cur_inode_num, &cur_inode);
                printf("%s\n", cur_inode.filename);
            }
            flag <<= 1;
            ++cur_inode_num;
        }
    }
    printf("<<<<<<<<<<<<<<<<<<<<Dirs<<<<<<<<<<<<<<<<<<<<<<\n");
    delete[] bitmap_array;
    bitmap_array = NULL;
    pthread_mutex_unlock(&bitmap_mutex);
    close(fd);
    ERROR("%s", "quit list_files");
    return 0;
}



int write_file(char *filename, int len, void *file_binary, int filesize)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0) return 1;

    struct SuperBlock sb;
    read_super_block(&sb);

    // 查找可以的inode
    uint8_t *bitmap_array = new uint8_t[sb.bitmap_size];
    memset(bitmap_array, 0, sb.bitmap_size);
    pthread_mutex_lock(&bitmap_mutex);
    read_bitmap(&sb, bitmap_array);
    int bitmap_size = sb.bitmap_size;
    int cur_inode_num = -1;
    int col = 8;
    for (int i = bitmap_size - 1; i >= 0; i--)
    {
        uint8_t flag = 1;
        for (uint j = 0; j < col; j++)
        {
            // 跳过 manager inode
            if (i == bitmap_size - 1 && j == 0)
            {
                flag <<= 1;
                continue;
            }
            if ((bitmap_array[i] & flag) == 0)
            {
                cur_inode_num = (bitmap_size - 1 - i) * col + j;
                INFO("cur_inode_num: %d", cur_inode_num);
                break;
            }
            flag <<= 1;
        }

        if (cur_inode_num > 0)
        {
            break;
        }
    }
    
    if (cur_inode_num < 64)
    {
        INFO("%d is available", cur_inode_num);
    }
    else
    {
        ERROR("space isn't available, cur_inode_num: %d", cur_inode_num);
        delete[] bitmap_array;
        bitmap_array = NULL;
        pthread_mutex_unlock(&bitmap_mutex);
        return 3;
    }

    // 更新 bitmap
    int second_offset = cur_inode_num / 8;
    int first_offset = cur_inode_num % 8;
    bitmap_array[bitmap_size - second_offset - 1] |= (1 << first_offset);
    write_bitmap(&sb, bitmap_array, bitmap_size);
    delete[] bitmap_array;
    bitmap_array = NULL;
    delete[] bitmap_array;
    pthread_mutex_unlock(&bitmap_mutex);

    // inode 更新
    pthread_mutex_lock(inodes_mutex + cur_inode_num);
    struct INode inode;
    memset(&inode, 0, sizeof(inode));
    inode.isValid = 1;
    inode.block_num = cur_inode_num;
    int size = sizeof(inode.filename);
    snprintf(inode.filename, size - 1, "%s", filename);
    write_inode(&inode, cur_inode_num);
    pthread_mutex_unlock(inodes_mutex + cur_inode_num);

    // 写入文件
    if (file_binary == NULL || filesize <= 0)
    {
        ERROR("%s", "file binary is null");
        return 4;
    }
    else
    {
        size_t offset = sb.super_block_size
            + sb.inode_size * sb.inode_count
            + sb.bitmap_size
            + sb.filesize * inode.block_num;
        lseek(fd, offset, SEEK_SET);
        write(fd, file_binary, filesize);
    }

    return 0;
}

int read_file(char *filename)
{
    int fd = open_virt_file(O_RDONLY);
    if (fd < 0) return 1;
    int len = strlen(filename);
    if (len == 0) return 2;

    struct SuperBlock sb;
    read_super_block(&sb);

    uint8_t *bitmap_array = new uint8_t[sb.bitmap_size];
    pthread_mutex_lock(&bitmap_mutex);
    read_bitmap(&sb, bitmap_array);
    int file_block_num = 0;
    for (uint i = 1; i < sb.bitmap_bit_count; ++i)
    {
        if (is_valid_inode(&sb, bitmap_array, i) == 1)
        {
            struct INode inode;
            read_specific_inode(i, &inode);
            if (0 == strcmp(filename, inode.filename))
            {
                file_block_num = inode.block_num;
                INFO("find the file, the num of block storing file : %d", file_block_num);
            }
        }
    }

    if (file_block_num == 0)
    {
        ERROR("failed to find the file, please check if you filename(%s) exist...", filename);
        delete[] bitmap_array;
        bitmap_array = NULL;
        pthread_mutex_unlock(&bitmap_mutex);
        return 4;
    }

    delete[] bitmap_array;
    bitmap_array = NULL;
    pthread_mutex_unlock(&bitmap_mutex);

    // store the file
    char *buffer = new char[sb.filesize];
    memset(buffer, 0, sb.filesize * sizeof(*buffer));
    size_t offset = sb.super_block_size + sb.inode_size * sb.inode_count + sb.filesize * file_block_num;
    lseek(fd, offset, SEEK_SET);
    read(fd, buffer, sb.filesize);

    int wfd = open(filename, O_RDWR | O_CREAT);
    lseek(wfd, 0, SEEK_SET);
    write(wfd, buffer, sb.filesize);
    close(fd);
    return 0;
}

int delete_file(char *filename)
{
    int fd = open_virt_file(O_RDWR);
    if (fd < 0) return 1;
    if (filename == NULL) return 2;

    struct SuperBlock sb;
    read_super_block(&sb);

    uint8_t *bitmap_array = new uint8_t[sb.bitmap_size];
    memset(bitmap_array, 0, sizeof(*bitmap_array) * sb.bitmap_size);
    pthread_mutex_lock(&bitmap_mutex);
    read_bitmap(&sb, bitmap_array);
    int cur_inode_num = 0;
    for (uint i = 1; i < sb.bitmap_bit_count; ++i)
    {
        if (is_valid_inode(&sb, bitmap_array, i) == 1)
        {
            struct INode inode;
            read_specific_inode(i, &inode);
            if (0 == strcmp(filename, inode.filename))
            {
                INFO("find the file, deleting the file: %s", filename);
                cur_inode_num = i;
            }
        }
    }

    if (cur_inode_num == 0)
    {
        ERROR("Failed to the file, because don't find the file, please to check if the file exist...");
        delete[] bitmap_array;
        bitmap_array = NULL;
        pthread_mutex_unlock(&bitmap_mutex);
        return 3;
    }
    
    delete_bitmap(&sb, bitmap_array, cur_inode_num);
    delete[] bitmap_array;
    bitmap_array = NULL;
    pthread_mutex_unlock(&bitmap_mutex);

    // delete_inode
    struct INode inode;
    memset(&inode, 0, sizeof(inode));
    write_inode(&inode, cur_inode_num);

    close(fd);
    return 0;
}



