#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();

TASK(InitTask) {
    // 初始化系统资源
    BSP_UartInit();
    BSP_TimerInit();
    EnableInterrupts;

    printf("System Initialized.\n");
    
    // 激活周期任务
    ActivateTask(PeriodicTask);
    
    TerminateTask();
}

TASK(PeriodicTask) {
    printf("Periodic Task Running...\n");
    
    // 设置下一个周期任务
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次
    
    TerminateTask();
}

// 其他任务...

int main(int argc, char *argv[]) {
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

// 其他任务的实现...