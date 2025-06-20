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
    StartOS(OSDEFAULTAPPMODE);
	
    return 0;
}

// 显示LOGO
void showLogo()
{
    printf("************************************************************\n");
    printf("*********************ESEC OSEK RESOURCE*********************\n");
    printf("************************************************************\n\n");
}

// 初始化任务
TASK(InitTask)
{
    printf("Initializing system resources...\n");
    // 这里可以添加其他初始化代码
    showLogo();
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("Periodic task running...\n");
    // 执行周期性操作
    TerminateTask();
}

// 同步任务
TASK(SyncTask)
{
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Sync task running: Task ID %u\n", (int)taskId);
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync task acquired Resource1.\n");
        // 执行任务
        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync task released Resource1.\n");
    } else {
        printf("Sync task failed to acquire Resource1: %d\n", status);
    }
    
    TerminateTask();
}

// 周期性定时器回调
void PeriodicTimerCallback(void)
{
    ActivateTask(PeriodicTask);
}

// 定义周期性任务的定时器
void SetupPeriodicTimer(void)
{
    // 设置定时器以每隔一定时间调用 PeriodicTimerCallback
    SetRelAlarm(PeriodicAlarm, 1000, 1000); // 每1000毫秒触发一次
}

/*************************************************
**************** Task1 ***************************
**************************************************
*/
TASK(Task1)
{
    // 启动初始化任务
    ActivateTask(InitTask);
    
    // 设置周期性任务定时器
    SetupPeriodicTimer();
    
    // 启动同步任务
    ActivateTask(SyncTask);
    
    TerminateTask();
}

/*************************************************
**************** Idle Task ***********************
**************************************************
*/
TASK(Idle)
{
    ShutdownOS(0xff);
}