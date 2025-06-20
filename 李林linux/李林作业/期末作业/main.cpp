#include "file.hpp"
#include <sys/stat.h>
#include <iostream>
int main(int argc, char *argv[]){

    if(argc != 3) {
    printf("usage: sort <folder path> <destination file>\n");
    return -1;
  }
    delete_directory(TMP_FILE_DIR);
    mkdir(TMP_FILE_DIR, 0777);
    list_files(argv[1]);
    int result = merge_orderd_files(0, TMP_FILE_DIR);
    copy_prefix_file(TMP_FILE_DIR, result, argv[2]);
    cout << "successful!" << endl;
    return 0;  
}