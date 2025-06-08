`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:59:04 06/02/2023
// Design Name:   SCCPU
// Module Name:   C:/Users/Lenovo/Desktop/DPCPU3/CPU_tb.v
// Project Name:  SCCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: SCCPU
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module CPU_tb;

	// Inputs
	reg Clock;
	reg Resetn;

	// Outputs
	wire [31:0] PC;
	wire [31:0] if_Inst;
	wire [31:0] exe_Alu_Result;
	wire [31:0] mem_mo;
	wire stall;
	wire exe_wreg;
	wire mem_wreg;
	wire [4:0] exe_rn;
	wire [4:0] mem_rn;
	wire exe_m2reg;
	wire mem_m2reg;
	wire [4:0] rs;
	wire [4:0] rt;
	wire regrt;

	// Instantiate the Unit Under Test (UUT)
	SCCPU uut (
		.Clock(Clock), 
		.Resetn(Resetn), 
		.PC(PC), 
		.if_Inst(if_Inst), 
		.exe_Alu_Result(exe_Alu_Result), 
		.mem_mo(mem_mo), 
		.stall(stall), 
		.exe_wreg(exe_wreg), 
		.mem_wreg(mem_wreg), 
		.exe_rn(exe_rn), 
		.mem_rn(mem_rn), 
		.exe_m2reg(exe_m2reg), 
		.mem_m2reg(mem_m2reg), 
		.rs(rs), 
		.rt(rt), 
		.regrt(regrt)
	);

	initial begin
		// Initialize Inputs
		Clock = 0;
		Resetn = 0;

		// Wait 100 ns for global reset to finish
		#100;
		Resetn=1;
        
		// Add stimulus here

	end
  always#50 Clock=~Clock;   
endmodule

