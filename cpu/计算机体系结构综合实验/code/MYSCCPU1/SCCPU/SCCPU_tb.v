`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   16:35:33 05/31/2023
// Design Name:   SCCPU
// Module Name:   C:/Users/Administrator/Desktop/MYSCCPU/SCCPU/SCCPU_tb.v
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

module SCCPU_tb;

	// Inputs
	reg Clock;
	reg Resetn;

	// Outputs
	wire [31:0] PC;
	wire [31:0] Inst;
	wire [31:0] Alu_Result;

	// Instantiate the Unit Under Test (UUT)
	SCCPU uut (
		.Clock(Clock), 
		.Resetn(Resetn), 
		.PC(PC), 
		.Inst(Inst), 
		.Alu_Result(Alu_Result)
	);

	initial begin
		// Initialize Inputs
		Clock = 0;
		Resetn = 0;

		// Wait 100 ns for global reset to finish
		#100;
		Resetn = 1;
        
		// Add stimulus here

	end
   always #50 Clock=~Clock;
	 
	
endmodule

