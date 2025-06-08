#include "filesystem.hpp"
#include <string>
int main(int argc, char const *argv[])
{
    FileSystem *fs = new FileSystem();
    fs->read(argv[1]);
    delete fs;
    return 0;
}