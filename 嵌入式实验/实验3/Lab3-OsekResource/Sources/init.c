/****************************************************************************
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC All Rights Reserved.
 ***************************************************************************/

/**
 * @file 	init.c
 * @brief
 * @date
 */



#include "include.h"
#include <stdio.h>
#include <string.h>


void showLogo();

int main(int argc ,char *argv[])
{

  //volatile int x=sizeof(OSDWORD);


 	BSP_UartInit(); 	
	BSP_TimerInit();
	EnableInterrupts;
	//showLogo();
	StartOS(OSDEFAULTAPPMODE);
	
	
	return 0;

}

//show LOGO
void showLogo()
{
	printf("************************************************************\n");
	printf("*********************ESEC OSEK RESOURCE*********************\n");
	printf("************************************************************\n\n");
}

//output task ID and state
void showTaskInfo(TaskType taskId)
{
	TaskStateType taskState;
	GetTaskState(taskId, &taskState);
	printf("task id: %u, ", (int)taskId);
	switch(taskState)
	{
		case RUNNING:
			printf("taskState: running\n");
		break;
		case WAITING:
			printf("taskState: waiting\n");
		break;
		case READY:
			printf("taskState: ready\n");
		break;
		case SUSPENDED:
			printf("taskState: suspended\n");
	    break;
	    default:
	    break;
	}
}
/*************************************************
**************** Task1 ***************************
**************************************************
*/
TASK(Task1)
{
	TaskType taskId;
	StatusType status;
	
	GetTaskID(&taskId);

	printf("**********************task 1 running**********************\n");
	showTaskInfo(taskId); 
	
	printf("set TaskAlarm1...\n");	
	SetRelAlarm(TaskAlarm1, 3000, 10000);
	
	printf("task1 wait event: 0x01\n");
	WaitEvent(0x01);
	printf("task1 get event 0x01 from TaskAlarm8\n");
	printf("task1 cancel TaskAlarm8...\n");
	CancelAlarm(TaskAlarm8);   
    
 	printf("**********************task 1 end**************************\n"); 
	TerminateTask();
}



/*************************************************
**************** Task2 ***************************
**************************************************
*/
TASK(Task2)
{
	TaskType taskId;
	StatusType status;
	
	printf("task2 cancel TaskAlarm1...\n");
	CancelAlarm(TaskAlarm1);
	GetTaskID(&taskId);
  printf("**********************task 2 running**********************\n");
    
	//get Resource1
    status = GetResource(Resource1);
    printf("task2 get resource1...\n");
    if(status != E_OK)
    {
    	printf("task2 get resource1 error: %d\n", status);	
    }
    
  printf("set TaskAlarm2...\n");	
	SetRelAlarm(TaskAlarm2, 3000, 10000);
    
    //release Resource1
    printf("task2 release resource1...\n");
    status = ReleaseResource(Resource1);   
    if(status != E_OK)
    {
    	printf("task2 release resource1 error: %d\n", status);	
    }
	printf("**********************task 2 end**************************\n");
	TerminateTask();
}



/*************************************************
**************** Task3 ***************************
**************************************************
*/
TASK(Task3)
{ 
	TaskType taskId;
	StatusType status;
	
	printf("task3 cancel TaskAlarm2...\n");
	CancelAlarm(TaskAlarm2);
	
	GetTaskID(&taskId);
  printf("**********************task 3 running**********************\n");
  showTaskInfo(taskId); 
   
   //get Resource2
    status = GetResource(Resource2);
    printf("task3 get resource2...\n");
    if(status != E_OK)
    {
    	printf("task3 get resource2 error: %d\n", status);	
    }
    
  printf("set TaskAlarm3...\n");	
	SetRelAlarm(TaskAlarm3, 3000, 10000);
    
    //release Resource2
    printf("task3 release resource2...\n");
    status = ReleaseResource(Resource2);   
    if(status != E_OK)
    {
    	printf("task3 release resource2 error: %d\n", status);	
    }
    
    printf("**********************task 3 end**************************\n");
	TerminateTask();
}


