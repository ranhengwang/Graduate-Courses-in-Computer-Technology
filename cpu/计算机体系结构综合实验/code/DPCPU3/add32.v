`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    21:14:15 04/20/2023 
// Design Name: 
// Module Name:    add32 
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
module add32(a, b, c
    );
	 input [31:0] a, b;
	 output [31:0] c;
	 
	 assign c=a+b;

endmodule