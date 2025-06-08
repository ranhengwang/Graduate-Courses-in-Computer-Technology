#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>


void infoMessage(std::string s);
void errorMessage(const std::exception &e);
void errorMessage(std::string s);
void stdMessage(std::string s);

// manually set
const u_int DATA_BLOCK_SIZE = 64 * 1024; // 64KB < 2^32
const u_int DATA_BLOCK_NUM = 1024;       // 1024 < 2^31
const u_int INODE_BLOCK_NUM = 128;       // 128
const u_int MAX_FILE_NAME = 16;

// typedef
typedef u_int64_t FileSize;
typedef u_int64_t MetaFilePos;
typedef u_int BlockIndex;
typedef u_int DataSize;
typedef char FileName[MAX_FILE_NAME];

// Data Block struct
const u_int DATA_SIZE = DATA_BLOCK_SIZE - sizeof(BlockIndex) - sizeof(DataSize);
typedef struct
{
    DataSize filled_data;
    u_char data[DATA_SIZE];
    BlockIndex nextBlock;
} DataBlock, *pDataBlock;
const u_int TOTAL_DATA_SIZE = DATA_BLOCK_SIZE * DATA_BLOCK_NUM;

// Inode Block struct
typedef struct
{
    FileName fileName;
    FileSize fileSize;
    BlockIndex firstBlock;
} InodeBlock, *pInodeBlock;
const u_int INODE_BLOCK_SIZE = sizeof(InodeBlock);
const u_int TOTAL_INODE_SIZE = sizeof(InodeBlock) * INODE_BLOCK_NUM;

// Super Block struct
const u_int IAT_size = INODE_BLOCK_NUM / 8;
const u_int DAT_size = DATA_BLOCK_NUM / 8;
typedef struct
{
    u_char IAT[IAT_size]; // InodeBlock Allocation Table
    u_char DAT[DAT_size]; // Data Allocation Table
} SuperBlock, *pSuperBlock;
const u_int SUPER_BLOCK_SIZE = sizeof(SuperBlock);

class FileSystem
{
public:
    FileSystem();
    ~FileSystem();

    bool create(const std::string &filename);
    void del(const std::string &filename);
    int write(const std::string &fileName, const std::string &content);
    int copyFrom(const std::string &fileName, const std::string &path);
    int copyTo(const std::string &fileName, const std::string &path);
    int read(const std::string &fileName);
    void list();

private:
    std::fstream fsFile;
    SuperBlock superBlock;
    std::unordered_map<std::string, BlockIndex> cwd;

    void printMetaInfo();
    void reconstructDir();
    void readSuperBlock();
    void writeSuperBlock();
    void readInodeBlock(InodeBlock *inode_block, BlockIndex inodeNum);
    void writeInodeBlock(InodeBlock *inode_block, BlockIndex inodeNum);
    void readDataBlock(DataBlock *data_block, BlockIndex blockNum);
    void writeDataBlock(DataBlock *data_block, BlockIndex blockNum);
    void deleteDataBlock(BlockIndex blockNum);
    MetaFilePos getFirstAvailableInodeBlock();
    MetaFilePos getFirstAvailableDataBlock();
    MetaFilePos getDataBlockPos(BlockIndex blockNum);
    MetaFilePos getInodeBlockPos(BlockIndex blockNum);
};

#endif // FILESYSTEM_HPP