module counter(
    input clk,
    input rst,

    output [3:0] out
);

    reg [3:0] cnt;

    always @(posedge clk) begin
	 /*
        cnt[3] <= cnt[3]&!cnt[2]&!rst | cnt[3]&!cnt[1]&!rst | cnt[2]&cnt[1]&cnt[0]&!rst;
        cnt[2] <= cnt[2]&!cnt[1]&!rst | !cnt[2]&cnt[1]&cnt[0]&!rst | !cnt[3]&cnt[2]&!cnt[0]&!rst;
        cnt[1] <= !cnt[1]&cnt[0]&!rst | !cnt[3]&cnt[1]&!cnt[0]&!rst | !cnt[2]&cnt[1]&!cnt[0]&!rst;
        cnt[0] <= !cnt[3]&!cnt[0]&!rst | !cnt[2]&!cnt[0]&!rst | !cnt[1]&!cnt[0]&!rst;
		  */
		  if(rst) begin
            cnt <= 4'b0;
          end

          else begin
            cnt[3] <= cnt[3] | cnt[2]&cnt[1]&cnt[0];
            cnt[2] <= cnt[2]&!cnt[1] | cnt[2]&!cnt[0] | !cnt[2]&cnt[1]&cnt[0];
            cnt[1] <= !cnt[1]&cnt[0] | cnt[1]&!cnt[0];
            cnt[0] <= !cnt[0];
          end
			
    end

    assign out = cnt;

endmodule

/*
Resource Usage: 5 LE


*/