#include "pool.hpp"
#include <pthread.h>

// 任务队列初始化  
void init_task_queue(TaskQueue *queue) {  
    queue->head = queue->tail = queue->count = 0;  
    pthread_mutex_init(&queue->lock, NULL);  
    pthread_cond_init(&queue->not_empty, NULL);  
    pthread_cond_init(&queue->not_full, NULL);  
}  
// 添加任务到队列  
void add_task(TaskQueue *queue, void (*function)(void *), TaskArgs *arg) {  
    pthread_mutex_lock(&queue->lock);  
    while (queue->count == MAX_TASKS) {  
        pthread_cond_wait(&queue->not_full, &queue->lock);  
    }  
    
    queue->tasks[queue->tail].function = function;  
    queue->tasks[queue->tail].arg = arg;  
    queue->tail = (queue->tail + 1) % MAX_TASKS;  

    queue->count++;  
    pthread_cond_signal(&queue->not_empty);  
    pthread_mutex_unlock(&queue->lock);  
}  
// 获取任务  
Task get_task(TaskQueue *queue) {  
    pthread_mutex_lock(&queue->lock);  
    while (queue->count == 0) {  
        pthread_cond_wait(&queue->not_empty, &queue->lock);  
    }  
    Task task = queue->tasks[queue->head];  

    queue->head = (queue->head + 1) % MAX_TASKS;  
    queue->count--;  

    pthread_cond_signal(&queue->not_full);  
    pthread_mutex_unlock(&queue->lock);  
    return task;  
}  

// 工作线程循环  
void *worker(void *pool) {  
    ThreadPool *thread_pool = (ThreadPool *)pool;  
    while (1) {  
        Task task = get_task(&thread_pool->task_queue);  
        if (task.function == NULL && thread_pool->stop) {  
            break;  
        }  
        task.function(task.arg);  
        free(task.arg);
    }  
    return NULL;  
}  

// 创建线程池  
ThreadPool *create_thread_pool(int thread_count) {  
    ThreadPool *pool = (ThreadPool *) malloc(sizeof(ThreadPool));  
    pool->thread_count = thread_count;  
    pool->stop = 0;  
    pool->threads = (pthread_t *) malloc(thread_count * sizeof(pthread_t));  
    init_task_queue(&pool->task_queue);  

    for (int i = 0; i < thread_count; i++) {  
        pthread_create(&pool->threads[i], NULL, worker, pool);  
    }  

    return pool;  
}  

// 销毁线程池  
void destroy_thread_pool(ThreadPool *pool) {  
    pool->stop = 1;  
    for (int i = 0; i < pool->thread_count; i++) {  
        add_task(&pool->task_queue, NULL, NULL); // 添加结束任务  
    }  
    for (int i = 0; i < pool->thread_count; i++) {  
        pthread_join(pool->threads[i], NULL);  
    }  
    free(pool->threads);  
    pthread_mutex_destroy(&pool->task_queue.lock);  
    pthread_cond_destroy(&pool->task_queue.not_empty);  
    pthread_cond_destroy(&pool->task_queue.not_full);  
    free(pool);  
}  