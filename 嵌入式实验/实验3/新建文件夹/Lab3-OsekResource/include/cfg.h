/*************************************************
**************** Init Task *********************
**************************************************
*/
TASK(InitTask)
{
    // Initialize system resources
    BSP_UartInit(); 	
    BSP_TimerInit();
    
    // Start the main application
    StartOS(OSDEFAULTAPPMODE);
    
    TerminateTask();
}