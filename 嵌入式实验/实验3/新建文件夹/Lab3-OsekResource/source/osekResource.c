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
    printf("Initialization Task Running...\n");
    // 初始化资源、设置初始状态等
    // 例如：初始化UART、定时器等
    // 这里可以添加更多的初始化代码

    printf("Initialization Complete.\n");
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("Periodic Task Running...\n");
    // 执行周期性操作
    // 例如：检查传感器状态、更新显示等

    TerminateTask();
}

// 同步任务
TASK(SyncTask)
{
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Sync Task Running, Task ID: %u\n", (int)taskId);
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync Task acquired Resource1.\n");
        // 执行需要同步的操作

        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync Task released Resource1.\n");
    } else {
        printf("Sync Task failed to acquire Resource1.\n");
    }

    TerminateTask();
}

// 周期性定时器回调
void PeriodicTimerCallback(void)
{
    ActivateTask(PeriodicTask);
}

// 定时器初始化
void InitPeriodicTimer()
{
    // 设置定时器以调用 PeriodicTimerCallback
    SetRelAlarm(TaskAlarm1, 1000, 1000); // 每1000毫秒触发一次
}

// 在初始化任务中调用定时器初始化
TASK(InitTask)
{
    printf("Initialization Task Running...\n");
    InitPeriodicTimer(); // 初始化周期性定时器
    TerminateTask();
}

// 其他任务和功能保持不变