module seven_seg (
	input clk,
	input [4:0] digit0,
	input [4:0] digit1,
	input [4:0] digit2,
	input [4:0] digit3,
	input [4:0] digit4,
	input [4:0] digit5,
	
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
	
	reg [2:0] counter;
	reg [31:0] master_counter;
	reg [5:0] select;
	reg [7:0] led;
	reg [7:0] led_data [0:20];
	
	initial begin
		counter <= 3'b0;
		select <= 6'b1;
		led <= 8'b11111111;
		master_counter <= 32'b0;
		
		led_data[0] = 8'b11000000;
		led_data[1] = 8'b11111001;
		led_data[2] = 8'b10100100;
		led_data[3] = 8'b10110000;
		led_data[4] = 8'b10011001;
		led_data[5] = 8'b10010010;
		led_data[6] = 8'b10000010;
		led_data[7] = 8'b11111000;
		led_data[8] = 8'b10000000;
		led_data[9] = 8'b10010000;
		led_data[10] = 8'b01000000;
		led_data[11] = 8'b01111001;
		led_data[12] = 8'b00100100;
		led_data[13] = 8'b00110000;
		led_data[14] = 8'b00011001;
		led_data[15] = 8'b00010010;
		led_data[16] = 8'b00000010;
		led_data[17] = 8'b01111000;
		led_data[18] = 8'b00000000;
		led_data[19] = 8'b00010000;
		led_data[20] = 8'b11111111;
	end
	
	always @ (posedge clk) begin
		master_counter <= master_counter + 1'b1;
	end
	
	always @ (posedge master_counter[15]) begin
		if (counter < 5) begin
			counter <= counter + 1'b1;
			select <= select<<1;
		end
		else begin
			counter <= 3'b0;
			select <= 6'b1;
		end
	end
	
	assign SEL0 = ~select[0];
	assign SEL1 = ~select[1];
	assign SEL2 = ~select[2];
	assign SEL3 = ~select[3];
	assign SEL4 = ~select[4];
	assign SEL5 = ~select[5];
	
	
	always @ (posedge clk) begin
	
		if (counter == 3'd0) begin
			led <= led_data[digit0];
		end
		else if (counter == 3'd1) begin
			led <= led_data[digit1];
		end
		else if (counter == 3'd2) begin
			led <= led_data[digit2];
		end
		else if (counter == 3'd3) begin
			led <= led_data[digit3];
		end
		else if (counter == 3'd4) begin
			led <= led_data[digit4];
		end
		else if (counter == 3'd5) begin
			led <= led_data[digit5];
		end
	end

	
	assign LED_A = led[0];
	assign LED_B = led[1];
	assign LED_C = led[2];
	assign LED_D = led[3];
	assign LED_E = led[4];
	assign LED_F = led[5];
	assign LED_G = led[6];
	assign LED_DP = led[7];
	
endmodule

/*
Description: 6 X Seven segment display driver. Accepts binary numbers 0 to 20 for each of the 6 digits.

		Input 0 to 9: Numbers 0 to 9.
		Input 10 to 19: Numbers 0 to 9 with decimal.
		Input 20: Digit off.

Resources Usage: ~60 LE
*/