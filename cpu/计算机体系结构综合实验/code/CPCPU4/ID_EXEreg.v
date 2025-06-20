`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    19:56:02 04/27/2023 
// Design Name: 
// Module Name:    ID_EXEreg 
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
module ID_EXEreg(clk,clrn,
	id_m2reg,id_wmem,id_aluc,id_aluimm,id_ra,id_rb,id_imm,id_shift,id_wreg,id_rn,
	exe_m2reg,exe_wmem,exe_aluc,exe_aluimm,exe_ra,exe_rb,exe_imm,exe_shift,exe_wreg,exe_rn
    );
	input id_m2reg,id_wmem,id_aluimm,id_shift,id_wreg;
	input [2:0] id_aluc;
	input [4:0] id_rn;
	input [31:0] id_ra,id_rb,id_imm;
	input clk,clrn;
	output exe_m2reg,exe_wmem,exe_aluimm,exe_shift,exe_wreg;
	output [2:0] exe_aluc;
	output [4:0] exe_rn;
	output [31:0] exe_ra,exe_rb,exe_imm;
	 
	reg exe_m2reg,exe_wmem,exe_aluimm,exe_shift,exe_wreg;
	reg [2:0] exe_aluc;
	reg [4:0] exe_rn;
	reg [31:0] exe_ra,exe_rb,exe_imm;
	
	 always @ (posedge clk or negedge clrn)
		if(clrn ==0 )
			begin 
				exe_m2reg<=0;
				exe_wmem<=0;
				exe_aluimm<=0;
				exe_shift<=0;
				exe_wreg<=0;
				exe_aluc<=0;
				exe_ra<=0;
				exe_rb<=0;
				exe_rn<=0;
				exe_imm<=0;
			end
		else
			begin
				exe_m2reg<=id_m2reg;
				exe_wmem<=id_wmem;
				exe_aluimm<=id_aluimm;
				exe_shift<=id_shift;
				exe_wreg<=id_wreg;
				exe_aluc<=id_aluc;
				exe_ra<=id_ra;
				exe_rb<=id_rb;
				exe_rn<=id_rn;
				exe_imm<=id_imm;
			end
endmodule
