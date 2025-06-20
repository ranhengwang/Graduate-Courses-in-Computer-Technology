#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cstring>
#include <filesystem>
#include <memory>
#include "filesystem.hpp"

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string BLUE = "\033[34m";
const std::string RESET = "\033[0m";
void infoMessage(std::string s)
{
    std::cout << GREEN << s << RESET << std::endl;
}
void errorMessage(const std::exception &e)
{
    std::cerr << RED << "Exception: " << e.what() << RESET << std::endl;
}
void errorMessage(std::string s)
{
    std::cerr << RED << s << RESET << std::endl;
}
void stdMessage(std::string s)
{
    std::cerr << BLUE << s << RESET << std::endl;
}

u_int countUsedBit(u_char *c, u_int len)
{
    u_int count = 0;
    for (u_int i = 0; i < len; i++)
    {
        for (u_int j = 0; j < 8; j++)
        {
            if (c[i] & (1 << j))
                count++;
        }
    }
    return count;
}
bool getFirstAvailableBit(u_char *c, u_int len, BlockIndex *number)
{
    u_int count = 0;
    for (u_int i = 0; i < len; i++)
    {
        for (u_int j = 0; j < 8; j++)
        {
            if (!(c[i] & (1 << j)))
            {
                *number = count;
                return true;
            }
            count++;
        }
    }
    return false;
}
void setBit(u_char *c, BlockIndex index)
{
    BlockIndex offset = index % 8;
    c[index / 8] |= (1 << offset);
}
void unsetBit(u_char *c, BlockIndex index)
{
    BlockIndex offset = index % 8;
    c[index / 8] ^= (1 << offset);
}
bool testBit(u_char *c, BlockIndex index)
{
    BlockIndex offset = index % 8;
    return c[index / 8] & (1 << offset);
}

#ifdef INFO
void showBit(char *p, int len)
{
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            std::cout << ((p[i] & (1 << j)) ? '1' : '0');
        }
        if (i % 8 == 7)
            std::cout << std::endl;
        else
            std::cout << ' ';
    }
}

void FileSystem::printMetaInfo()
{
    std::cout << std::endl
              << "********** Super Block **********" << std::endl;
    std::cout << "used inode blocks: " << countUsedBit(superBlock.IAT, IAT_size) << std::endl;
    showBit((char *)superBlock.IAT, IAT_size);
    std::cout << "used data blocks: " << countUsedBit(superBlock.DAT, DAT_size) << std::endl;
    showBit((char *)superBlock.DAT, DAT_size);
    std::cout << "*********************************" << std::endl
              << std::endl;
}
#endif

// Meta File Methods
MetaFilePos FileSystem::getDataBlockPos(BlockIndex blockNum)
{
    return SUPER_BLOCK_SIZE + TOTAL_INODE_SIZE + DATA_BLOCK_SIZE * blockNum;
}
MetaFilePos FileSystem::getInodeBlockPos(BlockIndex inodeNum)
{
    return SUPER_BLOCK_SIZE + INODE_BLOCK_SIZE * inodeNum;
}

void FileSystem::writeSuperBlock()
{
    fsFile.seekg(std::ios::beg);
    fsFile.write((char *)&superBlock, SUPER_BLOCK_SIZE);
}
void FileSystem::readSuperBlock()
{
    fsFile.seekg(std::ios::beg);
    fsFile.read((char *)&superBlock, SUPER_BLOCK_SIZE);
}

void FileSystem::writeInodeBlock(InodeBlock *inode_block, BlockIndex inodeNum)
{
    std::cout << "write inode block: " << inodeNum << std::endl;
    fsFile.seekg(getInodeBlockPos(inodeNum), std::ios::beg);
    fsFile.write((char *)inode_block, INODE_BLOCK_SIZE);
}
void FileSystem::readInodeBlock(InodeBlock *inode_block, BlockIndex inodeNum)
{
    fsFile.seekg(getInodeBlockPos(inodeNum), std::ios::beg);
    fsFile.read((char *)inode_block, INODE_BLOCK_SIZE);
}

