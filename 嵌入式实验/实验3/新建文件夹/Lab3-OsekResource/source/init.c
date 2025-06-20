/*************************************************
**************** InitTask ************************
**************************************************
*/
TASK(InitTask)
{
    // 初始化系统资源
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;

    // 显示系统LOGO
    showLogo();

    // 激活周期任务
    ActivateTask(PeriodicTask);

    // 终止初始化任务
    TerminateTask();
}