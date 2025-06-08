#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();
void SyncTask();

#define PERIODIC_TASK_PERIOD 1000 // 周期任务每1000毫秒执行一次

// 定义周期任务的ID
TASK(PeriodicTask)
{
    printf("Periodic Task Running...\n");
    // 在这里添加周期任务的具体操作
    TerminateTask();
}

// 定义同步任务的ID
TASK(SyncTask)
{
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Sync Task Running, Task ID: %u\n", (unsigned int)taskId);
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync Task acquired Resource1.\n");
        // 执行同步操作
        // ...
        
        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync Task released Resource1.\n");
    } else {
        printf("Sync Task failed to acquire Resource1: %d\n", status);
    }
    
    TerminateTask();
}

// 初始化任务
TASK(InitTask)
{
    printf("Initialization Task Running...\n");
    
    // 初始化系统资源
    BSP_UartInit(); 	
    BSP_TimerInit();
    
    // 启动周期任务
    SetRelAlarm(TaskAlarmPeriodic, PERIODIC_TASK_PERIOD, PERIODIC_TASK_PERIOD);
    
    // 启动同步任务
    ActivateTask(SyncTask);
    
    TerminateTask();
}

int main(int argc ,char *argv[])
{
    EnableInterrupts;
    StartOS(OSDEFAULTAPPMODE);
    
    return 0;
}

// 其他任务和函数保持不变...