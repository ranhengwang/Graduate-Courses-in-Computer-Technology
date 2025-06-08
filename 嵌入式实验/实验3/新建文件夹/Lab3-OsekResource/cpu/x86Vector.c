/*************************************************
**************** Initialization Task ************
**************************************************
*/
TASK(InitTask)
{
    // Perform system initialization
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;

    // Optionally show logo
    showLogo();

    // Activate the first task
    ActivateTask(Task1);
    
    TerminateTask();
}