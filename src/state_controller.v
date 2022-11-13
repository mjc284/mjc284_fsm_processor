module state_controller(
    input in1,
    input in2,
    input clk,

    output [2:0] out
);

    reg [2:0] states = 3'b0;

    always @(posedge clk) begin
        states[2] <= !states[2]&states[1];
        states[1] <= !states[2]&!states[1]&states[0] | !states[2]&!states[1]&in1&in2;
        states[0] <= !states[2]&!states[1]&!states[0];
    end

    assign out = states;

endmodule