#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();
void SyncTask();

int main(int argc ,char *argv[])
{
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;
    
    // 启动初始化任务
    ActivateTask(InitTask);
    
    return 0;
}

// 初始化任务
TASK(InitTask)
{
    printf("********************** Initialization Task Running **********************\n");
    
    // 进行系统初始化
    // 例如：初始化资源、设置定时器等
    // ...

    // 启动周期任务
    SetRelAlarm(PeriodicAlarm, 1000, 1000); // 每1000ms触发一次周期任务
    // 启动同步任务
    ActivateTask(SyncTask);
    
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("********************** Periodic Task Running **********************\n");
    
    // 执行周期性操作
    // 例如：监测系统状态、更新显示等
    // ...
    
    TerminateTask();
}

// 同步任务
TASK(SyncTask)
{
    TaskType taskId;
    GetTaskID(&taskId);
    
    printf("********************** Sync Task Running **********************\n");
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync Task acquired Resource1\n");
        
        // 执行需要同步的操作
        // ...
        
        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync Task released Resource1\n");
    } else {
        printf("Sync Task failed to acquire Resource1: %d\n", status);
    }
    
    TerminateTask();
}

// 其他任务和功能保持不变...

// 任务和事件的定义
// 定义周期性任务的报警
ALARMS(PeriodicAlarm, 1000, 1000); // 每1000ms触发一次

// 其他任务的定义...