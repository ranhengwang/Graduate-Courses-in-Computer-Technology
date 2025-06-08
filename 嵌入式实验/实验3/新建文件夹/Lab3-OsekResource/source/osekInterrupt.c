/*************************************************
**************** Initialization Task ************
**************************************************
*/
TASK(InitTask)
{
    // Initialize hardware components
    BSP_UartInit();
    BSP_TimerInit();
    
    // Enable interrupts
    EnableInterrupts;

    // Start the main application
    StartOS(OSDEFAULTAPPMODE);
    
    TerminateTask();
}