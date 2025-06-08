/*************************************************
**************** Initialization Task ************
**************************************************
*/
TASK(InitTask)
{
    // Initialize system resources
    BSP_UartInit(); 	
    BSP_TimerInit();
    
    // Enable interrupts
    EnableInterrupts;

    // Start the main application mode
    StartOS(OSDEFAULTAPPMODE);
    
    TerminateTask();
}