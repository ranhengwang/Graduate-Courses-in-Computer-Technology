#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <string>
#include <iostream>
using namespace std;

void list_files(string path);
void print_info(string full_path);
void print_permissions(mode_t mode);

int main(void){
    list_files(".");
    return 0;
}

void list_files(string path){
    DIR *dir;
   
    if(!(dir=opendir(path.data()))){
        perror("Error opening file");
        return;
    }
    struct dirent *dirent;
    while((dirent = readdir(dir))!=NULL){
        if(strcmp(dirent->d_name,".")==0 || strcmp(dirent->d_name,"..")==0){
            continue;
        }
        
        string full_path;
        full_path = path + "/" + dirent->d_name;
        print_info(full_path);

        struct stat _stat;

        if(stat(full_path.data(),&_stat)==-1){
            perror("Error stat");
            continue;
        }
        if(S_ISDIR(_stat.st_mode)){
            list_files(full_path);
        }
    }
}
void print_info(string full_path){
    struct stat file_stat;
    if(stat(full_path.data(),&file_stat)==-1){
        perror("Error stat");
        return;
    }
    print_permissions(file_stat.st_mode);
    printf(" %ld",file_stat.st_size);
    // printf(" %s\n",full_path);
    cout << " " << full_path << endl;
    return;
}

void print_permissions(mode_t mode) {
    char perms[11] = "----------";
    if (S_ISDIR(mode)) perms[0] = 'd';  // 目录
    if (mode & S_IRUSR) perms[1] = 'r';  // 用户读权限
    if (mode & S_IWUSR) perms[2] = 'w';  // 用户写权限
    if (mode & S_IXUSR) perms[3] = 'x';  // 用户执行权限
    if (mode & S_IRGRP) perms[4] = 'r';  // 组读权限
    if (mode & S_IWGRP) perms[5] = 'w';  // 组写权限
    if (mode & S_IXGRP) perms[6] = 'x';  // 组执行权限
    if (mode & S_IROTH) perms[7] = 'r';  // 其他读权限
    if (mode & S_IWOTH) perms[8] = 'w';  // 其他写权限
    if (mode & S_IXOTH) perms[9] = 'x';  // 其他执行权限
    printf("%s ", perms);
}