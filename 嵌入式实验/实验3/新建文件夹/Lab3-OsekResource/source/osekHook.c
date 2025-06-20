#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();

TASK(InitTask)
{
    // 初始化系统资源
    BSP_UartInit();
    BSP_TimerInit();
    EnableInterrupts;

    // 启动周期任务
    SetRelAlarm(PeriodicAlarm, 1000, 1000); // 每1000ms触发一次
    TerminateTask();
}

TASK(PeriodicTask)
{
    printf("Periodic Task is running...\n");
    // 在这里执行周期性操作
    TerminateTask();
}

TASK(SyncTask1)
{
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Sync Task 1 running, Task ID: %u\n", (int)taskId);
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status != E_OK) {
        printf("Sync Task 1 failed to get Resource1: %d\n", status);
    } else {
        // 执行任务
        printf("Sync Task 1 is using Resource1...\n");
        // 释放资源
        ReleaseResource(Resource1);
    }
    
    TerminateTask();
}

TASK(SyncTask2)
{
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Sync Task 2 running, Task ID: %u\n", (int)taskId);
    
    // 获取资源
    StatusType status = GetResource(Resource2);
    if (status != E_OK) {
        printf("Sync Task 2 failed to get Resource2: %d\n", status);
    } else {
        // 执行任务
        printf("Sync Task 2 is using Resource2...\n");
        // 释放资源
        ReleaseResource(Resource2);
    }
    
    TerminateTask();
}

int main(int argc, char *argv[])
{
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

// 其他任务和函数保持不变