module counter_rst(
    input [2:0] state,
    input [1:0] opcode,
    input cmp,
    input [3:0] count,
    output out
);

  wire jump = opcode[1]&!opcode[0]&cmp;
  wire [7:0] comb_in = {count[0], count[1], count[2], count[3], jump, state[0], state[1], state[2]};
  /*
  assign out = (!comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]&!comb_in[3]&!comb_in[2]&!comb_in[1]& comb_in[0]) |
               (!comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]&!comb_in[3]&!comb_in[2]& comb_in[1]&!comb_in[0]) |
               (!comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]&!comb_in[3]& comb_in[2]&!comb_in[1]& comb_in[0]) |
               (!comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]& comb_in[3]&!comb_in[2]&!comb_in[1]& comb_in[0]) |
               (!comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]& comb_in[3]& comb_in[2]&!comb_in[1]& comb_in[0]) |
               (!comb_in[7]&!comb_in[6]&!comb_in[5]& comb_in[4]&!comb_in[3]& comb_in[2]& comb_in[1]&!comb_in[0]) |
               (!comb_in[7]&!comb_in[6]&!comb_in[5]& comb_in[4]& comb_in[3]& comb_in[2]& comb_in[1]&!comb_in[0]) |
               ( comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]&!comb_in[3]&!comb_in[2]&!comb_in[1]&!comb_in[0]) |
               ( comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]& comb_in[3]&!comb_in[2]&!comb_in[1]&!comb_in[0]) |
               ( comb_in[7]&!comb_in[6]&!comb_in[5]&!comb_in[4]& comb_in[3]&!comb_in[2]& comb_in[1]&!comb_in[0]) |
               ( comb_in[7]&!comb_in[6]& comb_in[5]& comb_in[4]&!comb_in[3]& comb_in[2]&!comb_in[1]&!comb_in[0]) |
               ( comb_in[7]&!comb_in[6]& comb_in[5]& comb_in[4]& comb_in[3]& comb_in[2]&!comb_in[1]&!comb_in[0]);
  */
  
  assign out = (comb_in == 8'h01) ? 1'b1:
               (comb_in == 8'h02) ? 1'b1:
               (comb_in == 8'h05) ? 1'b1:
               (comb_in == 8'h09) ? 1'b1:
               (comb_in == 8'h0D) ? 1'b1:
               (comb_in == 8'h16) ? 1'b1:
               (comb_in == 8'h1E) ? 1'b1:
               (comb_in == 8'h80) ? 1'b1:
               (comb_in == 8'h88) ? 1'b1:
               (comb_in == 8'h8A) ? 1'b1:
               (comb_in == 8'hB4) ? 1'b1:
               (comb_in == 8'hBC) ? 1'b1:
               1'b0;

endmodule

/*
Resource Usage: 6 LE


*/