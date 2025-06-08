#pragma once

#include <fstream>

using namespace std;

#define TMP_FILE_DIR "/tmp/sort"
#define MAX_MEMORY 10000  // 内存中可以同时处理的最大数据量  
#define MAX_FILE_NAME 100  
#define MAX_TASKS 100  
#define LINE_BUF_SIZE 1024

static int count = 0;

// 合并临时文件  
void merge_files(int num_files);
// 比较函数，用于 qsort  
int compare(const void* a, const void* b);
// 自定义分块读取函数  
int fread_split(FILE* file, int* buffer, int max_size);

void merge_two_file(void *arg);
// 外部排序主函数  
void external_sort(void *arg);
void list_files(string path);
void delete_directory(const char *path);
int count_prefix(int prefix, const char *dir_path);
int merge_orderd_files(int prefix, const char *dir_path);
void copy_prefix_file(const char *path, int prefix, const char *dst_name);