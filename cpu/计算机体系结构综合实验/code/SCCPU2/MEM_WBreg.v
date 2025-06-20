`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:39:30 04/27/2023 
// Design Name: 
// Module Name:    MEM_WBreg 
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
module MEM_WBreg(clk,clrn,
  mem_Alu_Result,mem_mo,mem_m2reg,mem_wreg,mem_rn,
	wb_Alu_Result,wb_mo,wb_m2reg,wb_wreg,wb_rn
    );
	input mem_m2reg,mem_wreg;
	input clk, clrn;
	input [4:0] mem_rn;
	input [31:0] mem_Alu_Result,mem_mo;
	output wb_m2reg,wb_wreg;
	output [4:0] wb_rn;
	output [31:0] wb_Alu_Result,wb_mo;
	
	reg wb_m2reg,wb_wreg;
	reg [4:0] wb_rn;
	reg [31:0] wb_Alu_Result,wb_mo;

	
	always @ (posedge clk or negedge clrn)
		if(clrn ==0 )
			begin 
				
				wb_m2reg<=0;
				wb_wreg<=0;
				wb_rn<=0;
				wb_Alu_Result<=0;
				wb_mo<=0;
			end
		else
			begin
				
				wb_m2reg<=mem_m2reg;
				wb_wreg<=mem_wreg;
				wb_rn<=mem_rn;
				wb_Alu_Result<=mem_Alu_Result;
				wb_mo<=mem_mo;
			end


endmodule
