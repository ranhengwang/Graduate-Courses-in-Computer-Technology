### 1. 添加初始化任务
初始化任务将在系统启动时执行，负责初始化系统资源和设置。

```c
/*************************************************
**************** Init Task *********************
**************************************************
*/
TASK(InitTask)
{
    // Initialize system resources
    BSP_UartInit(); 	
    BSP_TimerInit();
    
    // Start the main application mode
    StartOS(OSDEFAULTAPPMODE);
    
    TerminateTask(); // Terminate the initialization task
}
```

### 2. 添加周期任务
周期任务将在特定时间间隔内执行。我们可以创建一个简单的周期任务，例如每隔1秒打印一条消息。

```c
/*************************************************
**************** Periodic Task ******************
**************************************************
*/
TASK(PeriodicTask)
{
    printf("Periodic Task is running...\n");
    
    // Set the next alarm for this task
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000); // 每1000毫秒触发一次
    
    TerminateTask();
}
```

### 3. 添加同步相关任务
我们可以创建两个任务，它们通过事件进行同步。例如，任务A在完成某些操作后激活任务B。

```c
/*************************************************
**************** Task A *************************
**************************************************
*/
TASK(TaskA)
{
    printf("Task A is running...\n");
    
    // Perform some operations...
    
    // Activate Task B
    ActivateTask(TaskB);
    
    // Set an event to notify Task B
    SetEvent(TaskB, 0x01);
    
    TerminateTask();
}

/*************************************************
**************** Task B *************************
**************************************************
*/
TASK(TaskB)
{
    EventMaskType eventMask;
    
    // Wait for the event from Task A
    WaitEvent(0x01);
    
    // Clear the event
    ClearEvent(0x01);
    
    printf("Task B is running after being activated by Task A...\n");
    
    TerminateTask();
}
```

### 4. 修改主函数
在主函数中，我们需要启动初始化任务而不是直接启动OS默认应用模式。

```c
int main(int argc ,char *argv[])
{
    // Start the initialization task
    StartOS(OSDEFAULTAPPMODE);
    
    return 0;
}
```

### 5. 更新任务调度
确保在系统配置中定义了新的任务和相应的调度策略。您需要在 OSEK 配置文件中添加新任务和定时器。

### 6. 定义定时器和事件
在 OSEK 配置文件中，您需要定义 `TaskAlarmPeriodic` 和其他相关的事件和资源。

### 7. 完整代码示例
以下是修改后的代码片段示例：

```c
// 省略前面的代码...

/*************************************************
**************** Init Task *********************
**************************************************
*/
TASK(InitTask)
{
    BSP_UartInit(); 	
    BSP_TimerInit();
    StartOS(OSDEFAULTAPPMODE);
    TerminateTask();
}

/*************************************************
**************** Periodic Task ******************
**************************************************
*/
TASK(PeriodicTask)
{
    printf("Periodic Task is running...\n");
    SetRelAlarm(TaskAlarmPeriodic, 1000, 1000);
    TerminateTask();
}

/*************************************************
**************** Task A *************************
**************************************************
*/
TASK(TaskA)
{
    printf("Task A is running...\n");
    ActivateTask(TaskB);
    SetEvent(TaskB, 0x01);
    TerminateTask();
}

/*************************************************
**************** Task B *************************
**************************************************
*/
TASK(TaskB)
{
    WaitEvent(0x01);
    ClearEvent(0x01);
    printf("Task B is running after being activated by Task A...\n");
    TerminateTask();
}

// 主函数
int main(int argc ,char *argv[])
{
    StartOS(OSDEFAULTAPPMODE);
    return 0;
}

// 省略后面的代码...
```

### 8. 测试和验证
在完成代码修改后，您需要编译并测试系统，以确保所有任务按预期工作，并且没有资源冲突或死锁。

通过以上步骤，您可以将初始化任务、周期任务和同步相关任务集成到现有的嵌入式系统项目中。请根据您的具体需求和系统架构进行适当的调整。