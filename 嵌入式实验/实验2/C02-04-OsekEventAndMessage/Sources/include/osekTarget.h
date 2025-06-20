/****************************************************************************
 *				���ӿƼ���ѧǶ��ʽ����������� ��Ȩ����
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/**
 * $Log: osekTarget.h,v $
 * Revision 1.0  2008/03/12 11:24:00
 * �����ļ�
*/

/**
 * @file 	osekTarget.h
 * @brief
 *	<li>���ܣ�����CPU��ص����ݽṹ���ꡢ�����ȡ�</li>
 * @author
 * @date 	2008-03-12
 * <p>���ţ�
 */


#ifndef _OSEK_TARGET_H
#define _OSEK_TARGET_H

#include "hidef.h"
/**************************** ���ò��� *********************************/


#ifdef __cplusplus
extern "C" {
#endif

/**************************** �������� *********************************/
#if defined(CONFIG_OSEK_TARGET_X86) || defined(CONFIG_OSEK_TARGET_PPC)
// �ڲ�����
OSDWORD osekTarget_SaveContext( void * savedContext );
void osekTarget_RestoreContext( void * restoredContext );

// ����ĺ������ڵ�һ��������չ����ͻ�������ʱ���������ջ
void osekTarget_LoadETSP(void* sp) ;
void osekTarget_LoadBTSP( void );
void osekTarget_SaveBTSP(OSDWORD sp );

// ��������
extern OSDWORD osekTarget_AllIntMask;
extern OSDWORD osekTarget_NestedAllIntMask;
extern OSDWORD osekTarget_NestedOsIntMask;
#if defined(CONFIG_OSEK_TARGET_X86)
extern OSDWORD osekTarget_SavedBTSP;
#endif
#endif

/**************************** ���岿�� *********************************/

#if defined(CONFIG_OSEK_TARGET_PPC)


/// PPC�����������Ľṹ����������PPCĿ���ʱ�ýṹ�Ŵ���
typedef struct T_OSEK_TARGET_TaskContext_struct
{
	/// pc�Ĵ���
	OSDWORD pc;
	/// sp�Ĵ���
	OSDWORD sp;
	/// gpr14-gpr31
	OSDWORD gpr[18];
#if defined(CONFIG_OSEK_TARGET_FLOAT)
	/// fpr14-fpr31���������˸��㴦��ʱ�����ԲŴ���
	double  fpr[18];
#endif/* #if defined(CONFIG_OSEK_TARGET_FLOAT)  */
}T_OSEK_TARGET_TaskContext_struct;


#define OSEK_TARGET_SaveBTSP(sp) osekTarget_SaveBTSP(sp)


// ����ĺ������ں��з����ٽ�������ʱ�����ж�ʹ�ã�ԭ����Ӧ��ֻ�ر�ISR2����ж�

#if defined(CONFIG_OSEK_COMPILER_GCC)

#if defined(CONFIG_OSEK_TARGET_MPC5554)

#define OSEK_TARGET_OSIntSave( osIntSave ) OSDWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave) asm("mfmsr %0" : "=r"(osIntSave):); asm ("wrteei 0")
#define OSEK_TARGET_EnableOSInt(osIntSave)  asm("mtmsr %0": :"r"(osIntSave))

#define OSEK_TARGET_DisableOSIntWithoutSave() asm ("wrteei 0")
#define OSEK_TARGET_EnableOSIntWithoutSave()  asm ("wrteei 1")

#define OSEK_TARGET_DisableAllInt() asm("mfmsr %0" : "=r"(osekTarget_AllIntMask):); asm ("wrteei 0")
#define OSEK_TARGET_EnableAllInt()  asm("mtmsr %0": :"r"(osekTarget_AllIntMask))

#define OSEK_TARGET_DisableNestedAllInt()  asm("mfmsr %0" : "=r"(osekTarget_NestedAllIntMask):); asm ("wrteei 0")
#define OSEK_TARGET_EnableNestedAllInt()   asm("mtmsr %0": :"r"(osekTarget_NestedAllIntMask))

#define OSEK_TARGET_DisableNestedOsInt() asm("mfmsr %0" : "=r"(osekTarget_NestedOsIntMask):); asm ("wrteei 0")
#define OSEK_TARGET_EnableNestedOsInt()  asm("mtmsr %0": :"r"(osekTarget_NestedOsIntMask))

#endif //#if defined(CONFIG_OSEK_TARGET_MPC5554)


#if defined(CONFIG_OSEK_TARGET_MPC555)

#define OSEK_TARGET_OSIntSave( osIntSave ) OSDWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave) asm("mfmsr %0" : "=r"(osIntSave):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableOSInt(osIntSave)  asm("mtmsr %0": :"r"(osIntSave))

#define OSEK_TARGET_DisableOSIntWithoutSave() asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableOSIntWithoutSave()  asm ("mtspr 80, 0")

#define OSEK_TARGET_DisableAllInt() asm("mfmsr %0" : "=r"(osekTarget_AllIntMask):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableAllInt()  asm("mtmsr %0": :"r"(osekTarget_AllIntMask))

#define OSEK_TARGET_DisableNestedAllInt()  asm("mfmsr %0" : "=r"(osekTarget_NestedAllIntMask):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableNestedAllInt()   asm("mtmsr %0": :"r"(osekTarget_NestedAllIntMask))

#define OSEK_TARGET_DisableNestedOsInt() asm("mfmsr %0" : "=r"(osekTarget_NestedOsIntMask):); asm ("mtspr 81, 0")
#define OSEK_TARGET_EnableNestedOsInt()  asm("mtmsr %0": :"r"(osekTarget_NestedOsIntMask))

#endif /* #if defined(CONFIG_OSEK_TARGET_MPC555) */

#endif /* #if defined(CONFIG_OSEK_COMPILER_GCC) */

#endif /* defined(CONFIG_OSEK_TARGET_PPC)  */



#if defined(CONFIG_OSEK_TARGET_X86)


/// X86�����������Ľṹ����������X86Ŀ���ʱ�ýṹ�Ŵ���
typedef struct T_OSEK_TARGET_TaskContext_struct
{
	/// eflags�Ĵ���
	OSDWORD eflags;
	/// sp�Ĵ���
	OSDWORD sp;
	/// ebp�Ĵ���
	OSDWORD ebp;
	/// ebx�Ĵ���
	OSDWORD ebx;
	/// esi�Ĵ���
	OSDWORD esi;
	/// edi�Ĵ���
	OSDWORD edi;
	/// pc�Ĵ���
	OSDWORD pc;
#if defined(CONFIG_OSEK_TARGET_FLOAT)
	/// ����Ĵ������������˸��㴦��ʱ�����ԲŴ���
	OSBYTE  fpr[108];
#endif/* #if defined(CONFIG_OSEK_TARGET_FLOAT)  */

}T_OSEK_TARGET_TaskContext_struct;


/// X86���쳣��ܽṹ����������X86Ŀ���ʱ�ýṹ�Ŵ���
typedef struct T_OSEK_TARGET_ExceptionFrame_struct{
	/// edi�Ĵ���
	OSDWORD  edi;
	/// esi�Ĵ���
	OSDWORD  esi;
	/// ebp�Ĵ���
	OSDWORD  ebp;
	/// esp0�Ĵ���
	OSDWORD  esp0;
	/// ebx�Ĵ���
	OSDWORD  ebx;
	/// edx�Ĵ���
	OSDWORD  edx;
	/// ecx�Ĵ���
	OSDWORD  ecx;
	/// eax�Ĵ���
	OSDWORD  eax;
	/// idtIndex�Ĵ���
	OSDWORD  idtIndex;
	/// faultCode�Ĵ���
	OSDWORD  faultCode;
	/// eip�Ĵ���
	OSDWORD  eip;
	/// cs�Ĵ���
	OSDWORD  cs;
	/// eflags�Ĵ���
	OSDWORD  eflags;
}T_OSEK_TARGET_ExceptionFrame_struct;


#define OSEK_TARGET_SaveBTSP(sp) (osekTarget_SavedBTSP = (sp) - (OSDWORD)0x20)


// ����ĺ������ں��з����ٽ�������ʱ�����ж�ʹ�ã�ԭ����Ӧ��ֻ�ر�ISR2����ж�

#if defined(CONFIG_OSEK_COMPILER_GCC)

#define OSEK_TARGET_OSIntSave( osIntSave ) OSDWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave)  asm("pushf"); asm("cli"); asm("popl %0" : "=g"(osIntSave):)
#define OSEK_TARGET_EnableOSInt(osIntSave)   asm("push %0": :"g"(osIntSave));asm("popf")

#define OSEK_TARGET_DisableOSIntWithoutSave()  asm("cli")
#define OSEK_TARGET_EnableOSIntWithoutSave()   asm("sti")

#define OSEK_TARGET_DisableAllInt() asm("pushf");asm("popl osekTarget_AllIntMask");asm("cli")
#define OSEK_TARGET_EnableAllInt()  asm("pushl osekTarget_AllIntMask"); asm("popf")

#define OSEK_TARGET_DisableNestedAllInt() asm("pushf");asm("popl osekTarget_NestedAllIntMask");asm("cli")
#define OSEK_TARGET_EnableNestedAllInt()  asm("pushl osekTarget_NestedAllIntMask"); asm("popf")

#define OSEK_TARGET_DisableNestedOsInt() asm("pushf");asm("popl osekTarget_NestedOsIntMask");asm("cli")
#define OSEK_TARGET_EnableNestedOsInt()  asm("pushl osekTarget_NestedOsIntMask"); asm("popf")

#endif /* #if defined(CONFIG_OSEK_COMPILER_GCC) */


#endif /* defined(CONFIG_OSEK_TARGET_X86)  */

//**********if defined(TARGET_HCS12X)*************
#if defined(CONFIG_OSEK_TARGET_HCS12X)

// �ڲ�����
OSDWORD osekTarget_SaveContext( void * savedContext );
void osekTarget_RestoreContext( void * restoredContext );
int fun(void * savedContext );

// ����ĺ������ڵ�һ��������չ����ͻ�������ʱ���������ջ
void osekTarget_LoadETSP(void* sp) ;
void osekTarget_LoadBTSP( void );
void osekTarget_SaveBTSP(OSWORD sp );

// ��������
extern OSWORD osekTarget_AllIntMask;
extern OSWORD osekTarget_NestedAllIntMask;
extern OSWORD osekTarget_NestedOsIntMask;
extern OSWORD osekTarget_SavedBTSP;


typedef struct T_OSEK_TARGET_TaskContext_struct
{


OSWORD CCR;
OSWORD sp;
OSWORD X;
OSWORD Y;
OSBYTE PAGE;
OSWORD pc;


}T_OSEK_TARGET_TaskContext_struct;  





#define OSEK_TARGET_SaveBTSP(sp) 	  	(osekTarget_SavedBTSP = (sp) - (OSWORD)0x20)
#define osekTarget_LoadBTSP()	  	  	asm(LDS osekTarget_SavedBTSP)
#define osekTarget_LoadETSP(sp)   		{unsigned char*temp=sp;asm(LDS temp);}


#define OSEK_TARGET_OSIntSave( osIntSave ) OSWORD osIntSave

#define OSEK_TARGET_DisableOSInt(osIntSave)  {asm PSHCW ;asm PULD ; asm STD osIntSave;DisableInterrupts}
#define OSEK_TARGET_EnableOSInt(osIntSave)   {asm(LDD osIntSave);asm(PSHD);asm(PULCW);}


#define OSEK_TARGET_DisableOSIntWithoutSave()  DisableInterrupts
#define OSEK_TARGET_EnableOSIntWithoutSave()   EnableInterrupts

#define OSEK_TARGET_DisableAllInt()   {asm PSHCW;asm PULD; asm STD osekTarget_AllIntMask ;DisableInterrupts;} 
#define OSEK_TARGET_EnableAllInt() 	  {asm LDD osekTarget_AllIntMask ;asm PSHD ;asm PULCW;} 

#define OSEK_TARGET_DisableNestedAllInt()	{asm(PSHCW);asm(PULD);asm(STD osekTarget_NestedAllIntMask);DisableInterrupts;}
#define OSEK_TARGET_EnableNestedAllInt()    {asm(LDD osekTarget_NestedAllIntMask);asm(PSHD);asm(PULCW);}

#define OSEK_TARGET_DisableNestedOsInt()   {asm(PSHCW);asm(PULD);asm(STD osekTarget_NestedOsIntMask);DisableInterrupts;}
#define OSEK_TARGET_EnableNestedOsInt()    {asm(LDD osekTarget_NestedOsIntMask);asm(PSHD);asm(PULCW);}

/*CLEAR_IPL is used to clear IPL bits when a new task is activated in ISR state*/ 
#define CLEAR_IPL {asm(TFR CCRH,A);asm(ANDA #$F8);asm(TFR A,CCRH);}

#endif 
//**********endif TARGET_HCS12X*************





#ifdef __cplusplus
}
#endif

#endif

