module spi_output (
	input mosi,
	input ss,
	input sck,
	
	output [7:0] dout
	);

	reg [7:0] shift_dout = 8'b0;
	reg [7:0] latch_dout = 8'b0;
	wire clk = !ss&sck;
	
	always @(posedge clk)
		shift_dout <= {shift_dout[6:0], mosi};
	
	always @(posedge ss)
		latch_dout <= shift_dout;
	
	assign dout = latch_dout;

endmodule
