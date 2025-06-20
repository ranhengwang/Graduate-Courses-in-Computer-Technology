#ifndef _INODE_H_
#define _INODE_H_

#include "../include/linux/fs/fs.h"

/// \brief init_inodes 初始化INode节点
///
/// \return 
int init_inodes();

/// \brief write_inode 将INode节点信息写入文件
///
/// \param in_ptr INode 列表
/// \param num 个数
///
/// \return 
int write_inode(const struct INode *in_ptr, int num);

/// \brief read_inodes 从文件中读出所有的INode节点
///
/// \param in_ptr INode 列表
///
/// \return 
int read_inodes(const struct INode *in_ptr);

/// \brief read_specific_inode 从文件中读出特定的Inode节点
///
/// \param num inode 号
/// \param in_ptr Inode 存储位置
///
/// \return 
int read_specific_inode(unsigned int num, const struct INode *in_ptr);

/// \brief print_inode_info 打印INode节点信息
///
/// \param in_ptr INode
///
/// \return 
int print_inode_info(const struct INode *in_ptr);

#endif




