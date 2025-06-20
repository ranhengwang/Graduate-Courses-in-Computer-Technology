/****************************************************************************
 *				���ӿƼ���ѧǶ��ʽ����������� ��Ȩ����
 *
 * 		Copyright (C) 2005-2009 ESEC UESTC. All Rights Reserved.
 ***************************************************************************/

/*
 * �޸ļ�¼��
 *	20061116	�������ļ���
 *
 */

/**
 * @file 	mpc5554Vector.c
 * @brief
 *	<li>���ܣ�ʵ��mpc5554�ж������� </li>
 *
 */


/**************************** ���ò��� ************************************/
#include "osprop.h"

/*************************** ǰ���������� *******************************/


/**************************** ���岿�� ************************************/
#if defined(OCC_MPC5554)


asm(".equ ISR_SRR0_OFFSET,   0x0");
asm(".equ ISR_SRR1_OFFSET,   0x4");

asm(".equ ISR_GPR0_OFFSET,	0x8");
asm(".equ ISR_GPR2_OFFSET,	0xC");
asm(".equ ISR_GPR3_OFFSET,	0x10");
asm(".equ ISR_GPR4_OFFSET,   0x14");
asm(".equ ISR_GPR5_OFFSET,   0x18");
asm(".equ ISR_GPR6_OFFSET,   0x1C");
asm(".equ ISR_GPR7_OFFSET,   0x20");
asm(".equ ISR_GPR8_OFFSET,   0x24");
asm(".equ ISR_GPR9_OFFSET ,  0x28");
asm(".equ ISR_GPR10_OFFSET , 0x2C");
asm(".equ ISR_GPR11_OFFSET,  0x30");
asm(".equ ISR_GPR12_OFFSET,  0x34");
asm(".equ ISR_GPR13_OFFSET, 0x38");


asm(".equ  ISR_CR_OFFSET ,    0x3C");
asm(".equ  ISR_CTR_OFFSET ,   0x40");
asm(".equ  ISR_XER_OFFSET ,   0x44");
asm(".equ  ISR_LR_OFFSET ,    0x48");

asm(".equ  ISR_GPR29_OFFSET ,    0x4C");
asm(".equ  ISR_MASK_OFFSET ,    0x50");

asm(".equ  ISR_FRAME_SIZE,    0x54");


/**************************** ʵ�ֲ��� ************************************/


asm(".macro BSP_SAVE_REGISTERS");
asm("subi 1,1,ISR_FRAME_SIZE");			// r1ΪSP

asm("stw  3,ISR_GPR3_OFFSET(1)");
asm("stw  4,ISR_GPR4_OFFSET(1)");

asm(" mfsrr0  3");
asm(" mfsrr1  4");
asm("stw  3,ISR_SRR0_OFFSET(1)");
asm("stw  4,ISR_SRR1_OFFSET(1)");

asm("stw  0,ISR_GPR0_OFFSET(1)");
asm("stw  2,ISR_GPR2_OFFSET(1)");
asm("stw  5,ISR_GPR5_OFFSET(1)");
asm("stw  6,ISR_GPR6_OFFSET(1)");
asm("stw  7,ISR_GPR7_OFFSET(1)");
asm("stw  8,ISR_GPR8_OFFSET(1)");
asm("stw  9,ISR_GPR9_OFFSET(1)");
asm("stw  10,ISR_GPR10_OFFSET(1)");
asm("stw  11,ISR_GPR11_OFFSET(1)");
asm("stw  12,ISR_GPR12_OFFSET(1)");
asm("stw  13,ISR_GPR13_OFFSET(1)");
asm("stw  29,ISR_GPR29_OFFSET(1)");

asm("mfctr   3");
asm("mfcr    4");
asm("mflr    5");
asm("mfxer   6");
asm("stw     3,ISR_CTR_OFFSET(1)");
asm("stw     4,ISR_CR_OFFSET(1)");
asm("stw     5,ISR_LR_OFFSET(1)");
asm("stw     6,ISR_XER_OFFSET(1)");
asm(".endm");

asm(".macro BSP_RESTORE_REGISTERS");

asm("lwz     3,ISR_CTR_OFFSET(1)");
asm("lwz     4,ISR_CR_OFFSET(1)");
asm("lwz     5,ISR_LR_OFFSET(1)");
asm("lwz     6,ISR_XER_OFFSET(1)");
asm("mtctr   3");
asm("mtcr    4");
asm("mtlr    5");
asm("mtxer   6");

asm("lwz  3,ISR_SRR0_OFFSET(1)");
asm("lwz  4,ISR_SRR1_OFFSET(1)");
asm(" mtsrr0  3");
asm(" mtsrr1  4");

