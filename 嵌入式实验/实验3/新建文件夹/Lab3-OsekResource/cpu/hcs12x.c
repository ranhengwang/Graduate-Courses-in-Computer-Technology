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

    // 显示系统标志
    showLogo();

    // 激活周期任务
    ActivateTask(PeriodicTask);
    
    TerminateTask();
}

TASK(PeriodicTask) {
    // 周期性执行的任务
    printf("Periodic Task is running...\n");
    
    // 设置下一个周期的定时器
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000ms执行一次

    TerminateTask();
}

// 其他任务...

int main(int argc, char *argv[]) {
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

// 显示LOGO
void showLogo() {
    printf("************************************************************\n");
    printf("*********************ESEC OSEK RESOURCE*********************\n");
    printf("************************************************************\n\n");
}

// 任务信息显示函数...
// 其他任务的实现...