void FileSystem::writeDataBlock(DataBlock *data_block, BlockIndex blockNum)
{
    fsFile.seekg(getDataBlockPos(blockNum), std::ios::beg);
    fsFile.write((char *)data_block, DATA_BLOCK_SIZE);
}
void FileSystem::readDataBlock(DataBlock *data_block, BlockIndex blockNum)
{
    fsFile.seekg(getDataBlockPos(blockNum), std::ios::beg);
    fsFile.read((char *)data_block, DATA_BLOCK_SIZE);
}
void FileSystem::deleteDataBlock(BlockIndex blockNum)
{
    DataBlock data_block;
    BlockIndex next_block_index;
    readDataBlock(&data_block, blockNum);
    while (data_block.nextBlock != blockNum)
    {
        next_block_index = data_block.nextBlock;
        unsetBit(superBlock.DAT, blockNum);
        blockNum = next_block_index;
        readDataBlock(&data_block, blockNum);
    }
    unsetBit(superBlock.DAT, blockNum);
}

void FileSystem::reconstructDir()
{
    cwd.clear();
    BlockIndex count = 0;
    InodeBlock inode_block;
    for (BlockIndex i = 0; i < INODE_BLOCK_NUM; i++)
    {
        if (superBlock.IAT[i / 8] & (1 << (i % 8)))
        {
            readInodeBlock(&inode_block, count);
            cwd[inode_block.fileName] = i;
        }
        count++;
    }
}

FileSystem::FileSystem()
{
    std::string fsPath = "filesystem.bin";
    memset(&superBlock, 0, SUPER_BLOCK_SIZE);
    if (!std::filesystem::exists(fsPath))
    {
        infoMessage("Meta File does not exist, create a new one");
        fsFile.open(fsPath, std::ios::out | std::ios::binary);
        fsFile.write((char *)&superBlock, SUPER_BLOCK_SIZE);
        fsFile.close();
    }
    fsFile.open(fsPath, std::ios::in | std::ios::out | std::ios::binary);
    readSuperBlock();
    reconstructDir();
#ifdef DEBUG
    printMetaInfo();
#endif
}
FileSystem::~FileSystem()
{
    writeSuperBlock();
    fsFile.close();
}

