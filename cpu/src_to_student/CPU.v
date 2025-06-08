module CPU(reset, sysclk,IRQ);
input reset, sysclk;
input IRQ;
// Generate CPU Clock
wire clk;
CPU_clk CPU_clk_1(.sysclk(sysclk), .reset(reset), .clk(clk));

// Instruction Fetch
reg [31:0] PC;
wire [31:0] Instruct;
ROM ROM_1(.addr(PC), .data(Instruct));

// Instruction Decode
wire [2:0] PCSrc;
wire RegWrite, MemRead, MemWrite, ALUSrc1, ALUSrc2, EXTOp, LUOp, Sign, IRQ;
wire [1:0] RegDst, MemToReg;
wire [5:0] ALUFun;

Control Control_1(.Instruct(Instruct), .IRQ(IRQ), .PC31(PC[31]), .PCSrc(PCSrc), .RegWr(RegWrite), .RegDst(RegDst), .MemRd(MemRead), .MemWr(MemWrite), .MemToReg(MemToReg), .ALUSrc1(ALUSrc1), .ALUSrc2(ALUSrc2), .EXTOp(EXTOp), .LUOp(LUOp), .ALUFun(ALUFun), .Sign(Sign));

// Instruction Decode (Register File) (& Write Back)
wire [31:0] DataBusA, DataBusB, DataBusC;
wire [4:0] Rs, Rt, Rd, AddrC;
parameter Xp = 5'd26;
parameter Ra = 5'd31;
assign Rs = Instruct[25:21];
assign Rt = Instruct[20:16];
assign Rd = Instruct[15:11];
assign AddrC = (RegDst == 2'd0) ? Rd :
			   (RegDst == 2'd1) ? Rt :
			   (RegDst == 2'd2) ? Ra : Xp;
RegFile RegFile_1(.reset(reset), .clk(clk), .addr1(Rs), .data1(DataBusA), .addr2(Rt), .data2(DataBusB), .wr(RegWrite), .addr3(AddrC), .data3(DataBusC));

// Immediate Extension
wire [31:0] EXTOut;
assign EXTOut = {EXTOp ? {16{Instruct[15]}} : 16'h0000, Instruct[15:0]};

// Load Upper Immediate
wire [31:0] LUOut;
assign LUOut = LUOp ? {Instruct[15:0], 16'd0} : EXTOut;

// Execution
wire [31:0] ALU_A, ALU_B, ALUOut;
assign ALU_A = ALUSrc1 ? {27'd0, Instruct[10:6]} : DataBusA;
assign ALU_B = ALUSrc2 ? LUOut : DataBusB;
ALU ALU_1(.A(ALU_A), .B(ALU_B), .ALUFun(ALUFun), .Sign(Sign), .Z(ALUOut));

// Memory Read
wire[31:0] memRdData;
DataMem DataMem_1(.reset(reset), .clk(clk), .rd(MemRead), .wr(MemWrite), .addr(ALUOut), .wdata(DataBusB), .rdata(memRdData));

// Write Back
wire [31:0] ReadData, PC_add_4;
assign ReadData = memRdData;
assign DataBusC = (MemToReg == 2'd0) ? ALUOut :
				  (MemToReg == 2'd1) ? ReadData : PC_add_4;

// PC Register
wire [31:0] PC_add, JT, ConBA;
parameter ILLOP = 32'h8000_0004;
parameter XADR = 32'h8000_0008;
assign PC_add = PC + 32'd4;
assign PC_add_4 = {PC[31], PC_add[30:0]};
assign JT = {PC_add_4[31:28], Instruct[25:0], 2'b00};
assign ConBA = PC_add_4 + {LUOut[29:0], 2'b00};
always @(negedge reset or posedge clk) begin
	if(~reset)
		PC <= 32'h8000_0000;
	else begin
		case(PCSrc)
			3'd0: PC <= PC_add_4;
			3'd1: PC <= (ALUOut[0]) ? ConBA : (PC + 4);
			3'd2: PC <= JT;
			3'd3: PC <= DataBusA;
			3'd4: PC <= ILLOP;
			3'd5: PC <= XADR;
			default: PC <= 32'h8000_0000;
		endcase
	end
end


endmodule