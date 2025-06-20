#ifndef _FS_H_
#define _FS_H_

#include <cstddef>
#include <stdio.h>
#include <pthread.h>
#include <string>
#include "../basic_type_redef.h"
#include "fs_size_def.h"

#define B_FILE_SCALE_SIZE 0
#define KB_FILE_SCALE_SIZE 1
#define MB_FILE_SCALE_SIZE 2
#define GB_FILE_SCALE_SIZE 3
#define TB_FILE_SCALE_SIZE 4

/// \brief open_virt_file 打开用于虚拟文件系统的大文件
///
/// \param mode 打开模式
///
/// \return 文件句柄
extern int open_virt_file(int mode);

/// \brief format_fs 格式化文件系统
///
/// \return 成功为0; 否则, 返回-1, 设置errno
extern int format_fs();

/// \brief init_fs 初始化文件系统
///
/// \return 成功为0; 否则, 返回-1, 设置errno
extern int init_fs();

/// \brief list_files 列出文件列表
///
/// \return 成功为0; 否则, 返回-1, 设置errno
extern int list_files();

/// \brief write_file 写入文件
///
/// \return 成功为0; 否则, 返回-1, 设置errno
extern int write_file(char *filename, int len, void *file_binary, int filesize);

/// \brief read_file 读文件
///
/// \return 成功为0; 否则, 返回-1, 设置errno
extern int read_file(char *filename);

/// \brief delete_file 删除文件
///
/// \return 成功为0; 否则, 返回-1, 设置errno
extern int delete_file(char *filename);

/// \brief 超级块
struct SuperBlock
{
    // 文件系统类型
    unsigned int fs_magic_num;
    // 文件系统块大小
    unsigned int fs_block_size;
    // 超级块大小
    unsigned int super_block_size;
    // inode 个数
    unsigned int inode_count;
    // inode 大小 B
    unsigned int inode_size;
    // inode 可用个数
    unsigned int inode_remaining_count;
    // 位图大小 B
    unsigned int bitmap_size;
    // 位图位
    unsigned int bitmap_bit_count;
    // 文件大小: -1: 无限制
    int filesize;
    // 补齐 1kB
    char paddding[988];
};

/// \brief INode 结点
struct INode
{
    uint8_t isValid;
    // 文件类型: 0, 文件; 默认为文件
    uint32_t mode;
    // 文件占用块大小: block_num 为0 , 则文件占用1块, 默认为0.
    uint32_t block_num;
    char filename[FILENAME_LEN];
    // 补齐128KB
    char padding[81];
};

extern pthread_mutex_t bitmap_mutex;
extern pthread_mutex_t inode_mutex;

#endif


