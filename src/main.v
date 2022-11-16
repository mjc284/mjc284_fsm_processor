module main (
	input clk_in,

	input but_rst,
	input but1,
	input but2,
	input but3,
	
	output led0,
	output led1,
	output led2,
	output led3,
	
	input [16:0] gpio_L_1,

	output LED_A,
	output LED_B,
	output LED_C,
	output LED_D,
	output LED_E,
	output LED_F,
	output LED_G,
	output LED_DP,
	output SEL0,
	output SEL1,
	output SEL2,
	output SEL3,
	output SEL4,
	output SEL5
	);

	wire clk = !but3;
	wire data = !but2;
	
	wire [2:0] state;
	
	reg [1:0] opcode;
	wire op_clk = !state[2]&!state[1]&!state[0]&!clk;
	always @(posedge op_clk) begin
		opcode <= {opcode[0], data};
	end

	wire rst;	
	reg state_controller_clk_reg = 1'b0;
	always @(negedge clk)
		state_controller_clk_reg <= rst;
	wire state_cntroller_clk = state_controller_clk_reg&clk;
	state_controller State_Controller(
		.in1(opcode[0]),
		.in2(opcode[1]),
		.clk(state_cntroller_clk),
		.out(state)
	);

	wire [3:0] count;
	counter Counter(
    .clk(clk),
    .rst(rst),

    .out(count)
	);
	
	counter_rst Counter_RST(
		.state(state),
    	.opcode(opcode),
    	.cmp(!but1),
    	.count(count),
 		.out(rst)
	);
	
	wire data_clk = !state[2]&!state[1]&state[0]&!clk;
	reg [15:0] reg_data = 16'b0;
	always @(posedge data_clk) begin
		reg_data <= {reg_data[14:0], data};
	end
	
	reg drclk = 1'b0;
	always @(negedge clk) begin
		drclk <= state[2]&!state[1]&!state[0];
	end
	
	wire arclk = !state[2]&state[1]&!state[0]&!clk | !state[2]&state[1]&state[0]&!clk;
	
	assign led0 = op_clk;
	assign led1 = data_clk;
	assign led2 = arclk;
	assign led3 = drclk;
	assign LED_A = reg_data[15];
	
/*
	seven_seg SEVEN_SEG (
		.clk(clk),
		.digit0(5'd20),
		.digit1(5'd20),
		.digit2(out),
		.digit3(5'd20),
		.digit4(5'd20),
		.digit5(5'd20),
		
		.LED_A(LED_A),
		.LED_B(LED_B),
		.LED_C(LED_C),
		.LED_D(LED_D),
		.LED_E(LED_E),
		.LED_F(LED_F),
		.LED_G(LED_G),
		.LED_DP(LED_DP),
		.SEL0(SEL0),
		.SEL1(SEL1),
		.SEL2(SEL2),
		.SEL3(SEL3),
		.SEL4(SEL4),
		.SEL5(SEL5)
	);
*/
	
endmodule

/*
Resource Usage: 26 LE (all other stuff) + 16 LE (reg_dout) = 42 LE

*/