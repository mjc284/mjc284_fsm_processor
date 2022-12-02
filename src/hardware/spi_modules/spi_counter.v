module spi_counter (
	input mosi,
	output miso,
	input ss,
	output test,
	
	input sck
	);
	
	wire clk = !ss&sck;
	
	reg [7:0] ctrl = 8'b0;
	reg [7:0] cnt = 8'b0;
	reg [7:0] shift_dout = 8'b10000000;
	
	always @(posedge clk) begin //00001111 <- 11110000
		ctrl <= {ctrl[6:0], mosi};
		shift_dout <= {shift_dout[0], shift_dout[7:1]};
	end
	
	assign test = |cnt;
	
	wire rst = ctrl[0];
	wire trig = ctrl[2];
		
	always @(posedge trig) begin
		if(rst)
			cnt <= 8'b0;
		else
			cnt <= cnt + 1'b1;
	end
	
	assign miso = |(shift_dout&cnt);
	
	
endmodule