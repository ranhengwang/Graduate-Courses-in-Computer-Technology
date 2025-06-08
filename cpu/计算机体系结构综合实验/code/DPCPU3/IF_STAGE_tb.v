`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   21:35:04 04/20/2023
// Design Name:   IF_STAGE
// Module Name:   C:/Users/Administrator/Desktop/MYSCCPU/SCCPU/IF_STAGE_tb.v
// Project Name:  SCCPU
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: IF_STAGE
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module IF_STAGE_tb;

	// Inputs
	reg clk;
	reg clrn;
	reg [1:0] pcsource;
	reg [31:0] bpc;
	reg [31:0] jpc;

	// Outputs
	wire [31:0] pc4;
	wire [31:0] inst;
	wire [31:0] PC;

	// Instantiate the Unit Under Test (UUT)
	IF_STAGE uut (
		.clk(clk), 
		.clrn(clrn), 
		.pcsource(pcsource), 
		.bpc(bpc), 
		.jpc(jpc), 
		.pc4(pc4), 
		.inst(inst), 
		.PC(PC)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		clrn = 0;
		pcsource = 0;
		bpc = 0;
		jpc = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
      
endmodule

