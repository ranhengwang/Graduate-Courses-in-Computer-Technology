`timescale 1ns/1ns

module CPU_tb;
reg reset, sysclk;
reg IRQ;

initial begin
	reset <= 0;
	sysclk <= 0;
	reset <= 0;
	IRQ <=0;
end
	
initial fork
	#13 reset <= 1;
	forever #10 sysclk = ~sysclk;
join

	
PipelineCPU PipelineCPU_1(reset, sysclk, IRQ);

endmodule