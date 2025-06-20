#include "filesystem.hpp"
#include <string>

int main(int argc, char const *argv[])
{
    FileSystem *fs = new FileSystem();
    // get file name from argv
    std::string fileName = argv[1];
    fs->create(fileName);
    delete fs;
    return 0;
}