`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:34:08 06/01/2023 
// Design Name: 
// Module Name:    STALL 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module STALL(
    E_Wreg, E_Rn, 
    Rs, Rt, regrt,
    M_Wreg, M_Rn, stall
    );
    
    input E_Wreg, M_Wreg, regrt;  //����д�ź��Լ�rnѡ���ź�
    input [4:0] E_Rn, M_Rn, Rs, Rt;  //ǰ��ָ���Ŀ�ļĴ����Լ�����ָ���Դ�Ĵ���
    output stall;  //�����ͣ�ź�

    wire stall1, stall2;

    assign stall1 = ((Rs == E_Rn) | (Rt == E_Rn) & ~regrt) & (E_Rn != 0) & E_Wreg; //ID����EXE��������ð�գ���Ҫ������stall
    assign stall2 = ((Rs == M_Rn) | (Rt == M_Rn) & ~regrt) & (M_Rn != 0) & M_Wreg;
    assign stall = stall1 | stall2;



endmodule