asm("lwz  0,ISR_GPR0_OFFSET(1)");
asm("lwz  2,ISR_GPR2_OFFSET(1)");
asm("lwz  3,ISR_GPR3_OFFSET(1)");
asm("lwz  4,ISR_GPR4_OFFSET(1)");
asm("lwz  5,ISR_GPR5_OFFSET(1)");
asm("lwz  6,ISR_GPR6_OFFSET(1)");
asm("lwz  7,ISR_GPR7_OFFSET(1)");
asm("lwz  8,ISR_GPR8_OFFSET(1)");
asm("lwz  9,ISR_GPR9_OFFSET(1)");
asm("lwz  10,ISR_GPR10_OFFSET(1)");
asm("lwz  11,ISR_GPR11_OFFSET(1)");
asm("lwz  12,ISR_GPR12_OFFSET(1)");
asm("lwz  13,ISR_GPR13_OFFSET(1)");
asm("lwz  29,ISR_GPR29_OFFSET(1)");

asm("addi 1,1,ISR_FRAME_SIZE");
asm("rfi");

asm(".endm");




asm(".equ INTC_IACKR ,0xfff48010");
asm(".equ INTC_EOIR ,0xfff48018");

asm(".text");
asm(".align  4");

asm(".global bspIsrHandler");
asm("bspIsrHandler:");

// �����ж�������
asm("BSP_SAVE_REGISTERS");

// ��ȡ�ж�����,�ж��������豣����r4��,��Ϊ������ж������ж�,�û��ж���ڳ�����Ҷ�Ҫ�õ�.
asm("addis	6, 0, INTC_IACKR@ha");
asm("lwz    4, INTC_IACKR@l(6)");
asm("rlwinm  4,4,30,2,31");		// ������INTVEC�� INTVEC>>2


/* �������õ��ж����ͽ�����ת,�ж������r4�� */
asm("addis	6, 0, osekConfig_InterruptTypeTable@ha");
asm("add 6,6,4");
asm("lbz    5, osekConfig_InterruptTypeTable@l(6)");
asm("cmpwi   5,2");
asm("beq    .isr2");

// ISR1����жϲ������ж�ջ�л�,ֱ�ӻ�ȡ�жϴ���������ڵ�ַ(�ж������r4��),Ȼ����ת�����ִ��.
asm(".isr1:");

//ʹ��ȫ���ж�
asm("wrteei 1");

asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");

asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

// �ж����������־����
asm("mbar 0");
asm("lis	3, INTC_EOIR@ha");
asm("li		4, 0x0");

//����ȫ���ж�
asm("wrteei 0");

asm("stw	4, INTC_EOIR@l(3)");
asm("b .exit");


// ISR2���ж���Ҫ����ջ�л�,�������жϴ�������˳�ʱҪ���������л����ж�����.
asm(".isr2:");

// ��r1������r29��,����ջ�л���Ļָ�
asm("mr 29,1");

// �ж��Ƿ���Ҫ����ջ�л�
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("cmpwi   7,0");
asm("bne    .nested");

// ��δǶ��������л�ջ
asm("addis	8, 0, osekInterrupt_IsrStackTop@ha");
asm("lwz	1, osekInterrupt_IsrStackTop@l(8)");

// ��Ƕ����ȼ�1,ԭ�е�ֵ��r7��
asm(".nested:");
asm("addi 7,7,1");
asm("stw 7,osekInterrupt_NestedLevl@l(6)");

//ʹ��ȫ���ж�
asm("wrteei 1");

//��ȡ�жϴ���������ڵ�ַ(�ж������r4��),Ȼ����ת�����ִ��
asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");
asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

//�ָ�ջ
asm("mr  1,29");

// �ָ��ж�������
asm("mbar 	0");
asm("lis	3, INTC_EOIR@ha");
asm("li		4, 0x0");

//����ȫ���ж�
asm("wrteei 0");

asm("stw	4, INTC_EOIR@l(3)");

// Ƕ����ȼ�1
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("subi    7,7,1");
asm("stw     7, osekInterrupt_NestedLevl@l(6)");

// �ж���ǰ�Ƿ����ж�Ƕ��,������ж�Ƕ���򲻽��������л����ȴ���.
asm("cmpwi   7,0");
asm("bne     .exit");

// �ж��Ƿ���Ҫ���������л�,�����Ҫ������ص���
asm("addis	6, 0, osekTask_IsrTriggeredTaskSwitchNecessary@ha");
asm("lwz	7, osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");
asm("cmpwi   7,0");
asm("beq     .exit");

// �ص�����Ҫͨ������osekTask_Dispatch���
asm(".schedule:");

// �����ȱ�־����
asm("xor 7,7,7");
asm("stw 7,osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");


