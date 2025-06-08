#pragma once

#include <cstring>
#include <iostream>


using namespace std;
#define MAX_TASKS 100 

//任务参数结构体
typedef struct {  
    int key;  
    string value;  
    string value1;  
    string value2;  
} TaskArgs;  

// 任务结构体  
typedef struct {  
    void (*function)(void *arg); // 任务函数  
    TaskArgs *arg;                   // 任务参数  
} Task;  

// 任务队列结构体  
typedef struct {  
    Task tasks[MAX_TASKS];         // 任务数组  
    int head;                      // 队列头  
    int tail;                      // 队列尾  
    int count;                     // 当前任务数量  
    pthread_mutex_t lock;          // 互斥锁  
    pthread_cond_t not_empty;      // 任务到达的条件变量  
    pthread_cond_t not_full;       // 任务队列不满的条件变量  
} TaskQueue;  

// 线程池结构体  
typedef struct {  
    pthread_t *threads;            // 工作线程数组  
    int thread_count;              // 线程数量  
    TaskQueue task_queue;          // 任务队列  
    int stop;                      // 停止标志  
} ThreadPool;  

// 任务队列初始化  
void init_task_queue(TaskQueue *queue);
// 添加任务到队列  
void add_task(TaskQueue *queue, void (*function)(void *), TaskArgs *arg);
// 获取任务  
Task get_task(TaskQueue *queue);
// 工作线程循环  
void *worker(void *pool);
// 创建线程池  
ThreadPool *create_thread_pool(int thread_count);
// 销毁线程池  
void destroy_thread_pool(ThreadPool *pool);