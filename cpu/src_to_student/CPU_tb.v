`timescale 1ns/1ns
/*
module baudrate_generator(sysclk, reset, brclk, brclk16);
input sysclk,reset;
output reg brclk, brclk16;
reg [11:0] cnt;
reg [7:0] cnt16;

always @(posedge sysclk or negedge reset)
begin
	if(~reset) begin
		cnt <= 12'd0;
		cnt16 <= 8'd0;
		brclk <= 1;
		brclk16 <= 1;
	end
	else begin
		if(cnt16 == 8'd162) begin
			cnt16 <= 8'd0;
			brclk16 <= ~brclk16;
			end
		else
			cnt16 <= cnt16 + 8'd1;
		if(cnt == 12'd2603) begin
			cnt <= 12'd0;
			brclk <= ~brclk;
			end
		else
			cnt <= cnt + 12'd1;
	end
end

endmodule

*/

module CPU_tb;
reg reset, sysclk;
reg IRQ;

initial begin
	reset <= 0;
	sysclk <= 0;
	reset <= 0;
   IRQ<=0;
end
	
initial fork
	#13 reset <= 1;
	forever #10 sysclk = ~sysclk;
join


	
CPU CPU_1(reset, sysclk, IRQ);

endmodule
