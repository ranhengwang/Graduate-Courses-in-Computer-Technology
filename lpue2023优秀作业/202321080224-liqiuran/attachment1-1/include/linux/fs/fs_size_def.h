#ifndef _FS_SIZE_DEF_H_
#define _FS_SIZE_DEF_H_

//{{{> 文件系统相关
// 文件系统存储空间大小 4G = 4 * 2^30 B = 4294967296 B
// 其中以64MB 进行划分, 第一块用来管理文件系统, 其他的63块用来存储文件, 每个文件大小为64MB
#define FS_STORY_SIZE 4294967296
// 文件系统块大小 64MB = 64 * 2^20 B = 67108864 B
#define FS_BLOCK_SIZE 67108864
//<}}}

//{{{> 超级块相关
// 超级块大小 1KB
#define SUPERBLOCK_SIZE 1024
//<}}}

//{{{> inode 相关
// iNode 所占空间大小: 128B, 其中: 块号占: log(FS_STORY_SIZE / FS_BLOCK_SIZE) 位
#define INODE_SIZE 128
// iNode 数量: FS_STORY_SIZE / FS_BLOCK_SIZE 个
#define INODE_COUNT 64
// iNodes 所占空间大小: INODE_SIZE * INODE_COUNT B
#define INODES_SIZE 8064
//<}}}

//{{{> 位图相关
// 位图大小 B, 从低到高, 且第0位永远是1
#define BITMAP_SIZE 8
//<}}}

//{{{> 目录项相关
// 目录项大小 64B
#define DENTRY_SIZE 64
//<}}}

//{{{> 文件相关
// 文件名长度 64B
#define FILENAME_LEN 32
// 文件大小
#define FILESIZE 67108864
//<}}}

//{{{> B, KB...大小
#define BYTE_SIZE 8
//<}}}
#endif



