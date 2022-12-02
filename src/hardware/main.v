module main (
	output mosi_bus,
	input miso_bus_1,
	output sck_bus,
	output ss_bus_1,
	
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
		.clk(cnt[0]),
		.clk_ram(clk),
		.miso(miso),
		.mosi(mosi),
		.sck(sck),
		.ss(ss)
		//.test(button)
	);
	
	wire ss0 = !(!ss[3]&!ss[2]&!ss[1]&!ss[0]);
	wire ss1 = !(!ss[3]&!ss[2]&!ss[1]& ss[0]);
	wire ss2 = !(!ss[3]&!ss[2]& ss[1]&!ss[0]);
	wire ss3 = !(!ss[3]&!ss[2]& ss[1]& ss[0]);
	wire ss4 = !(!ss[3]& ss[2]&!ss[1]&!ss[0]);
	wire ss5 = !(!ss[3]& ss[2]&!ss[1]& ss[0]);
	
	wire miso0;
	wire miso1;
	wire miso2;
	wire miso3;
	wire miso4;
	wire miso5;
	assign miso = !ss0&miso0 | !ss1&miso1 | !ss2&miso2 | !ss3&miso3 | !ss4&miso4 | !ss5&miso5;
	
	spi_counter SPI_Counter_1(
		.mosi(mosi),
		.miso(miso0),
		.sck(sck),
		.ss(ss0)
	);
	
	spi_counter SPI_Counter_2(
		.mosi(mosi),
		.miso(miso1),
		.sck(sck),
		.ss(ss1)
	);

	spi_register SPI_Register_1(
		.mosi(mosi),
		.miso(miso2),
		.sck(sck),
		.ss(ss2)
	);

	spi_register SPI_Register_2(
		.mosi(mosi),
		.miso(miso3),
		.sck(sck),
		.ss(ss3)
	);
	
	wire [7:0] out;
	spi_output SPI_Output(
		.mosi(mosi),
		.miso(miso4),
		.sck(sck),
		.ss(ss4),
		
		.dout(out)
	);
	assign led = out[0];
	
	assign mosi_bus = mosi;
	assign miso5 = miso_bus_1;
	assign sck_bus = sck;
	assign ss_bus_1 = ss5;
	
endmodule

