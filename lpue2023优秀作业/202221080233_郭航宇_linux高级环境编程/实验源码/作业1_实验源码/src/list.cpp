#include "filesystem.hpp"
#include <string>
int main(int argc, char const *argv[])
{
    FileSystem *fs = new FileSystem();
    fs->list();
    delete fs;
    return 0;
}