void FileSystem::list()
{
    std::cout << BLUE << std::left << std::setw(16) << "Name" << std::left << std::setw(10) << "Inode" << std::left << std::setw(10) << "Size" << RESET << std::endl;
    InodeBlock inode_block;
    for (const auto &pair : cwd)
    {
        std::string file_name = pair.first;
        readInodeBlock(&inode_block, pair.second);
        u_int file_size = inode_block.fileSize;
        std::cout << BLUE << std::left << std::setw(16) << file_name << std::left << std::setw(10) << pair.second << std::left << std::setw(10) << file_size << RESET << std::endl;
    }
}
bool FileSystem::create(const std::string &fileName)
{
    infoMessage("Try to create file: " + fileName);
    try
    {
        if (fileName.length() > MAX_FILE_NAME - 1)
        {
            errorMessage("File name too long");
        }
        if (cwd.find(fileName) != cwd.end())
        {
            errorMessage("File exists!");
        }
    }
    catch (const std::exception &e)
    {
        errorMessage(e);
        return false;
    }
    BlockIndex inode_index;
    getFirstAvailableBit(superBlock.IAT, INODE_BLOCK_NUM, &inode_index);
    BlockIndex data_index;
    getFirstAvailableBit(superBlock.DAT, DATA_BLOCK_NUM, &data_index);

    cwd[fileName] = inode_index;
    setBit(superBlock.IAT, inode_index);
    setBit(superBlock.DAT, data_index);
    writeSuperBlock();

    InodeBlock inode_block;
    memset(&inode_block.fileName, 0, INODE_BLOCK_SIZE);
    memcpy(&inode_block.fileName, fileName.c_str(), fileName.length());
    inode_block.fileSize = 0;
    inode_block.firstBlock = data_index;
    writeInodeBlock(&inode_block, inode_index);

    DataBlock data_block;
    memset(&data_block, 0, DATA_BLOCK_SIZE);
    data_block.nextBlock = data_index;
    data_block.filled_data = 0;
    writeDataBlock(&data_block, data_index);

#ifdef DEBUG
    printMetaInfo();
#endif
    return true;
}
void FileSystem::del(const std::string &fileName)
{
    try
    {
        if (fileName.length() > MAX_FILE_NAME - 1)
        {
            throw std::length_error("File name too long");
        }
        if (cwd.find(fileName) == cwd.end())
        {
            throw std::length_error("File does not exist!");
        }
    }
    catch (const std::exception &e)
    {
        errorMessage(e);
        return;
    }
    BlockIndex inode_index = cwd[fileName];
    cwd.erase(fileName);
    infoMessage("inode block: " + std::to_string(inode_index));
    InodeBlock inode_block;
    readInodeBlock(&inode_block, inode_index);
    infoMessage("Try to delete file: " + std::string(inode_block.fileName));
    deleteDataBlock(inode_block.firstBlock);
    unsetBit(superBlock.IAT, inode_index);
    writeSuperBlock();
#ifdef DEBUG
    printMetaInfo();
#endif
}
int FileSystem::write(const std::string &fileName, const std::string &content)
{
    infoMessage("Try to write file: " + fileName);
    infoMessage("Content: " + content);
    DataSize content_size = content.length();
    try
    {
        if (fileName.length() > MAX_FILE_NAME - 1)
        {
            throw std::length_error("File name too long");
        }
        if (cwd.find(fileName) == cwd.end())
        {
            throw std::length_error("File does not exist!");
        }
        FileSize remain_size = (DATA_BLOCK_NUM - countUsedBit(superBlock.DAT, DAT_size) + 1) * DATA_SIZE;
        if (content_size > remain_size)
        {
            throw std::runtime_error("Not enough remaining storage space!");
        }
    }
    catch (const std::exception &e)
    {
        errorMessage(e);
        return -1;
    }
    BlockIndex inode_index = cwd[fileName];
    InodeBlock inode_block;
    readInodeBlock(&inode_block, inode_index);
    BlockIndex data_index = inode_block.firstBlock;
    DataBlock data_block;
    DataSize total_write_size = 0;
    DataSize current_write_size = 0;
    if (inode_block.fileSize > 0)
    {
        readDataBlock(&data_block, data_index);
        while (data_block.nextBlock != data_index)
        {
            data_index = data_block.nextBlock;
            readDataBlock(&data_block, data_index);
        }
        current_write_size = std::min(content_size, DATA_SIZE - data_block.filled_data);
        total_write_size += current_write_size;
        memcpy(&data_block.data[data_block.filled_data], content.c_str(), current_write_size);
        data_block.filled_data += current_write_size;
        if (total_write_size < content_size)
        {
            BlockIndex next_data_index;
            getFirstAvailableBit(superBlock.DAT, DAT_size, &next_data_index);
            data_block.nextBlock = next_data_index;
            setBit(superBlock.DAT, next_data_index);
            data_index = next_data_index;
        }
        writeDataBlock(&data_block, data_index);
    }
    while (total_write_size < content_size)
    {
        memset(&data_block, 0, DATA_BLOCK_SIZE);
        current_write_size = std::min(content_size - total_write_size, DATA_SIZE);
        memcpy(&data_block.data, content.c_str() + total_write_size, current_write_size);
        data_block.filled_data = current_write_size;
        total_write_size += current_write_size;
        if (total_write_size < content_size)
        {
            BlockIndex next_data_index;
            getFirstAvailableBit(superBlock.DAT, DAT_size, &next_data_index);
            data_block.nextBlock = next_data_index;
            setBit(superBlock.DAT, next_data_index);
            writeDataBlock(&data_block, data_index);
            data_index = next_data_index;
        }
        else
        {
            data_block.nextBlock = data_index;
            writeDataBlock(&data_block, data_index);
        }
    }
    inode_block.fileSize += content_size;
    writeInodeBlock(&inode_block, inode_index);
    return total_write_size;
}
int FileSystem::read(const std::string &fileName)
{
    infoMessage("Try to read file: " + fileName);
    try
    {
        if (fileName.length() > MAX_FILE_NAME - 1)
        {
            throw std::length_error("File name too long");
        }
        if (cwd.find(fileName) == cwd.end())
        {
            throw std::length_error("File does not exist!");
        }
    }
    catch (const std::exception &e)
    {
        errorMessage(e);
        return -1;
    }
    BlockIndex inode_index = cwd[fileName];
    InodeBlock inode_block;
    readInodeBlock(&inode_block, inode_index);
    BlockIndex data_index = inode_block.firstBlock;
    DataBlock data_block;
    memset(&data_block, 0, DATA_BLOCK_SIZE);
    DataSize total_read_data_size = 0;
    DataSize read_data_size = 0;
    while (total_read_data_size < inode_block.fileSize)
    {
        readDataBlock(&data_block, data_index);
        read_data_size = std::min(inode_block.fileSize - total_read_data_size, (FileSize)DATA_SIZE);
        std::cout.write((const char *)data_block.data, read_data_size);
        total_read_data_size += read_data_size;
        if (total_read_data_size < inode_block.fileSize)
        {
            data_index = data_block.nextBlock;
        }
    }
    std::cout << std::endl;
    return total_read_data_size;
}
int FileSystem::copyFrom(const std::string &fileName, const std::string &path)
{
    if (!std::filesystem::exists(path.c_str()))
    {
        errorMessage("File not exists.");
        return -1;
    }
    std::ifstream file(path.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        errorMessage("failed to open file.");
        return -1;
    }
    if (!create(fileName))
    {
        return -1;
    }
    file.seekg(0, std::ios::end);        // 将文件位置指针重置到文件结束
    std::streamsize size = file.tellg(); // 获取文件大小
    file.seekg(0, std::ios::beg);        // 将文件位置指针重置到文件开始

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size))
    {
        errorMessage("Error reading file.");
    }
    write(fileName, std::string(buffer.data(), size));
    return size;
}
int FileSystem::copyTo(const std::string &fileName, const std::string &path)
{
    if (std::filesystem::exists(path.c_str()))
    {
        errorMessage("File exists.");
        return -1;
    }
    std::ofstream file(path.c_str(), std::ios::binary); // 以二进制模式打开文件
    if (!file.is_open())
    {
        errorMessage("Unable to open file.");
        return -1;
    }

    infoMessage("Try to read file: " + fileName);
    try
    {
        if (cwd.find(fileName) == cwd.end())
        {
            errorMessage("File does not exist!");
        }
    }
    catch (const std::exception &e)
    {
        errorMessage(e);
        return -1;
    }
    BlockIndex inode_index = cwd[fileName];
    InodeBlock inode_block;
    readInodeBlock(&inode_block, inode_index);
    BlockIndex data_index = inode_block.firstBlock;
    DataBlock data_block;
    memset(&data_block, 0, DATA_BLOCK_SIZE);
    DataSize total_read_data_size = 0;
    DataSize read_data_size = 0;
    while (total_read_data_size < inode_block.fileSize)
    {
        readDataBlock(&data_block, data_index);
        read_data_size = std::min(inode_block.fileSize - total_read_data_size, (FileSize)DATA_SIZE);
        file.write((const char *)data_block.data, read_data_size);
        total_read_data_size += read_data_size;
        if (total_read_data_size < inode_block.fileSize)
        {
            data_index = data_block.nextBlock;
        }
    }
    return total_read_data_size;
}