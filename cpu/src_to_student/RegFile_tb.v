`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   09:19:19 05/11/2020
// Design Name:   RegFile
// Module Name:   D:/computer_org_lab/mips-cpu-master/lab1/src/RegFile_tb.v
// Project Name:  lab1
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: RegFile
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module RegFile_tb;

	// Inputs
	reg reset;
	reg clk;
	reg [4:0] addr1;
	reg [4:0] addr2;
	reg wr;
	reg [4:0] addr3;
	reg [31:0] data3;

	// Outputs
	wire [31:0] data1;
	wire [31:0] data2;

	// Instantiate the Unit Under Test (UUT)
	RegFile uut (
		.reset(reset), 
		.clk(clk), 
		.addr1(addr1), 
		.data1(data1), 
		.addr2(addr2), 
		.data2(data2), 
		.wr(wr), 
		.addr3(addr3), 
		.data3(data3)
	);

	initial begin
		// Initialize Inputs
		reset = 0;
		clk = 0;
		addr1 = 0;
		addr2 = 0;
		wr = 0;
		addr3 = 0;
		data3 = 0;

		// Wait 100 ns for global reset to finish
		#100;
		addr3=01;
		data3=32'h00ff00ff;
		wr=1;
		#110;
		addr3=02;
		data3=32'Haa55aa55;
		wr=1;
		#120
		addr1=01;
		addr2=02;
		wr=0;
		
      #130  
		addr1=00;
		addr2=03;
		wr=0;
		// Add stimulus here

	end

initial fork
	#13 reset <= 1;
	forever #10 clk = ~clk;
join


endmodule

