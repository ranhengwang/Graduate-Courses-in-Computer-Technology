`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   15:38:17 05/31/2023
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

		//选择pc4
		#100;
		clrn=1;
		pcsource = 0;
		bpc = 32'h00000008;
		jpc = 32'h00000004;
		
		//选择bpc
		#100;
		clrn=1;
		pcsource = 1;
		bpc = 32'h0000000c;
		jpc = 32'h00000004;
		
		//选择jpc
		#100;
		clrn=1;
		pcsource = 2;
		bpc = 32'h00000008;
		jpc = 32'h00000004;
		
		//选择0
		#100;
		clrn=1;
		pcsource = 3;
		bpc = 32'h00000008;
		jpc = 32'h00000004;
		
		//置位0
		#100;
		clrn=0;
		pcsource = 1;
		bpc = 32'h00000008;
		jpc = 32'h00000004;
		
		
        

	end
  always#50 clk=~clk;
      
endmodule

