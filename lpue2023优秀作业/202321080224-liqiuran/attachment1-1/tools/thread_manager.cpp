#include "thread_manager.h"
#include "../include/debug/macros.h"
#include <cstddef>
#include <stddef.h>
#include <iostream>
#include <pthread.h>
#include <string.h>
#include <typeinfo>

// 一组的线程数量
#define THREAD_GROUP_MAX_COUNT 10


/// \brief 进程执行函数的参数
class ThreadPoolExecArgs
{
public:
    uint32_t index;
    // 任务
    ThreadTask *task_ptr;
    // 任务锁
    pthread_mutex_t task_mutex;
    // 运行条件
    pthread_cond_t run_cond;
    // 终止信号
    bool is_closed;
    // 终止信号锁
    pthread_mutex_t closed_signal_mutex;
public:
    ThreadPoolExecArgs():
        index(0),
        is_closed(false)
    {
        if ((task_ptr = new ThreadTask) == NULL)
        {
            ERROR("pid %p: %s", pthread_self(), "failed to create task_ptr.");
            return ;
        }
        memset(task_ptr, 0, sizeof(*task_ptr));

        if (pthread_mutex_init(&task_mutex, NULL))
        {
            ERROR("pid %p: %s", pthread_self(), "failed to init task_mutex");
            return ;
        }
        
        if (pthread_cond_init(&run_cond, NULL))
        {
            ERROR("pid %p: %s", pthread_self(), "failed to init run_cond");
            return ;
        }

        if (pthread_mutex_init(&closed_signal_mutex, NULL))
        {
            ERROR("pid %p: %s", pthread_self(), "failed to init task_mutex");
            return ;
        }
    }

    virtual ~ThreadPoolExecArgs()
    {
        pthread_mutex_lock(&task_mutex);
        delete task_ptr;
        task_ptr = NULL;
        pthread_mutex_unlock(&task_mutex);

        if (pthread_mutex_destroy(&task_mutex))
        {
            ERROR("pid %p: %s", pthread_self(), "failed to destroy task_mutex");
        }

        if (pthread_cond_destroy(&run_cond))
        {
            ERROR("pid %p: %s", pthread_self(), "failed to destroy run_cond");
        }

        if (pthread_mutex_destroy(&closed_signal_mutex))
        {
            ERROR("pid %p: %s", pthread_self(), "failed to destroy task_mutex");
        }
    }

    int set_task(void *task_ptr)
    {
        INFO("pid %p: entering set_task", pthread_self());

        pthread_mutex_lock(&task_mutex);
        memcpy(this->task_ptr, task_ptr, sizeof(*this->task_ptr));
        pthread_mutex_unlock(&task_mutex);
        pthread_cond_signal(&run_cond);
        INFO("pid %p: quiting set_task", pthread_self());
        return 0;
    }

    int unset_task()
    {
        pthread_mutex_lock(&task_mutex);
        memset(task_ptr, 0, sizeof(*task_ptr));
        pthread_mutex_unlock(&task_mutex);
        return 0;
    }
};

class ThreadPoolPrivMems
{
public:
    // 线程句柄
    pthread_t thread_handles[THREAD_GROUP_MAX_COUNT];
    // 运行的函数组
    void *(*exec_funcs[THREAD_GROUP_MAX_COUNT])(void *);
    // 运行的函数组参数
    ThreadPoolExecArgs *exec_args[THREAD_GROUP_MAX_COUNT];
    // task 数量
    uint32_t task_count;
    // thread count
    uint32_t thread_count;
public:
    ThreadPoolPrivMems() :
        task_count(0)
    {
        // 初始化线程句柄
        memset(thread_handles, 0, sizeof(thread_handles));
        memset(exec_args, 0, sizeof(exec_args));
        // 初始化exec_funcs = NULL
        for (int i = 0; i < THREAD_GROUP_MAX_COUNT; ++i)
        {
            exec_funcs[i] = NULL;
            exec_args[i] = new ThreadPoolExecArgs;
            exec_args[i]->index = i;
        }
    }

    virtual ~ThreadPoolPrivMems()
    {
        for (int i = 0; i < THREAD_GROUP_MAX_COUNT; ++i)
        {
            exec_funcs[i] = NULL;
            delete exec_args[i];
            exec_args[i] = NULL;
        }
    }
};

void thread_clean_func(void *args)
{
    ThreadPoolExecArgs *exec_args = (ThreadPoolExecArgs *)args;
    pthread_mutex_unlock(&exec_args->task_mutex);
    return;
}


