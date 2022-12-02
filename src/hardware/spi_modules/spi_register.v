module spi_register (
	input mosi,
	input ss,
	input sck,
	output miso
	);

	reg [7:0] shift_dout = 8'b10000000;
	reg [7:0] shift_din = 8'b0;
	reg [7:0] latch_dout = 8'b0;
	
	always @(posedge sck) begin
		if(!ss) begin
			shift_din <= {shift_din[6:0], mosi};
			shift_dout <= {shift_dout[0], shift_dout[7:1]};
		end
		else begin
			shift_din <= 8'b0;
			shift_dout <= 8'b10000000;
		end
	end
	
	wire en = |shift_din;
	wire clr = &shift_din;
	wire trig = shift_dout[7]&!sck;
	
	always @(posedge trig) begin
		if(en)
			if(clr)
				latch_dout <= 8'b0;
			else
				latch_dout <= shift_din;
	end
	
	assign miso = |(latch_dout&shift_dout);

endmodule
