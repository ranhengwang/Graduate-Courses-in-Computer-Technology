#include "file.hpp"
#include "pool.hpp"
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <future>
#include <vector>
#include <stdlib.h>  
#include <unistd.h>
using namespace std;

// 合并临时文件  
void merge_files(int num_files) {  
    FILE** files = (FILE**)malloc(num_files * sizeof(FILE*));  
    int* buffers = (int*)malloc(num_files * sizeof(int));  
    int* active = (int*)malloc(num_files * sizeof(int));  

    char tmp_file_name[128];
    // int count = 0;
    sprintf(tmp_file_name, "%s/0%s%d%s", TMP_FILE_DIR, "file", count++,".txt");
    FILE* output = fopen(tmp_file_name, "w");  
    // printf("%s\n",tmp_file_name);
    // 打开所有临时文件  
    char filename[MAX_FILE_NAME];  
    for (int i = 0; i < num_files; i++) {  
        
        sprintf(filename, "temp_%d.txt", i);  
        files[i] = fopen(filename, "r");  
        
        // 读取每个文件的第一个数据  
        if (fscanf(files[i], "%d", &buffers[i]) == 1) {  
            active[i] = 1;  
        } else {  
            active[i] = 0;  
        }  
    }  

    // 多路归并  
    while (1) {  
        int min_index = -1;  
        int min_value = 99999;  

        // 找出当前最小的数据  
        for (int i = 0; i < num_files; i++) {  
            if (active[i] && buffers[i] < min_value) {  
                min_index = i;  
                min_value = buffers[i];  
            }  
        }  

        // 如果没有找到有效数据，说明排序结束  
        if (min_index == -1) break;  

        // 写入最小值  
        fprintf(output, "%d ", min_value);  

        // 读取下一个数据  
        if (fscanf(files[min_index], "%d", &buffers[min_index]) != 1) {  
            active[min_index] = 0;  
        }  
    }  

    // 清理  
    fclose(output);  
    for (int i = 0; i < num_files; i++) {  
        fclose(files[i]);  
        // free(&filename[i]);
    }  
    free(files);  
    free(buffers);  
    free(active);  
}  

// 比较函数，用于 qsort  
int compare(const void* a, const void* b) {  
    return (*(int*)a - *(int*)b);  
}  
// 自定义分块读取函数  
int fread_split(FILE* file, int* buffer, int max_size) {  
    int count = 0;  
    int num;  

    while (count < max_size && fscanf(file, "%d", &num) == 1) {  
        buffer[count++] = num;  
    }  

    return count;  
}  

void merge_two_file(void *arg) 
{
      TaskArgs *task = (TaskArgs *)arg;
     char* file1 = (char*)task->value1.data();
     char* file2 = (char*)task->value2.data();
     char* output = (char*)task->value.data();
  char line_buffer1[LINE_BUF_SIZE];
  char line_buffer2[LINE_BUF_SIZE];
  FILE *f1 = fopen(file1, "r");
  FILE *f2 = fopen(file2, "r");
  FILE *fo = fopen(output, "w");

  if(!fo || (!f1 && !f2)) {
    perror("couldn't open file");
    if(fo) fclose(fo);
    if(f1) fclose(f1);
    if(f2) fclose(f2);
    return ;
  }

  if(!f2) {
    while(fgets(line_buffer1, LINE_BUF_SIZE, f1))
      fprintf(fo, "%s", line_buffer1) ;
    fclose(f1); fclose(fo);
    return ;
  } 

  if(!f1) {
    while(fgets(line_buffer1, LINE_BUF_SIZE, f2))
      fprintf(fo, "%s", line_buffer1);
    fclose(f2); fclose(fo);
    return ;
  }

  char *s1 = fgets(line_buffer1, LINE_BUF_SIZE, f1);
  char *s2 = fgets(line_buffer2, LINE_BUF_SIZE, f2);

  while(s1 || s2) { // Ordinary merge operation
    if(!s2 || s1 && atoi(line_buffer1) <= atoi(line_buffer2)) {
      fprintf(fo, "%s", line_buffer1);
      s1 = fgets(line_buffer1, LINE_BUF_SIZE, f1);
    } else {
      fprintf(fo, "%s", line_buffer2);
      s2 = fgets(line_buffer2, LINE_BUF_SIZE, f2);
    }
  }

  fclose(f1); fclose(f2); fclose(fo);
  free(file1); free(file2); free(output);

  return ;
}

// 外部排序主函数  
void external_sort(void *arg) {  
    TaskArgs *task = (TaskArgs *)arg;
    const char* input_file = task->value.data();
    FILE* input = fopen(input_file, "r");  
    if (!input) {  
        printf("无法打开输入文件\n");  
        return;  
    }  

    int* buffer = (int*)malloc(MAX_MEMORY * sizeof(int));  
    int num_files = 0;  
    int count;  

    // 分块读取并排序  
    while ((count = fread_split(input, buffer, MAX_MEMORY)) > 0) {  
        // 对当前块排序  
        qsort(buffer, count, sizeof(int), compare);  

        // 写入临时文件  
        char filename[MAX_FILE_NAME];  
        sprintf(filename, "temp_%d.txt", num_files);  
        FILE* temp_file = fopen(filename, "w");  
        
        for (int i = 0; i < count; i++) {  
            fprintf(temp_file, "%d ", buffer[i]);  
        }  
        
        fclose(temp_file);  
        num_files++;  
    }  

    // 归并排序  
    merge_files(num_files);  

    // 清理  
    free(buffer);  
    fclose(input);  
}  


