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
    // 进行系统初始化
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
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次

    TerminateTask();
}

// 同步任务
TASK(SyncTask)
{
    printf("Sync task running...\n");
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status == E_OK) {
        // 执行需要同步的操作
        printf("Sync task acquired Resource1.\n");

        // 释放资源
        ReleaseResource(Resource1);
        printf("Sync task released Resource1.\n");
    } else {
        printf("Sync task failed to acquire Resource1: %d\n", status);
    }

    TerminateTask();
}

// 其他任务和功能...

// 显示LOGO
void showLogo()
{
    printf("************************************************************\n");
    printf("*********************ESEC OSEK RESOURCE*********************\n");
    printf("************************************************************\n\n");
}

// 任务信息显示函数
void showTaskInfo(TaskType taskId)
{
    TaskStateType taskState;
    GetTaskState(taskId, &taskState);
    printf("task id: %u, ", (int)taskId);
    switch(taskState)
    {
        case RUNNING:
            printf("taskState: running\n");
            break;
        case WAITING:
            printf("taskState: waiting\n");
            break;
        case READY:
            printf("taskState: ready\n");
            break;
        case SUSPENDED:
            printf("taskState: suspended\n");
            break;
        default:
            break;
    }
}

// Idle任务
TASK(Idle)
{
    ShutdownOS(0xff);
}