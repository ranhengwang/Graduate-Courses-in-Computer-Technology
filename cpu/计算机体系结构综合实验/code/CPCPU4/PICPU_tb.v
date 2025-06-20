`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   11:53:18 06/02/2023
// Design Name:   SCCPU
// Module Name:   C:/Users/Lenovo/Desktop/CPCPU4/PICPU_tb.v
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

module PICPU_tb;

	// Inputs
	reg Clock;
	reg Resetn;

	// Outputs
	wire [31:0] PC;
	wire [31:0] if_Inst;
	wire [31:0] exe_Alu_Result;
	wire [31:0] mem_mo;

	// Instantiate the Unit Under Test (UUT)
	SCCPU uut (
		.Clock(Clock), 
		.Resetn(Resetn), 
		.PC(PC), 
		.if_Inst(if_Inst), 
		.exe_Alu_Result(exe_Alu_Result), 
		.mem_mo(mem_mo)
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
     always#50 Clock=~Clock 
endmodule

