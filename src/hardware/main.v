module main (
	//input clk,
	output mosi_bus,
	input miso_bus_1,
	output sck_bus,
	output ss_bus_1,
	
	//output [3:0] button,
	
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
		.clk(cnt[15]),
		.clk_ram(clk),
		.miso(miso),
		.mosi(mosi),
		.sck(sck),
		.ss(ss)
		//.test(button)
	);
	
	wire ss0 = !ss[3]&!ss[2]&!ss[1]&!ss[0];
	wire ss1 = !ss[3]&!ss[2]&!ss[1]& ss[0];
	wire ss2 = !ss[3]&!ss[2]& ss[1]&!ss[0];
	wire ss3 = !ss[3]&!ss[2]& ss[1]& ss[0];
	
	wire miso0;
	wire miso1;
	wire miso2;
	wire miso3;
	assign miso = ss0&miso0 | ss1&miso1 | ss2&miso2 | ss3&miso3;
	
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
	
	assign mosi_bus = mosi;
	assign miso2 = miso_bus_1;
	assign sck_bus = sck;
	assign ss_bus_1 = ss2;
	
endmodule

