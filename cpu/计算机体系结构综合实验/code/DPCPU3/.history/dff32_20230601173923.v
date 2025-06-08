`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:40:58 05/15/2019 
// Design Name: 
// Module Name:    dff32 
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
module dff32(d,clk,clrn,q,stall
    );
	 input [31:0] d;
	 input clk,clrn;
	 input stall;
	 output [31:0] q;
    reg [31:0] q;
    always @ (negedge clrn or posedge clk)
	 if(clrn==0)      //复位信号为0时，PC值为0
	     begin
		      q<=0;
		  end
	else if(stall==1) //stall信号为1时，PC值保持不变
	     begin
		      q<=q;
		  end
    else	          //否则，PC值加4
        begin		      
		      q<=d;
		  end	 
endmodule