/*************************************************
**************** Task4 ***************************
**************************************************
*/
TASK(Task4)
{ 
	TaskType taskId;
	StatusType status;
	
	GetTaskID(&taskId);
	
	printf("task4 cancel TaskAlarm3...\n");
	CancelAlarm(TaskAlarm3);
	
    printf("**********************task 4 running**********************\n");
    showTaskInfo(taskId); 
    
    //get Resource3
    status = GetResource(Resource3);
    printf("task4 get resource3...\n");
    if(status != E_OK)
    {
    	printf("task4 get resource3 error: %d\n", status);	
    }
    
  printf("set TaskAlarm4...\n");	
	SetRelAlarm(TaskAlarm4, 3000, 10000);
    
    //release Resource3
    printf("task4 release resource3...\n");
    status = ReleaseResource(Resource3);   
    if(status != E_OK)
    {
    	printf("task4 release resource3 error: %d\n", status);	
    }
    
    printf("**********************task 4 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task5 ***************************
**************************************************
*/
TASK(Task5)
{
	TaskType taskId;
	StatusType status;
	
	GetTaskID(&taskId);
  printf("task5 cancel TaskAlarm4...\n");
	CancelAlarm(TaskAlarm4);
    printf("**********************task 5 running**********************\n");
   	showTaskInfo(taskId); 
    
    //get Resource1
    status = GetResource(Resource1);
    printf("task5 get resource1...\n");
    if(status != E_OK)
    {
    	printf("task5 get resource1 error: %d\n", status);	
    }
    
    //activate task6
    printf("activate task6...\n");
    ActivateTask(Task6);
    
    //release Resource1
    printf("task5 release resource1...\n");
    status = ReleaseResource(Resource1);   
    if(status != E_OK)
    {
    	printf("task5 release resource1 error: %d\n", status);	
    }
    
    printf("**********************task 5 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task6 ***************************
**************************************************
*/
TASK(Task6)
{ 
	TaskType taskId;
	StatusType status;
	
	GetTaskID(&taskId);
    printf("**********************task 6 running**********************\n");
    showTaskInfo(taskId); 
    
    //get Resource2
    status = GetResource(Resource2);
    printf("task6 get resource2...\n");
    if(status != E_OK)
    {
    	printf("task6 get resource2 error: %d\n", status);	
    }
    
    //activate task7
    printf("activate task7...\n");
    ActivateTask(Task7);
    
    printf("task6 wait event: 0x06\n");
	  WaitEvent(0x06);
	  
	  printf("task6 get event 0x06 from TaskAlarm5\n");
	  printf("task6 cancel TaskAlarm5...\n");
	  CancelAlarm(TaskAlarm5);   
    
    //release Resource2
    printf("task6 release resource2...\n");
    status = ReleaseResource(Resource2);
    
    if(status != E_OK)
    {
    	printf("task6 release resource2 error: %d\n", status);	
    }
    
    printf("**********************task 6 end**************************\n");
    TerminateTask();
}

/*************************************************
**************** Task7 ***************************
**************************************************
*/
TASK(Task7)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    	
    showLogo();
    	
    printf("**********************task 7 running**********************\n");
    showTaskInfo(taskId); 
    
    //get Resource3
    status = GetResource(Resource3);
    printf("task7 get resource3...\n");
    if(status != E_OK)
    {
    	printf("task7 get resource3 error: %d\n", status);	
    }
    
    //activate task8
    printf("activate task8...\n");
    ActivateTask(Task8);
    
    printf("set TaskAlarm5...\n");	
  	SetRelAlarm(TaskAlarm5, 3000, 10000);
  	
  	printf("task7 wait event: 0x07\n");
	  WaitEvent(0x07);
	  
	  printf("task7 get event 0x07 from TaskAlarm6\n");
	  printf("task7 cancel TaskAlarm6...\n");
	  CancelAlarm(TaskAlarm6);   
    
    //release Resource3
    printf("task7 release resource3...\n");
    status = ReleaseResource(Resource3);
    if(status != E_OK)
    {
    	printf("task7 release resource3 error: %d\n", status);	
    }
    
    
    printf("**********************task 7 end**************************\n");
    TerminateTask();
}


/*************************************************
**************** Task8 ***************************
**************************************************
*/
TASK(Task8)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    	
    showLogo();
    	
    printf("**********************task 8 running**********************\n");
    showTaskInfo(taskId); 
    
    //get Resource1
    status = GetResource(Resource1);
    printf("task8 get resource1...\n");
    if(status != E_OK)
    {
    	printf("task8 get resource1 error: %d\n", status);	
    }
    
    //activate task9
    printf("activate task9...\n");
    ActivateTask(Task9);
    
    printf("set TaskAlarm6...\n");	
  	SetRelAlarm(TaskAlarm6, 3000, 10000);
  	
  	printf("task8 wait event: 0x08\n");
	  WaitEvent(0x08);
	  
	  printf("task8 get event 0x08 from TaskAlarm7\n");
	  printf("task8 cancel TaskAlarm7...\n");
	  CancelAlarm(TaskAlarm7);   
    
    //release Resource1
    printf("task8 release resource1...\n");
    status = ReleaseResource(Resource1);
    if(status != E_OK)
    {
    	printf("task8 release resource1 error: %d\n", status);	
    }
    
    
    printf("**********************task 8 end**************************\n");
    TerminateTask();
}
/*************************************************
**************** Task9 ***************************
**************************************************
*/
TASK(Task9)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    	
    showLogo();
    	
    printf("**********************task 9 running**********************\n");
    showTaskInfo(taskId); 
    
    //get Resource2
    status = GetResource(Resource2);
    printf("task9 get resource2...\n");
    if(status != E_OK)
    {
    	printf("task9 get resource2 error: %d\n", status);	
    }
    
    //activate task10
    printf("activate task10...\n");
    ActivateTask(Task10);
    
    printf("set TaskAlarm7...\n");	
  	SetRelAlarm(TaskAlarm7, 3000, 10000);
  	
  	printf("task9 wait event: 0x09\n");
	  WaitEvent(0x09);
	  
	  printf("task9 get event 0x09 from TaskAlarm8\n");
	  printf("task9 cancel TaskAlarm8...\n");
	  CancelAlarm(TaskAlarm8);   
    
    //release Resource2
    printf("task9 release resource2...\n");
    status = ReleaseResource(Resource2);
    if(status != E_OK)
    {
    	printf("task9 release resource2 error: %d\n", status);	
    }
    
    
    printf("**********************task 9 end**************************\n");
    TerminateTask();
}
/*************************************************
**************** Task10 ***************************
**************************************************
*/
TASK(Task10)
{ 
	TaskType taskId;
	StatusType status;
	GetTaskID(&taskId);
    	
    showLogo();
    	
    printf("**********************task 10 running**********************\n");
    showTaskInfo(taskId); 
    
    //get Resource3
    status = GetResource(Resource3);
    printf("task10 get resource3...\n");
    if(status != E_OK)
    {
    	printf("task10 get resource3 error: %d\n", status);	
    }
    
    printf("set TaskAlarm8...\n");	
  	SetRelAlarm(TaskAlarm8, 3000, 10000);   
    
    //release Resource3
    printf("task10 release resource3...\n");
    status = ReleaseResource(Resource3);
    if(status != E_OK)
    {
    	printf("task10 release resource3 error: %d\n", status);	
    }
    
    
    printf("**********************task 10 end**************************\n");
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

