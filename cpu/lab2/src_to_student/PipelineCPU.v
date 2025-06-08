`timescale 1ns/1ps

module PipelineCPU(reset, sysclk, IRQ);

/************************IO*************************/
input reset, sysclk;
input IRQ;

wire irq;

wire [31:0] memRdData;

wire [31:0] ReadData;
wire [31:0] WriteData;
wire [31:0] MemAddr;
wire MemRead;
wire MemWrite;
/************************Clk*************************/
wire clk;
assign clk = sysclk;

/************************Core*************************/
Pipeline_Core Pipeline_Core_1(.clk(clk), .reset(reset), .MemAddr(MemAddr), .WriteData(WriteData), 
	.ReadData(ReadData), .MemWrite(MemWrite), .MemRead(MemRead), .IRQ_in(irq));

/***********************Peripheral********************/
DataMem DataMem_1(.reset(reset), .clk(clk), .rd(MemRead), .wr(MemWrite), .addr(MemAddr), .wdata(WriteData), .rdata(memRdData));

assign irq = IRQ;
assign ReadData = memRdData;

endmodule