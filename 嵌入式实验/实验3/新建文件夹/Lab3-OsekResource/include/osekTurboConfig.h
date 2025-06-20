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
    printf("Initializing system...\n");
    // 进行系统初始化操作
    // 例如：初始化资源、设置定时器等

    // 初始化完成后，激活周期任务
    ActivateTask(PeriodicTask);
    // 激活同步任务
    ActivateTask(SyncTask);

    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("Periodic task running...\n");
    // 执行周期性操作
    // 例如：检查传感器状态、更新显示等

    // 设置下一个周期
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000毫秒执行一次

    TerminateTask();
}

// 同步任务
TASK(SyncTask)
{
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Sync task running, task ID: %u\n", (int)taskId);

    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync task acquired Resource1.\n");
        // 执行需要同步的操作

        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync task released Resource1.\n");
    } else {
        printf("Sync task failed to acquire Resource1: %d\n", status);
    }

    TerminateTask();
}

// 其他任务和功能保持不变...

// 任务定义
TASK(Task1) { /* ... */ }
TASK(Task2) { /* ... */ }
// 继续定义其他任务...

// Idle任务
TASK(Idle)
{
    ShutdownOS(0xff);
}