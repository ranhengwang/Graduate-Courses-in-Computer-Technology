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
    
    input E_Wreg, M_Wreg, regrt;  //输入写信号以及rn选择信号
    input [4:0] E_Rn, M_Rn, Rs, Rt;  //前面指令的目的寄存器以及后面指令的源寄存器
    output stall;  //输出暂停信号

    wire stall1, stall2;

    assign stall1 = ((Rs == E_Rn) | (Rt == E_Rn) & ~regrt) & (E_Rn != 0) & E_Wreg; //ID级与EXE级的数据冒险，需要两个个stall
    assign stall2 = ((Rs == M_Rn) | (Rt == M_Rn) & ~regrt) & (M_Rn != 0) & M_Wreg;
    assign stall = stall1 | stall2;



endmodule
