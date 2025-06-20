/****************************************************************************
 *				���ӿƼ���ѧǶ��ʽ����������� ��Ȩ����
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * $Log: osekEvent.c,v $
 * Revision 1.0  2008/04/04 16:31:00
 * �����ļ���
 */

/*
 * @file 	osekEvent.c
 * @brief
 *	<li>���ܣ�ʵ���¼�����</li>
 * @author
 * @date 	2008-04-04
 * <p>���ţ�
 */


/**************************** ���ò��� *********************************/

#include "osprop.h"
#include "osapi.h"


/*************************** ǰ���������� ******************************/

/**************************** ���岿�� *********************************/

/**************************** ʵ�ֲ��� *********************************/

#if defined(CONFIG_OSEK_SYSTEM_ET)

/*
 * @brief
 *    osekEvent_SetEvent�������¼����ڲ���������
 * @param
 * @param[in]	tcbPtr��ָ��������ƿ��ָ�롣
 *          	mask���¼����롣
 * @param[out]  �ޡ�
 * @returns:    E_OK�� �ɹ���
 *              [E_OS_ACCESS:ָ����������չ����]
 *              [E_OS_STATE:ָ��������SUSPEND״̬��]
 *              osekTask_WakeTask���õķ���ֵ��E_OK--�ɹ����ѣ�OSEK_TASK_NEED_DISPATCH--�ɹ����Ѳ�����Ҫ���µ��ȣ�E_OS_LIMIT--û���㹻�ľ�������á�
 */

StatusType osekEvent_SetEvent(T_OSEK_TASK_ControlBlock *tcbPtr, EventMaskType mask)
{

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// ���ָ����������չ���񣬷���E_OS_ACCESS
	if( (tcbPtr->configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		return E_OS_ACCESS;
	}

	// ���������SUSPEND״̬������E_OS_STATE
	if( tcbPtr->status == SUSPENDED )
	{
		return E_OS_STATE;
	}
#endif

	// ����������������¼���
	tcbPtr->setEvent |= mask;

	// ��������ڵȴ�״̬���������õ��¼�������ȴ����¼���������������
    if( tcbPtr->status == WAITING )
    {
    	if (( tcbPtr->waitEvent & tcbPtr->setEvent) != 0 )
        {
        	return osekTask_WakeTask(tcbPtr);
        }
    }

	// �ɹ�������E_OK
	return E_OK;

}


/*
 * @brief
 *    SetEvent�������¼���
 * @param
 * @param[in]	taskId������ID�š�
 *          	mask���¼����롣
 * @param[out]  �ޡ�
 * @returns:    E_OK�� �ɹ���
 * <p>          [E_OS_ID:��Ч������ID�š�]
 * <p>          [E_OS_ACCESS:ָ����������չ����]
 * <p>          [E_OS_STATE:ָ��������SUSPEND״̬��]
 */
StatusType  SetEvent( TaskType taskId, EventMaskType mask )
{
	T_OSEK_TASK_ControlBlock *tcbPtr;
	StatusType status;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// �������ID������Χ������E_OS_ID
	if( taskId > CONFIG_OSEK_TASK_MAX_ID )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_SetEvent,taskId) ;
	}
#endif

	tcbPtr = &osekTask_TaskTable[taskId];

	OSEK_TARGET_DisableOSInt(osIntSave);

	// �����ڲ�����osekEvent_SetEvent�����¼�
	status = osekEvent_SetEvent(tcbPtr,mask);

	// �����Ҫ��ʵʩ���ȣ��˴����л�����һ������ִ��
	if( status == OSEK_TASK_NEED_DISPATCH )
	{
		osekTask_Dispatch();
	}
	else
	{
		OSEK_TARGET_EnableOSInt(osIntSave);
		if( status != E_OK )
		{
			OSEK_HOOK_ErrorHook(status,OSServiceId_SetEvent,taskId) ;
		}
		else
		{
			return status;
		}
	}

	OSEK_TARGET_EnableOSInt(osIntSave);
	return E_OK;

}


/*
 * @brief
 *    ClearEvent������¼���
 * @param
 * @param[in]	mask���¼����롣
 * @param[out]  �ޡ�
 * @returns:    E_OK�� �ɹ���
 * <p>          [E_OS_ACCESS:������������չ����]
 * <p>          [E_OS_CALLEVEL:���ж��е����˴˺�����]
 */
