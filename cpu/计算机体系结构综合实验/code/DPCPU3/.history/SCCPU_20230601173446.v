`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:33:59 05/14/2019 
// Design Name: 
// Module Name:    SCCPU 
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
/////////////////////////////////////////////////////////////////////////////////
module SCCPU(Clock, Resetn, PC, if_Inst, exe_Alu_Result,mem_mo,stall,exe_wreg,mem_wreg,exe_rn, mem_rn,exe_m2reg,mem_m2reg,rs,rt,regrt
    );
	 input Clock, Resetn;//输入的时钟及复位信号
	 output [31:0] PC, if_Inst, exe_Alu_Result,mem_mo; //输出当前PC值，指令，alu计算结果，DM取出的数据
	 output stall; //暂停信号输出
	 output exe_wreg,mem_wreg;   //写寄存器堆信号
	 output [4:0] exe_rn, mem_rn;  //要写的寄存器
	 output exe_m2reg,mem_m2reg;  //写寄存器堆数据来源
	 output [4:0] rs,rt;  //源寄存器
	 output regrt;  //是否用rt
	 
	 wire [31:0] mem_Alu_Result, wb_Alu_Result;//ALU结果
	 wire [1:0] pcsource;  //PC选择器
	 wire [4:0] rs,rt;  //源寄存器传递
	 wire regrt;  //是否用rt
	 wire stall;  //暂停信号
	 
	 wire [31:0] bpc, jpc, if_pc4, id_pc4, id_Inst,if_Inst; //pc及指令
	 
	 wire [31:0] wdi, id_ra, exe_ra, id_rb, exe_rb, mem_rb, id_imm, exe_imm;
	 wire id_m2reg, exe_m2reg,mem_m2reg,wb_m2reg;//写回数据选择器
	 
	 wire id_wmem, exe_wmem, id_aluimm, exe_aluimm, id_shift, exe_shift, z;
	 
	 wire [2:0] id_aluc, exe_aluc; //alu操作码
	 
	 wire id_wreg, exe_wreg, mem_wreg, wb_wreg; //写寄存器堆信号传递
	 wire [4:0] id_rn, exe_rn, mem_rn, wb_rn; //rn传递
	 
	 wire [31:0] mem_mo, wb_mo;//DM取出的数据

	 wire id_wreg_org,id_wmem_org; //译码阶段的写寄存器堆和写DM的信号

	 //取指阶段
	 IF_STAGE stage1 (Clock, Resetn, pcsource, bpc, jpc, if_pc4, if_Inst, PC,stall);
	 
	 //锁存并传递pc4，Inst
	 IF_IDreg  IF_ID (Clock,Resetn,if_pc4,if_Inst,id_pc4,id_Inst);
	 
	 //译码阶段
	 ID_STAGE stage2 (id_pc4, id_Inst, wdi, Clock, Resetn, bpc, jpc, pcsource,
				   id_m2reg, id_wmem_org, id_aluc, id_aluimm, id_ra, id_rb, id_imm, 
					id_shift, z,id_wreg_org,id_rn,rs, rt, regrt);	
	
	//加入stall后的wreg和wmem信号(译码后的信号更新)
	assign id_wreg = ~stall & id_wreg_org;		
	assign id_wmem = ~stall & id_wmem_org;

	 //锁存并传递	m2reg, wmem, aluc, aluimm, ra, rb, imm, shift，wreg，rn
	 ID_EXEreg ID_EXE (Clock,Resetn,id_m2reg,id_wmem,id_aluc,id_aluimm,
			id_ra,id_rb,id_imm,id_shift,id_wreg,id_rn,exe_m2reg,exe_wmem,
			exe_aluc,exe_aluimm,exe_ra,exe_rb,exe_imm,exe_shift,exe_wreg,exe_rn);

	 //执行阶段
	 EXE_STAGE stage3 (exe_aluc, exe_aluimm, exe_ra, exe_rb, exe_imm, exe_shift, exe_Alu_Result,
			z,exe_m2reg,exe_wmem,exe_wreg,exe_rn);
	 
	 //锁存并传递Alu_Result, rb，wmem，m2reg，wreg，rn
	 EXE_MEMreg EXE_MEM(Clock,Resetn,exe_Alu_Result,exe_rb,exe_wmem,exe_m2reg,exe_wreg,exe_rn,
			mem_Alu_Result,mem_rb,mem_wmem,mem_m2reg,mem_wreg,mem_rn);

	 //访存阶段
	 MEM_STAGE stage4 (mem_wmem, mem_Alu_Result[4:0], mem_rb, Clock, mem_mo,
			mem_wreg,mem_m2reg,mem_rn);

	 //锁存并传递Alu_Result，m2reg，wreg，rn，mo
	 MEM_WBreg MEM_WB(Clock,Resetn,mem_Alu_Result,mem_mo,mem_m2reg,mem_wreg,mem_rn,
			wb_Alu_Result,wb_mo,wb_m2reg,wb_wreg,wb_rn);
	 
	 //写回寄存器堆
	 WB_STAGE stage5 (wb_Alu_Result, wb_mo, wb_m2reg, wdi,wb_wreg,wb_rn);

    //stall信号产生
	 STALL STALL_UNIT(exe_wreg,exe_rn,rs,rt,regrt,mem_wreg,mem_rn,stall);
	 
endmodule
