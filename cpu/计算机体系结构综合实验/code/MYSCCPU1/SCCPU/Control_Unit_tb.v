`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:12:26 05/31/2023
// Design Name:   Control_Unit
// Module Name:   C:/Users/Administrator/Desktop/MYSCCPU/SCCPU/Control_Unit_tb.v
// Project Name:  SCCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Control_Unit
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module Control_Unit_tb;

	// Inputs
	reg rsrtequ;
	reg [5:0] func;
	reg [5:0] op;

	// Outputs
	wire wreg;
	wire m2reg;
	wire wmem;
	wire [2:0] aluc;
	wire regrt;
	wire aluimm;
	wire sext;
	wire [1:0] pcsource;
	wire shift;

	// Instantiate the Unit Under Test (UUT)
	Control_Unit uut (
		.rsrtequ(rsrtequ), 
		.func(func), 
		.op(op), 
		.wreg(wreg), 
		.m2reg(m2reg), 
		.wmem(wmem), 
		.aluc(aluc), 
		.regrt(regrt), 
		.aluimm(aluimm), 
		.sext(sext), 
		.pcsource(pcsource), 
		.shift(shift)
	);

	initial begin
		// Initialize Inputs
		rsrtequ = 0;
		func = 0;
		op = 0;

		//寄存器与
		#100;
      rsrtequ = 0;
		func = 6'b000001;
		op = 6'b000001;
		
		//逻辑右移
		#100;
      rsrtequ = 0;
		func = 6'b000010;
		op = 6'b000010;
		
		//立即数加法
		#100;
      rsrtequ = 0;
		func = 6'b000000;
		op = 6'b000101;
		
		//load
		#100;
      rsrtequ = 0;
		func = 6'b000000;
		op = 6'b001101;
		
		//beq（不跳转）
		#100;
      rsrtequ = 0;
		func = 6'b000000;
		op = 6'b001111;
		
		//beq（跳转）
		#100;
      rsrtequ = 1;
		func = 6'b000000;
		op = 6'b001111;
		
		//jump
		#100;
      rsrtequ = 0;
		func = 6'b000000;
		op = 6'b010010;


	end
   
endmodule

