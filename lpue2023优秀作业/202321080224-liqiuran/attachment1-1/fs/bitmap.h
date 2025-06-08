#ifndef _BITMAP_H_
#define _BITMAP_H_

#include "../include/linux/fs/fs.h"

/// \brief init_bitmap 初始化位图
///
/// \return 
int init_bitmap();

/// \brief read_bitmap 读取位图
///
/// \param sb_ptr 超级块指针
/// \param bitmap_ptr 位图列表
///
/// \return 
int read_bitmap(const struct SuperBlock *sb_ptr, uint8_t *bitmap_ptr);

/// \brief write_bitmap 写入位图
///
/// \param sb_ptr 超级块指针
/// \param bitmap_ptr 位图列表
/// \param size 位图大小
///
/// \return 
int write_bitmap(const struct SuperBlock *sb_ptr, void *bitmap_ptr, int size);

/// \brief is_valid_inode 判断INode是否有效
///
/// \param sb_ptr 超级块指针
/// \param bitmap_ptr 位图列表
/// \param num 文件编号
///
/// \return 
int is_valid_inode(const struct SuperBlock *sb_ptr, uint8_t *bitmap_ptr, int num);

/// \brief delete_bitmap 删除位图的某个位置
///
/// \param sb_ptr 超级块指针
/// \param bitmap_ptr 位图列表
/// \param num 文件编号
///
/// \return 
int delete_bitmap(const struct SuperBlock *sb_ptr, uint8_t *bitmap_ptr, int num);

/// \brief print_bitmap_array 打印位图
///
/// \param bitmap_array 位图列表
/// \param size 个数
///
/// \return 
int print_bitmap_array(const uint8_t *bitmap_array, uint size);

#endif



