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

#define TASK_NUM 14

// 假设有14个任务和7个消息/事件
#define MSG_NUM 7
#define EVT_NUM 7

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



// 任务优先级（示例，实际优先级请结合OSEK配置）
const int task_priority[TASK_NUM] = {14,13,12,11,10,9,8,7,6,5,4,3,2,1};



// 消息和事件定义（示例）
//MessageType Msg[MSG_NUM];
EventMaskType Evt[EVT_NUM] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40};

void showLogo() {
    printf("************************************************************\n");
    printf("****************ESEC OSEK EVENT AND MESSAGE*****************\n");
    printf("************************************************************\n\n");
}

void showTaskInfo(TaskType taskId) {
    TaskStateType taskState;
    GetTaskState(taskId, &taskState);
    printf("task id: %u, ", (int)taskId);
    switch(taskState) {
        case RUNNING: printf("taskState: running\n"); break;
        case WAITING: printf("taskState: waiting\n"); break;
        case READY: printf("taskState: ready\n"); break;
        case SUSPENDED: printf("taskState: suspended\n"); break;
        default: break;
    }
}

// 前7个任务：消息通信
TASK(Task1) {
    TaskType taskId; GetTaskID(&taskId);
    showLogo();
    printf("Task1 running, priority=%d\n", task_priority[0]);
    showTaskInfo(taskId);
    // 发送消息给Task}char msg[] = "Msg from Task1";      SendMessage(Msg1, msg);
    printf("Task1 sent message to Task2\n");
    TerminateTask();
}

TASK(Task2) {
    TaskType taskId;
    GetTaskID(&taskId);
    printf("Task2 running, priority=%d\n", task_priority[1]);
    showTaskInfo(taskId);
    char buf[32];
    if (ReceiveMessage(Msg1, buf) == E_OK)
        printf("Task2 received: %s\n", buf);
    // 发送消息给Task3
    SendMessage(Msg2, "Msg from Task2");
    TerminateTask();
}

TASK(Task3) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task3 running, priority=%d\n", task_priority[2]);
    showTaskInfo(taskId);
    char buf[32];
    if (ReceiveMessage(Msg2, buf) == E_OK)
        printf("Task3 received: %s\n", buf);
    SendMessage(Msg3, "Msg from Task3");
    TerminateTask();
}

TASK(Task4) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task4 running, priority=%d\n", task_priority[3]);
    showTaskInfo(taskId);
    char buf[32];
    if (ReceiveMessage(Msg3, buf) == E_OK)
        printf("Task4 received: %s\n", buf);
    SendMessage(Msg4, "Msg from Task4");
    TerminateTask();
}

TASK(Task5) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task5 running, priority=%d\n", task_priority[4]);
    showTaskInfo(taskId);
    char buf[32];
    if (ReceiveMessage(Msg4, buf) == E_OK)
        printf("Task5 received: %s\n", buf);
    SendMessage(Msg5, "Msg from Task5");
    TerminateTask();
}

TASK(Task6) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task6 running, priority=%d\n", task_priority[5]);
    showTaskInfo(taskId);
    char buf[32];
    if (ReceiveMessage(Msg5, buf) == E_OK)
        printf("Task6 received: %s\n", buf);
    SendMessage(Msg6, "Msg from Task6");
    TerminateTask();
}

TASK(Task7) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task7 running, priority=%d\n", task_priority[6]);
    showTaskInfo(taskId);
    char buf[32];
    if (ReceiveMessage(Msg7, buf) == E_OK)
        printf("Task7 received: %s\n", buf);
    // 最后一个消息
    TerminateTask();
}

// 后7个任务：事件同步
TASK(Task8) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task8 running, priority=%d\n", task_priority[7]);
    showTaskInfo(taskId);
    printf("Task8 wait event 0x01\n");
    WaitEvent(Evt[0]);
    printf("Task8 got event 0x01\n");
    SetEvent(Task9, Evt[1]);
    TerminateTask();
}

TASK(Task9) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task9 running, priority=%d\n", task_priority[8]);
    showTaskInfo(taskId);
    printf("Task9 wait event 0x02\n");
    WaitEvent(Evt[1]);
    printf("Task9 got event 0x02\n");
    SetEvent(Task10, Evt[2]);
    TerminateTask();
}

TASK(Task10) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task10 running, priority=%d\n", task_priority[9]);
    showTaskInfo(taskId);
    printf("Task10 wait event 0x04\n");
    WaitEvent(Evt[2]);
    printf("Task10 got event 0x04\n");
    SetEvent(Task11, Evt[3]);
    TerminateTask();
}

TASK(Task11) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task11 running, priority=%d\n", task_priority[10]);
    showTaskInfo(taskId);
    printf("Task11 wait event 0x08\n");
    WaitEvent(Evt[3]);
    printf("Task11 got event 0x08\n");
    SetEvent(Task12, Evt[4]);
    TerminateTask();
}

TASK(Task12) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task12 running, priority=%d\n", task_priority[11]);
    showTaskInfo(taskId);
    printf("Task12 wait event 0x10\n");
    WaitEvent(Evt[4]);
    printf("Task12 got event 0x10\n");
    SetEvent(Task13, Evt[5]);
    TerminateTask();
}

TASK(Task13) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task13 running, priority=%d\n", task_priority[12]);
    showTaskInfo(taskId);
    printf("Task13 wait event 0x20\n");
    WaitEvent(Evt[5]);
    printf("Task13 got event 0x20\n");
    SetEvent(Task14, Evt[6]);
    TerminateTask();
}

TASK(Task14) {
    TaskType taskId; GetTaskID(&taskId);
    printf("Task14 running, priority=%d\n", task_priority[13]);
    showTaskInfo(taskId);
    printf("Task14 wait event 0x40\n");
    WaitEvent(Evt[6]);
    printf("Task14 got event 0x40\n");
    TerminateTask();
}