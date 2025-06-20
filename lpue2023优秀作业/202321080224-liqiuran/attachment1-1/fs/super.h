#ifndef _SUPER_H_
#define _SUPER_H_

#include "../include/linux/fs/fs.h"

/// \brief init_super_block 初始化超级块
///
/// \return 成功为0; 否则, 返回-1, 设置errno
int init_super_block();

/// \brief read_super_block 读出超级块
///
/// \param sb_ptr 超级块存放位置
///
/// \return 
int read_super_block(struct SuperBlock *sb_ptr);

/// \brief write_super_blcok 写入超级块到文件系统
///
/// \param sb_ptr 超级块存放位置
///
/// \return 
int write_super_blcok(struct SuperBlock *sb_ptr);

/// \brief print_super_block 输出超级块信息
///
/// \param sb 超级块位置
///
/// \return 
int print_super_block(const struct SuperBlock *sb);

#endif



