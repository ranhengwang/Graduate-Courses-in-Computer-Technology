#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();
void SyncTask();

int main(int argc, char *argv[]) {
    BSP_UartInit();
    BSP_TimerInit();
    EnableInterrupts;

    // 启动初始化任务
    ActivateTask(InitTask);
    
    return 0;
}

// 初始化任务
TASK(InitTask) {
    printf("********************** Initialization Task Running **********************\n");
    
    // 进行系统初始化
    // 例如：初始化资源、设置定时器等
    // ...

    // 启动周期任务
    SetRelAlarm(PeriodicAlarm, 1000, 1000); // 每1000ms触发一次周期任务

    // 启动同步任务
    ActivateTask(SyncTask);

    printf("********************** Initialization Task End **************************\n");
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask) {
    printf("********************** Periodic Task Running **********************\n");
    
    // 执行周期性操作
    // 例如：监控传感器数据、更新状态等
    // ...

    printf("********************** Periodic Task End **************************\n");
    TerminateTask();
}

// 同步任务
TASK(SyncTask) {
    TaskType taskId;
    GetTaskID(&taskId);
    
    printf("********************** Sync Task Running **********************\n");
    
    // 获取资源
    StatusType status = GetResource(Resource1);
    if (status != E_OK) {
        printf("Sync Task: Get Resource1 Error: %d\n", status);
    } else {
        // 执行需要同步的操作
        // ...

        // 释放资源
        ReleaseResource(Resource1);
    }

    printf("********************** Sync Task End **************************\n");
    TerminateTask();
}

// 其他任务和功能保持不变...

// 周期性任务的定时器
ALARMCALLBACK(PeriodicAlarm) {
    ActivateTask(PeriodicTask);
}

// 其他任务的定义...