void *thread_func(void *args)
{
    INFO("pid %p: entering, args: %p", pthread_self(), args);
    if (args == NULL)
    {
        ERROR("args is NULL");
        return NULL;
    }

    ThreadPoolExecArgs *exec_args = (ThreadPoolExecArgs *)args;
    ThreadTask *task_ptr = exec_args->task_ptr;
    if (task_ptr == NULL)
    {
        ERROR("pid %p: task_ptr is NULL", pthread_self());
        return NULL;
    }
    pthread_mutex_t *task_mutex_ptr = &exec_args->task_mutex;
    pthread_cond_t *run_cond_ptr = &exec_args->run_cond;

    void *(*func_ptr)(void *args);
    func_ptr = NULL;
    while (1)
    {
        pthread_mutex_lock(&exec_args->closed_signal_mutex);
        if (exec_args->is_closed)
        {
            pthread_mutex_unlock(&exec_args->closed_signal_mutex);
            break;
        }
        pthread_mutex_unlock(&exec_args->closed_signal_mutex);

        pthread_mutex_lock(task_mutex_ptr);
        if (task_ptr->exec_func == NULL)
        {
            WARNING("pid %p: waiting", pthread_self());
            pthread_cond_wait(run_cond_ptr, task_mutex_ptr);
        }

        func_ptr = task_ptr->exec_func;
        pthread_mutex_unlock(task_mutex_ptr);

        if (func_ptr == NULL)
        {
            continue;
        }

        void *res = func_ptr(task_ptr->exec_args);
        if (res)
        {
            ;
        }
    }

    INFO("pid %p: quiting", pthread_self());
    return NULL;
}

ThreadPool::ThreadPool()
{
    INFO("pid %p: creating filesystem thread pool", pthread_self());
    if ((members_ptr = new ThreadPoolPrivMems) == NULL)
    {
        ERROR("pid %p:failed to new members", pthread_self());
    }
}

ThreadPool::~ThreadPool()
{
    INFO("pid %p: destroying filesystem thread poll", pthread_self());
    delete members_ptr;
}

int ThreadPool::init(ThreadTask *tasks, uint32_t task_count, uint32_t thread_count)
{
    if (tasks == NULL || task_count == 0) return 1;

    if (thread_count > THREAD_GROUP_MAX_COUNT)
    {
        this->members_ptr->thread_count = THREAD_GROUP_MAX_COUNT;
    }
    else
    {
        this->members_ptr->thread_count = thread_count;
    }
    ERROR("thread count: %u", this->members_ptr->thread_count);

    for (uint32_t i = 0; i < THREAD_GROUP_MAX_COUNT; i++)
    {
        // 创建线程
        while (pthread_create(members_ptr->thread_handles + i, NULL, thread_func, members_ptr->exec_args[i]))
        {
            WARNING("pid %p: recreating a pthread.", pthread_self());
        }
        INFO("pid %p: creatint a thread, thread id: %p", pthread_self(), members_ptr->thread_handles[i]);
    }

    set_tasks(tasks, task_count);
    return 0;
}

int ThreadPool::uninit()
{
    for (int i = 0; i < THREAD_GROUP_MAX_COUNT; ++i)
    {
        INFO("%s", "thread is shutdowning");
        pthread_mutex_lock(&members_ptr->exec_args[i]->closed_signal_mutex);
        members_ptr->exec_args[i]->is_closed = true;
        pthread_mutex_unlock(&members_ptr->exec_args[i]->closed_signal_mutex);
    }

    for (int i = 0; i < THREAD_GROUP_MAX_COUNT; ++i)
    {
        pthread_cond_broadcast(&members_ptr->exec_args[i]->run_cond);
        pthread_join(members_ptr->thread_handles[i], NULL);
    }

    return 0;
}

int ThreadPool::set_tasks(ThreadTask *tasks, uint32_t task_count)
{
    if (task_count > THREAD_GROUP_MAX_COUNT) return -1;
    members_ptr->task_count = task_count;
    for (uint32_t i = 0; i < task_count; ++i)
    {
        members_ptr->exec_args[i]->set_task(&(tasks[i]));
    }
    return 0;
}

int ThreadPool::unset_tasks()
{
    for (int i = 0; i < THREAD_GROUP_MAX_COUNT; ++i)
    {
        members_ptr->exec_args[i]->unset_task();
    }
    return 0;
}

int ThreadPool::update_tasks(ThreadTask *tasks, uint32_t pos)
{
    if (pos >= members_ptr->task_count)
    {
        ERROR("pid %p: pos error", pthread_self());
        return 1;
    }
    members_ptr->exec_args[pos]->set_task(tasks);
    return 0;
}






