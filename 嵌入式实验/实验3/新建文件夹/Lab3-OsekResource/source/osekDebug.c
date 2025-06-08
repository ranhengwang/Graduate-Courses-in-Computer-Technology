#include "include.h"
#include <stdio.h>
#include <string.h>

void showLogo();
void InitTask();
void PeriodicTask();
void SyncTask();

int main(int argc ,char *argv[])
{
    BSP_UartInit(); 	
    BSP_TimerInit();
    EnableInterrupts;
    StartOS(OSDEFAULTAPPMODE);
	
    return 0;
}

// Initialization Task
TASK(InitTask)
{
    printf("Initialization Task Running...\n");
    // Initialize resources, set up alarms, etc.
    // For example, set a periodic alarm for the PeriodicTask
    SetRelAlarm(PeriodicAlarm, 1000, 1000); // 1 second periodic task
    TerminateTask();
}

// Periodic Task
TASK(PeriodicTask)
{
    printf("Periodic Task Running...\n");
    // Perform periodic operations here
    TerminateTask();
}

// Synchronization Task
TASK(SyncTask)
{
    TaskType taskId;
    StatusType status;

    GetTaskID(&taskId);
    printf("Sync Task Running, Task ID: %u\n", (int)taskId);

    // Example of getting a resource
    status = GetResource(Resource1);
    if (status == E_OK) {
        printf("Sync Task acquired Resource1.\n");
        // Perform operations with the resource
        // Release the resource after use
        ReleaseResource(Resource1);
    } else {
        printf("Sync Task failed to acquire Resource1: %d\n", status);
    }

    TerminateTask();
}

// Show Logo
void showLogo()
{
    printf("************************************************************\n");
    printf("*********************ESEC OSEK RESOURCE*********************\n");
    printf("************************************************************\n\n");
}

// Task Definitions
// (Include your existing task definitions here, e.g., Task1, Task2, etc.)

/*************************************************
**************** Task Idle ***********************
**************************************************
*/
TASK(Idle)
{
    ShutdownOS(0xff);
}