void list_files(string path){
     ThreadPool *pool = create_thread_pool(4); // 创建一个有4个线程的线程池  
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

        TaskArgs* args = new TaskArgs();
        args->value = full_path;
        add_task(&pool->task_queue, external_sort, args); 
    }
    sleep(3);

 
    destroy_thread_pool(pool);
}

void delete_directory(const char *path) 
{
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int result = 0;

  if (d) {
    struct dirent *dir;
    while ((dir = readdir(d)) != NULL) {
      // Except . and ..
      if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
        char *file_path = (char*)malloc(path_len + strlen(dir->d_name) + 2);
        if (file_path) {
          snprintf(file_path, path_len + strlen(dir->d_name) + 2, "%s/%s", path, dir->d_name);
          if (dir->d_type == DT_DIR) {
            // Iteratively delete sub directory
            delete_directory(file_path);
          } else {
            // Delete file
            result = remove(file_path);
            if (result != 0) {
              printf("Error deleting file: %s\n", file_path);
            }
          }
          free(file_path);
        }
      }
    }
    closedir(d);
  }
  // Delete directory
  rmdir(path); 
}

int count_prefix(int prefix, const char *dir_path) 
{
  DIR *dp;
  struct dirent *ep;
  int count = 0;
  char buffer[LINE_BUF_SIZE];
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);

  dp = opendir(dir_path);
  if(dp != NULL) {
    while((ep = readdir(dp)))
      if(!strncmp(buffer, ep->d_name, sz))  // Find the file prefixed with prefix
        ++count;
    closedir(dp);
  } else {
    perror("couldn't open the directory");
  }
  return count;
}

int merge_orderd_files(int prefix, const char *dir_path)
{

  if(count_prefix(prefix, dir_path) <= 1) 
    return prefix;

  DIR *dp;
  struct dirent *ep;
  vector<future<int>> results;
  char file1[LINE_BUF_SIZE], file2[LINE_BUF_SIZE], output_file[LINE_BUF_SIZE];
  char buffer[LINE_BUF_SIZE];
  int i = 1;
    ThreadPool *pool = create_thread_pool(4);
  dp = opendir(dir_path);
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);

  if(dp != NULL) {
    while((ep = readdir(dp))) {
      if(!strncmp(buffer, ep->d_name, sz)) {
        if(i % 2) { // Mark the first file
          sprintf(file1, "%s/%s", dir_path, ep->d_name);
        } else { // Mark the second file
          sprintf(file2, "%s/%s", dir_path, ep->d_name);
          sprintf(output_file, "%s/%d%s", dir_path, prefix + 1, strrchr(file1, '/') + sz + 1);

        TaskArgs* arg = new TaskArgs();
        arg->value = output_file;
        arg->value1 = file1;
        arg->value2 = file2;
        add_task(&pool->task_queue,merge_two_file,arg);
        }
        ++ i;
      } 
    }
    if(i % 2 == 0) {
      sprintf(output_file, "%s/%d%s", dir_path, prefix + 1, strrchr(file1, '/') + sz + 1);
      //cout << file1 << "::" << output_file << endl;
      // If this is a single file, fill another parameter of merge_two_file with NULL
            TaskArgs* arg = new TaskArgs();
        arg->value = output_file;
        arg->value1 = file1;
        arg->value2 = file2;
        add_task(&pool->task_queue,merge_two_file,arg);
    }
    for(auto && result : results)
      result.get(); // Wait for all sort threads to complete
    closedir(dp);
  } else {
    perror("couldn't open the directory");
    return -1;
  }

     destroy_thread_pool(pool);
  // Sort operation of this prefix complete, start the sort for next prefix
  return merge_orderd_files(prefix + 1, dir_path);
}

// Copy the file prefixed with prefix in direcoty path to file dst_name
void copy_prefix_file(const char *path, int prefix, const char *dst_name) 
{
  DIR *dp;
  struct dirent *ep;
  char buffer[LINE_BUF_SIZE];
  char path_buffer[128];
  sprintf(buffer, "%d", prefix);
  size_t sz = strlen(buffer);
  FILE *dst_file, *src_file;

  if(!path || prefix < 0 || !dst_name)
    return;

  dp = opendir(path);
  if(!dp) {
    perror("couldn't open the directory");
    return;
  }

  dst_file = fopen(dst_name, "w");
  if(!dst_file) {
    closedir(dp);
    perror("couldn't open the file");
    return;
  }

  while((ep = readdir(dp)))
    if(!strncmp(buffer, ep->d_name, sz)) {  // Find file prefixed with prefix
      sprintf(path_buffer, "%s/", path);
      strcat(path_buffer, ep->d_name);
      src_file = fopen(path_buffer, "r");
      if(src_file) {
        while (fgets(buffer, LINE_BUF_SIZE, src_file) != NULL)  // Copy to file dst_name
          fprintf(dst_file, "%s", buffer);
        fclose(src_file);
      }
      break;
    }

  closedir(dp);
  fclose(dst_file);
}