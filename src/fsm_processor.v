module fsm_processor(
	input clk,
	output clk_ram,
	input cmp,
	output data,
	output data_clk,
	output [3:0] test,
	output test2
);
	
	wire [2:0] state;
	
	reg [1:0] opcode;
	wire op_clk = !state[2]&!state[1]&!state[0]&!clk /* synthesis keep */;
	wire dout;
	always @(posedge op_clk) begin
		opcode <= {opcode[0], dout};
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
    	.cmp(cmp),
    	.count(count),
 		.out(rst)
	);
	
	assign data_clk = !state[2]&!state[1]&state[0]&!clk;
	/*
	reg [15:0] reg_data = 16'b0;
	always @(posedge data_clk) begin
		reg_data <= {reg_data[14:0], dout};
	end
	*/
	
	reg drshft = 1'b0;
	always @(negedge clk) begin
		drshft <= !(state[2]&!state[1]&!state[0]);
	end
	
	reg reg_arclk = 1'b0;
	always @(negedge clk)
		reg_arclk <= !state[2]&state[1];
		
	wire arclk = reg_arclk & !clk /* synthesis keep */;
	
	reg init = 1'b0; 
	always @(posedge drshft)
		init <= 1'b1;
		
	dummy_load Load(.load(arclk));
	
	wire arshft = !state[2]&state[1]&state[0] | !init;
	
	wire dummy;
	dff DFF(
		.d(dummy), 
		.clk(arclk),

		.clrn(1'b1), 
		.prn(1'b1), 
		.q(dummy)
	);
	
	wire ardin = dout & init;

	//wire clk_ram;
	ram Ram(
		.arclk(arclk),
		.ardin(ardin),
		.arshft(arshft),
		.drclk(clk),
		.drdin(1'b0),
		.drshft(drshft),
		.erase(),
		.oscena(1'b1),
		.program(1'b0),
		.busy(),
		.drdout(dout),
		.osc(clk_ram),
		.rtpbusy()
	);
	
	assign data = dout;
endmodule

/*
Resource Usage: 26 LE (all other stuff) + 16 LE (reg_dout) = 42 LE
					 1 Block-ram
*/