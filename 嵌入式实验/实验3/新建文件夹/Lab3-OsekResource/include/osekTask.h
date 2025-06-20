#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();

int main(int argc ,char *argv[])
{
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;
    
    // 启动初始化任务
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
    
    // 激活周期任务
    ActivateTask(PeriodicTask);
    
    TerminateTask();
}

// 周期任务
TASK(PeriodicTask)
{
    printf("Periodic task running...\n");
    // 这里可以添加周期性操作代码
    
    // 设置下一个周期
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次
    
    TerminateTask();
}

// 同步相关任务示例
TASK(SyncTask)
{
    TaskType taskId;
    StatusType status;
    
    GetTaskID(&taskId);
    printf("Sync task running: %u\n", (int)taskId);
    
    // 获取资源
    status = GetResource(Resource1);
    if(status != E_OK)
    {
        printf("Sync task get resource1 error: %d\n", status);	
    }
    
    // 执行任务
    printf("Sync task is executing...\n");
    
    // 释放资源
    status = ReleaseResource(Resource1);
    if(status != E_OK)
    {
        printf("Sync task release resource1 error: %d\n", status);	
    }
    
    TerminateTask();
}

/*************************************************
**************** Task Idle ***********************
**************************************************
*/
TASK(Idle)
{
    ShutdownOS(0xff);
}