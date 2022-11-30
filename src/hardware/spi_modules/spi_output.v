module spi_output (
	input mosi,
	input ss,
	input sck,
	output miso,
	
	input [7:0] din,
	output [7:0] dout
	);

	reg [7:0] shift_dout = 8'b1;
	reg [7:0] shift_din = 8'b0;
	reg [7:0] latch_dout = 8'b0;
	reg [7:0] latch_din = 8'b0;
	wire clk = !ss&sck;
	
	always @(posedge clk) begin
		shift_din <= {shift_din[6:0], mosi};
		shift_dout <= {shift_dout[6:0], shift_dout[7]};
	end
	
	always @(posedge ss) 
		latch_dout <= shift_din;
	
	always @(negedge ss)
		latch_din <= din;
	
	assign dout = latch_dout;
	assign miso = |(latch_din&shift_dout);

endmodule
