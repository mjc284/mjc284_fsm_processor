module spi_counter (
	input mosi;
	output miso;
	input ss1;
	input ss2;
	input sck;
	);
	
	wire clk1 = !ss1&sck;
	wire clk2 = !ss2&sck;
	
	reg [7:0] ctrl = 8'b0;
	reg [7:0] cmp = 8'b0;
	
	always @(posedge clk1)
		ctrl <= {ctrl[6:0], mosi};
		
	always @(posedge clk2)
		cmp <= {cmp[6:0], mosi};
		
	wire inc = ctrl[1];
	wire rst = ctrl[0]&ctrl[1];
	reg [7:0] counter = 8'b0;
	
	/*
	implement counter with reset
	*/
	
	assign miso = cmp&counter;
	
	
endmodule