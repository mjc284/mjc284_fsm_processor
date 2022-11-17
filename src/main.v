module main (
	//input clk,
	input cmp,
	output data,
	output data_clk,
	
	output [3:0] button,
	
	output led
	);
	

	wire clk;
	reg [24:0] cnt;
	always @(posedge clk)
		cnt <= cnt + 1'b1;

		wire testest;
	fsm_processor FSM_Processor(
		.clk(cnt[19]),
		.clk_ram(clk),
		.cmp(1'b0),
		.data(data),
		.data_clk(data_clk),
	);

	assign led = data;
	
endmodule

