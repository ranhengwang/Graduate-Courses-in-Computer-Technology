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
    StartOS(OSDEFAULTAPPMODE);
    
    return 0;
}

// 显示LOGO
void showLogo() {
    printf("************************************************************\n");
    printf("*********************ESEC OSEK RESOURCE*********************\n");
    printf("************************************************************\n\n");
}

// 初始化任务
TASK(InitTask) {
    showLogo();
    printf("Initialization Task Running...\n");
    
    // 初始化资源和定时器
    // 例如：BSP_InitResources();
    
    // 设置周期任务
    SetRelAlarm(PeriodicAlarm, 1000, 1000); // 每1000ms触发一次周期任务
    
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask) {
    printf("Periodic Task Running...\n");
    
    // 执行周期性操作
    // 例如：读取传感器数据，更新状态等
    
    TerminateTask();
}

// 同步任务
TASK(SyncTask) {
    TaskType taskId;
    StatusType status;
    
    GetTaskID(&taskId);
    printf("Sync Task Running...\n");
    
    // 获取资源
    status = GetResource(Resource1);
    if (status != E_OK) {
        printf("Sync Task: Get Resource1 Error: %d\n", status);
    } else {
        // 执行需要同步的操作
        printf("Sync Task: Got Resource1\n");
        
        // 释放资源
        ReleaseResource(Resource1);
    }
    
    TerminateTask();
}

/*************************************************
**************** Task1 ***************************
**************************************************
*/
TASK(Task1) {
    // 任务1的实现
    TerminateTask();
}

/*************************************************
**************** Task2 ***************************
**************************************************
*/
TASK(Task2) {
    // 任务2的实现
    TerminateTask();
}

/*************************************************
**************** Task Idle ***********************
**************************************************
*/
TASK(Idle) {
    ShutdownOS(0xff);
}