`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:21:26 04/27/2023 
// Design Name: 
// Module Name:    EXE_MEMreg 
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
module EXE_MEMreg(clk,clrn,
	exe_Alu_Result,exe_rb,exe_wmem,exe_m2reg,exe_wreg,exe_rn,
	mem_Alu_Result,mem_rb,mem_wmem,mem_m2reg,mem_wreg,mem_rn
    );
	input exe_wmem,exe_m2reg,exe_wreg;
	input clk, clrn;
	input [4:0] exe_rn;
	input [31:0] exe_Alu_Result,exe_rb;
	output mem_wmem,mem_m2reg,mem_wreg;
	output [4:0] mem_rn;
	output [31:0] mem_Alu_Result,mem_rb;
	
	reg mem_wmem,mem_m2reg,mem_wreg;
	reg [4:0] mem_rn;
	reg [31:0] mem_Alu_Result,mem_rb;
	
	always @ (posedge clk or negedge clrn)
		if(clrn ==0 )
			begin 
				mem_wmem<=0;
				mem_m2reg<=0;
				mem_wreg<=0;
				mem_rn<=0;
				mem_Alu_Result<=0;
				mem_rb<=0;
			end
		else
			begin
				mem_wmem<=exe_wmem;
				mem_m2reg<=exe_m2reg;
				mem_wreg<=exe_wreg;
				mem_rn<=exe_rn;
				mem_Alu_Result<=exe_Alu_Result;
				mem_rb<=exe_rb;
			end

endmodule
