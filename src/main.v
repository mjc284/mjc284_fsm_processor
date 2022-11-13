module main (
	input clk,

	input but_rst,
	input but1,
	input but2,
	input but3,
	
	output led0,
	output led1,
	output led2,
	output led3,

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

	wire [2:0] out;
	state_controller State_Controller(
		.in1(!but1),
		.in2(!but2),
		.clk(!but3),
		.out(out)
	);

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
	
endmodule