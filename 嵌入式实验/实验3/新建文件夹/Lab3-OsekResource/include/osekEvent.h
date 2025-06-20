#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();
void SyncTask();

int main(int argc, char *argv[])
{
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;
    
    // 启动初始化任务
    StartOS(OSDEFAULTAPPMODE);
    
    return 0;
}

// 初始化任务
TASK(InitTask)
{
    printf("Initializing system...\n");
    // 进行系统初始化操作
    // 例如，初始化资源、设置定时器等
    // ...

    // 初始化完成后，激活周期任务
    ActivateTask(PeriodicTask);
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("Periodic task running...\n");
    // 执行周期性操作
    // 例如，读取传感器数据
    // ...

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
    if (status != E_OK)
    {
        printf("Sync task failed to get resource1: %d\n", status);
        TerminateTask();
    }

    // 执行同步操作
    // ...

    // 释放资源
    ReleaseResource(Resource1);
    TerminateTask();
}

// 其他任务和功能...

/*************************************************
**************** Task1 ***************************
**************************************************
*/
TASK(Task1)
{
    // 任务1的实现...
    TerminateTask();
}

/*************************************************
**************** Task2 ***************************
**************************************************
*/
TASK(Task2)
{
    // 任务2的实现...
    TerminateTask();
}

// 其他任务...

/*************************************************
**************** Idle Task ***********************
**************************************************
*/
TASK(Idle)
{
    ShutdownOS(0xff);
}