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

// 初始化任务
TASK(InitTask)
{
    printf("Initialization Task Running...\n");
    // 进行系统初始化
    // 例如：初始化硬件、设置初始状态等
    // ...

    // 激活周期任务
    ActivateTask(PeriodicTask);
    // 激活同步任务
    ActivateTask(SyncTask);

    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("Periodic Task Running...\n");
    // 执行周期性操作
    // 例如：读取传感器数据、更新状态等
    // ...

    // 设置下一个周期
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次

    TerminateTask();
}

// 同步任务
TASK(SyncTask)
{
    printf("Sync Task Running...\n");
    // 等待事件
    WaitEvent(0x01);
    // 处理事件
    printf("Event received in Sync Task.\n");

    // 释放资源
    ReleaseResource(Resource1);

    TerminateTask();
}

// 其他任务和功能...

// 任务1
TASK(Task1)
{
    // 任务1的实现
    // ...
}

// 任务2
TASK(Task2)
{
    // 任务2的实现
    // ...
}

// 其他任务...

// 任务空闲
TASK(Idle)
{
    ShutdownOS(0xff);
}