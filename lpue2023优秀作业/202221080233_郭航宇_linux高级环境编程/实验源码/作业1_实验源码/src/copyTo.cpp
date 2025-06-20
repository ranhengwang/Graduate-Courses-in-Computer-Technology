#include "filesystem.hpp"
#include <string>
int main(int argc, char const *argv[])
{
    FileSystem *fs = new FileSystem();
    fs->copyTo(argv[1], argv[2]);
    delete fs;
    return 0;
}