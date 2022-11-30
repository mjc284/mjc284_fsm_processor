module fsm_processor(
	input clk,
	output clk_ram,
	input miso,
	output mosi,
	output sck,
	output [5:0] ss
	//output [3:0] test
);
	
	wire [2:0] state;
	//assign test[2:0] = state;///////////////////
	//assign test[3] = clk;
	
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
	
	reg reg_sck = 1'b0;
	wire sck_ctrl = state[2]&!state[1]&state[0] | state[2]&state[1]&!state[0];
	always @(negedge clk)
		reg_sck <= sck_ctrl;
	assign sck = sck_ctrl&reg_sck&!clk;
	
	wire ss_clk = !state[2]&!state[1]&state[0]&!clk;
	reg [5:0] reg_ss = 6'b0;
	always @(posedge ss_clk) begin
		reg_ss <= {reg_ss[4:0], dout};
	end
	assign ss = reg_ss;
	
	wire cmp_ctrl = state[2]&state[1];
	wire reg_cmp_ctrl = !(state[2]&state[1]&state[0])&!clk;
	
	reg reg_miso = 1'b0;
	always @(posedge clk)
		reg_miso <= miso;
	wire pre_cmp = !(reg_miso^dout)&cmp&cmp_ctrl | !cmp_ctrl;
	reg reg_cmp = 1'b1;
	always @(posedge reg_cmp_ctrl)
		reg_cmp <=  pre_cmp;
	assign cmp = reg_cmp;
	
	assign mosi = !cmp_ctrl&dout;
	
	reg drshft = 1'b0;
	always @(negedge clk) begin
		drshft <= !(state[2]&!state[1]&!state[0]);
	end
	
	
	wire arclk_ctrl = !state[2]&state[1] | state[2]&state[1]&state[0];
	reg reg_arclk = 1'b0;
	always @(negedge clk)
		reg_arclk <= arclk_ctrl;
	wire arclk = arclk_ctrl&reg_arclk&!clk /* synthesis keep */;
	
	
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
Resource Usage: 42 LE
					 1 Block-ram
*/