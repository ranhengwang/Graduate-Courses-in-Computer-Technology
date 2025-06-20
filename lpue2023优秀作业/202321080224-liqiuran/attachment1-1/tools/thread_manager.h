#ifndef _THREAD_MANAGRE_H_
#define _THREAD_MANAGRE_H_

#include <unistd.h>
#include <stdint.h>

struct ThreadTask
{
    void *(*exec_func)(void *);
    void *exec_args;
    void *(*callback)(void *);
    void *callback_args;
};


/// \brief ThreadPool 的私有成员变量列表
class ThreadPoolPrivMems;

/// \brief 线程池
class ThreadPool
{
public:
    ThreadPool();
    virtual ~ThreadPool();

public:
    /// \brief init 创建并初始化
    int init(ThreadTask *tasks, unsigned int task_count, uint32_t thread_count=10);
    /// \brief uninit 删除线程池
    int uninit();
    /// \brief set_tasks 设置任务列表
    int set_tasks(ThreadTask *tasks, unsigned int task_count);
    /// \brief unset_tasks 卸载任务列表
    int unset_tasks();
    /// \brief update_tasks 更新任务列表
    int update_tasks(ThreadTask *tasks, unsigned int pos);

private:
    ThreadPoolPrivMems *members_ptr;
};



#endif



