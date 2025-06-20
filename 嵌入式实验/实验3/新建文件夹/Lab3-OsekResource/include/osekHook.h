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

    // 显示系统标志
    showLogo();

    // 激活周期任务
    ActivateTask(PeriodicTask);
    
    TerminateTask();
}

TASK(PeriodicTask)
{
    // 周期性执行的任务
    printf("Periodic Task is running...\n");
    
    // 设置下一个周期
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次

    TerminateTask();
}

TASK(SyncTask1)
{
    TaskType taskId;
    GetTaskID(&taskId);
    
    printf("Sync Task 1 is running...\n");
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync Task 1 acquired Resource1.\n");
        // 执行任务逻辑
        // ...
        
        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync Task 1 released Resource1.\n");
    } else {
        printf("Sync Task 1 failed to acquire Resource1.\n");
    }
    
    TerminateTask();
}

TASK(SyncTask2)
{
    TaskType taskId;
    GetTaskID(&taskId);
    
    printf("Sync Task 2 is running...\n");
    
    // 获取资源
    StatusType status = GetResource(Resource2);
    if (status == E_OK) {
        printf("Sync Task 2 acquired Resource2.\n");
        // 执行任务逻辑
        // ...
        
        // 释放资源
        ReleaseResource(Resource2);
        printf("Sync Task 2 released Resource2.\n");
    } else {
        printf("Sync Task 2 failed to acquire Resource2.\n");
    }
    
    TerminateTask();
}

int main(int argc ,char *argv[])
{
    // 启动初始化任务
    StartOS(OSDEFAULTAPPMODE);
    
    return 0;
}

// 显示系统标志
void showLogo()
{
    printf("************************************************************\n");
    printf("*********************ESEC OSEK RESOURCE*********************\n");
    printf("************************************************************\n\n");
}