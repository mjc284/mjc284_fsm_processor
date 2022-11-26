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

	wire [5:0] ss;
	wire sck;
	wire mosi;
	wire miso;
		
	fsm_processor FSM_Processor(
		.clk(cnt[15]),
		.clk_ram(clk),
		.miso(miso),
		.mosi(mosi),
		.sck(sck),
		.ss(ss)
		//.test(button)
	);
	
	wire slv1 = !ss[1]&!ss[0];
	wire clk_slv1 = slv1&sck;
	reg [7:0] reg_slv1 = 8'b0;
	always @(posedge clk_slv1)
		reg_slv1 <= {reg_slv1[6:0], mosi};
	assign miso = reg_slv1[7];
	
	wire slv2 = !ss[1]&ss[0];
	wire clk_slv2 = slv2&sck;
	reg [7:0] reg_slv2 = 8'b0;
	always @(posedge clk_slv2)
		reg_slv2 <= {reg_slv2[6:0], mosi};
		
	assign led = reg_slv2[0];
	
endmodule

