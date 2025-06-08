/*************************************************
**************** Init Task *********************
**************************************************
*/
TASK(InitTask)
{
    // 初始化系统资源
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;

    // 启动其他任务
    ActivateTask(Task1);
    TerminateTask();
}