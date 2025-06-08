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
    // 例如：初始化资源、设置初始状态等

    // 初始化完成后，激活周期任务和同步任务
    ActivateTask(PeriodicTask);
    ActivateTask(SyncTask);

    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    while (1) {
        printf("Periodic task running...\n");
        // 执行周期性操作
        // 例如：检查传感器状态、更新显示等

        // 设置下一个周期
        SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次
        WaitEvent(0x01); // 等待事件
    }
}

// 同步任务
TASK(SyncTask)
{
    while (1) {
        printf("Sync task waiting for event...\n");
        WaitEvent(0x02); // 等待事件

        // 执行同步操作
        printf("Sync task running...\n");
        // 例如：处理共享资源、更新状态等

        // 处理完成后，清除事件
        ClearEvent(0x02);
    }
}

// 其他任务和功能...

// 任务结束
TASK(Idle)
{
    ShutdownOS(0xff);
}