StatusType  ClearEvent( EventMaskType mask )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// ���������������չ���񣬷���E_OS_ACCESS
	if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_ClearEvent,0) ;
	}

	// ����Ǵ��ж��е��õĴ˺���������E_OS_CALLEVEL
	if( osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_ClearEvent,0) ;
	}
#endif

	OSEK_TARGET_DisableOSInt(osIntSave);
	// ��ָ���¼���������������¼����������
	osekTask_RunningTask->taskControlBlock->setEvent &= (EventMaskType)(~mask);
	OSEK_TARGET_EnableOSInt(osIntSave);

	// �ɹ�������E_OK
	return E_OK;

}


/*
 * @brief
 *    WaitEvent���ȴ��¼���
 * @param
 * @param[in]	mask���¼����롣
 * @param[out]  �ޡ�
 * @returns:    E_OK�� �ɹ���
 * <p>          [E_OS_ACCESS:������������չ����]
 * <p>          [E_OS_RESOURCE:��������ӵ��������Դ��ϵͳ���ڹص��ȵ������]
 * <p>          [E_OS_CALLEVEL:���ж���ʹ���˴˵��á�]
 */
StatusType  WaitEvent( EventMaskType mask )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// ���������������չ���񣬷���E_OS_ACCESS
	if( (osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_WaitEvent,0) ;
	}

#if defined(CONFIG_OSEK_RESOURCE)
	// �������������Դ�б�Ϊ0������E_OS_RESOURCE
	if( osekTask_RunningTask->taskControlBlock->resourceList != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_WaitEvent,0) ;
	}

#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// ����ж���Դû���ͷţ�����E_OS_RESOURCE
    if (osekResource_ISRLast != 0)
    {
        OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_WaitEvent,0) ;
    }
#endif  /* defined(CONFIG_OSEK_RESOURCE_ISR) */

#endif

	// �����������Ϊ0������E_OS_RESOURCE
	if( osekTask_SchedulerLockLevel != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_WaitEvent,0) ;
	}

	// ����ж�Ƕ������Ϊ0������E_OS_CALLEVEL
	if( osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_WaitEvent,0) ;
	}
#endif

	// ���ж�
	OSEK_TARGET_DisableOSInt(osIntSave);

	// ������������ĵȴ��¼�����
	osekTask_RunningTask->taskControlBlock->waitEvent = mask;

	// ������������ѱ����õ��¼���������ȴ����¼����������л����ȴ�״̬
	if( (mask & osekTask_RunningTask->taskControlBlock->setEvent) == 0 )
	{
		osekTask_WaitTask();
	}

	// ���ж�
	OSEK_TARGET_EnableOSInt(osIntSave);

	// �ɹ�������E_OK
	return E_OK;

}


/*
 * @brief
 *    GetEvent����ȡ�¼���
 * @param
 * @param[in]	taskId�������ID�š�
 * @param[out]  mask��  �¼�������������á�
 * @returns:    E_OK�� �ɹ���
 * <p>          [E_OS_ID:��Ч������ID�š�]
 * <p>          [E_OS_ACCESS:ָ����������չ����]
 * <p>          [E_OS_STATE:ָ��������SUSPEND״̬��]
 */
StatusType  GetEvent( TaskType taskId, EventMaskRefType mask )
{

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// �������ID������Χ������E_OS_ID
	if( taskId > CONFIG_OSEK_TASK_MAX_ID )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetEvent,taskId) ;
	}

	// ���ָ����������չ���񣬷���E_OS_ACCESS
	if( (osekTask_TaskTable[taskId].configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetEvent,taskId) ;
	}

	// ���ָ��������SUSPEND״̬������E_OS_STATE
	if( osekTask_TaskTable[taskId].status == SUSPENDED )
	{
		OSEK_HOOK_ErrorHook(E_OS_STATE,OSServiceId_GetEvent,taskId) ;
	}
#endif

	// ��ָ�����������õ��¼���������������
	*mask = osekTask_TaskTable[taskId].setEvent;

	// �ɹ�������E_OK
	return E_OK;

}

#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET)  */


