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
	 input Clock, Resetn;//�����ʱ�Ӽ���λ�ź�
	 output [31:0] PC, if_Inst, exe_Alu_Result,mem_mo; //�����ǰPCֵ��ָ�alu��������DMȡ��������
	 output stall; //��ͣ�ź����
	 output exe_wreg,mem_wreg;   //д�Ĵ������ź�
	 output [4:0] exe_rn, mem_rn;  //Ҫд�ļĴ���
	 output exe_m2reg,mem_m2reg;  //д�Ĵ�����������Դ
	 output [4:0] rs,rt;  //Դ�Ĵ���
	 output regrt;  //�Ƿ���rt
	 
	 wire [31:0] mem_Alu_Result, wb_Alu_Result;//ALU���
	 wire [1:0] pcsource;  //PCѡ����
	 wire [4:0] rs,rt;  //Դ�Ĵ�������
	 wire regrt;  //�Ƿ���rt
	 wire stall;  //��ͣ�ź�
	 
	 wire [31:0] bpc, jpc, if_pc4, id_pc4, id_Inst,if_Inst; //pc��ָ��
	 
	 wire [31:0] wdi, id_ra, exe_ra, id_rb, exe_rb, mem_rb, id_imm, exe_imm;
	 wire id_m2reg, exe_m2reg,mem_m2reg,wb_m2reg;//д������ѡ����
	 
	 wire id_wmem, exe_wmem, id_aluimm, exe_aluimm, id_shift, exe_shift, z;
	 
	 wire [2:0] id_aluc, exe_aluc; //alu������
	 
	 wire id_wreg, exe_wreg, mem_wreg, wb_wreg; //д�Ĵ������źŴ���
	 wire [4:0] id_rn, exe_rn, mem_rn, wb_rn; //rn����
	 
	 wire [31:0] mem_mo, wb_mo;//DMȡ��������

	 wire id_wreg_org,id_wmem_org; //����׶ε�д�Ĵ����Ѻ�дDM���ź�

	 //ȡָ�׶�
	 IF_STAGE stage1 (Clock, Resetn, pcsource, bpc, jpc, if_pc4, if_Inst, PC,stall);
	 
	 //���沢����pc4��Inst
	 IF_IDreg  IF_ID (Clock,Resetn,if_pc4,if_Inst,id_pc4,id_Inst);
	 
	 //����׶�
	 ID_STAGE stage2 (id_pc4, id_Inst, wdi, Clock, Resetn, bpc, jpc, pcsource,
				   id_m2reg, id_wmem_org, id_aluc, id_aluimm, id_ra, id_rb, id_imm, 
					id_shift, z,id_wreg_org,id_rn,rs, rt, regrt);	
	
	//����stall���wreg��wmem�ź�(�������źŸ���)
	assign id_wreg = ~stall & id_wreg_org;		
	assign id_wmem = ~stall & id_wmem_org;

	 //���沢����	m2reg, wmem, aluc, aluimm, ra, rb, imm, shift��wreg��rn
	 ID_EXEreg ID_EXE (Clock,Resetn,id_m2reg,id_wmem,id_aluc,id_aluimm,
			id_ra,id_rb,id_imm,id_shift,id_wreg,id_rn,exe_m2reg,exe_wmem,
			exe_aluc,exe_aluimm,exe_ra,exe_rb,exe_imm,exe_shift,exe_wreg,exe_rn);

	 //ִ�н׶�
	 EXE_STAGE stage3 (exe_aluc, exe_aluimm, exe_ra, exe_rb, exe_imm, exe_shift, exe_Alu_Result,
			z,exe_m2reg,exe_wmem,exe_wreg,exe_rn);
	 
	 //���沢����Alu_Result, rb��wmem��m2reg��wreg��rn
	 EXE_MEMreg EXE_MEM(Clock,Resetn,exe_Alu_Result,exe_rb,exe_wmem,exe_m2reg,exe_wreg,exe_rn,
			mem_Alu_Result,mem_rb,mem_wmem,mem_m2reg,mem_wreg,mem_rn);

	 //�ô�׶�
	 MEM_STAGE stage4 (mem_wmem, mem_Alu_Result[4:0], mem_rb, Clock, mem_mo,
			mem_wreg,mem_m2reg,mem_rn);

	 //���沢����Alu_Result��m2reg��wreg��rn��mo
	 MEM_WBreg MEM_WB(Clock,Resetn,mem_Alu_Result,mem_mo,mem_m2reg,mem_wreg,mem_rn,
			wb_Alu_Result,wb_mo,wb_m2reg,wb_wreg,wb_rn);
	 
	 //д�ؼĴ�����
	 WB_STAGE stage5 (wb_Alu_Result, wb_mo, wb_m2reg, wdi,wb_wreg,wb_rn);

    //stall�źŲ���
	 STALL STALL_UNIT(exe_wreg,exe_rn,rs,rt,regrt,mem_wreg,mem_rn,stall);
	 
endmodule