// �����ص���
asm("subi   1,1,0x8 ");
asm("bl osekTask_Dispatch");
asm("addi 1,1,0x8");


// �ָ��ж�������,�����жϷ���
asm(".exit:");
asm("BSP_RESTORE_REGISTERS");


// FIT�쳣������
asm(".text");
asm(".align  4");
asm(".global FITHandler");
asm("FITHandler:");

// �����ж�������
asm("BSP_SAVE_REGISTERS");


// ��ȡ�ж�����,�ж��������豣����r4��,��Ϊ������ж������ж�,�û��ж���ڳ�����Ҷ�Ҫ�õ�.
asm("lis 4, 0x0000");
asm("ori 4,4, 0x0134");


/* �������õ��ж����ͽ�����ת,�ж������r4�� */
asm("addis	6, 0, osekConfig_InterruptTypeTable@ha");
asm("add 6,6,4");
asm("lbz    5, osekConfig_InterruptTypeTable@l(6)");
asm("cmpwi   5,2");
asm("beq    .fitisr2");

// ISR1����жϲ������ж�ջ�л�,ֱ�ӻ�ȡ�жϴ���������ڵ�ַ(�ж������r4��),Ȼ����ת�����ִ��.
asm(".fitisr1:");

asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");

asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");

asm("b .fitexit");


// ISR2���ж���Ҫ����ջ�л�,�������жϴ�������˳�ʱҪ���������л����ж�����.
asm(".fitisr2:");

// ��r1������r29��,����ջ�л���Ļָ�
asm("mr 29,1");

// �ж��Ƿ���Ҫ����ջ�л�
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("cmpwi   7,0");
asm("bne    .fitnested");

// ��δǶ��������л�ջ
asm("addis	8, 0, osekInterrupt_IsrStackTop@ha");
asm("lwz	1, osekInterrupt_IsrStackTop@l(8)");

// ��Ƕ����ȼ�1,ԭ�е�ֵ��r7��
asm(".fitnested:");
asm("addi 7,7,1");
asm("stw 7,osekInterrupt_NestedLevl@l(6)");


//��ȡ�жϴ���������ڵ�ַ(�ж������r4��),Ȼ����ת�����ִ��
asm("addis	6, 0, osekConfig_InterruptEntryTable@ha");
asm("rotlwi  5, 4,2");
asm("add     6,6,5");
asm("lwz	7, osekConfig_InterruptEntryTable@l(6)");
asm("mtctr 7");
asm("subi   1,1,0x8 ");
asm("bctrl");
asm("addi 1,1,0x8");


//�ָ�ջ
asm("mr  1,29");

// Ƕ����ȼ�1
asm("addis	6, 0, osekInterrupt_NestedLevl@ha");
asm("lwz	7, osekInterrupt_NestedLevl@l(6)");
asm("subi    7,7,1");
asm("stw     7, osekInterrupt_NestedLevl@l(6)");

// �ж���ǰ�Ƿ����ж�Ƕ��,������ж�Ƕ���򲻽��������л����ȴ���.
asm("cmpwi   7,0");
asm("bne     .fitexit");

// �ж��Ƿ���Ҫ���������л�,�����Ҫ������ص���
asm("addis	6, 0, osekTask_IsrTriggeredTaskSwitchNecessary@ha");
asm("lwz	7, osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");
asm("cmpwi   7,0");
asm("beq     .fitexit");

// �ص�����Ҫͨ������osekTask_Dispatch���
asm(".fitschedule:");

// �����ȱ�־����
asm("xor 7,7,7");
asm("stw 7,osekTask_IsrTriggeredTaskSwitchNecessary@l(6)");


// �����ص���
asm("subi   1,1,0x8 ");
asm("bl osekTask_Dispatch");
asm("addi 1,1,0x8");

// �ָ��ж�������,�����жϷ���
asm(".fitexit:");
asm("BSP_RESTORE_REGISTERS");


/**
  * @brief
  *		��ʼ���ж�������
  *
  * @param[in]	�ޡ�
  *
  *
  * @return		�ޡ�
  *
  */
volatile void  BSP_InitializeVector( )
{
  asm ("lis 5, bspIsrHandler@h");   /* IVPR = address base used with IVOR's */
  asm ("mtIVPR 5 ");

  asm ("lis 5, bspIsrHandler@h"); /* IVOR4 = address of handler */
  asm ("ori 5, 5, bspIsrHandler@l");
  asm ("mtIVOR4 5");

  asm ("lis 0, FITHandler@h"); /* IVOR11 = address of handler */
  asm ("ori 0, 0, FITHandler@l");
  asm ("mtIVOR11 0");

}




#endif	// #if defined(OCC_MPC5554)
