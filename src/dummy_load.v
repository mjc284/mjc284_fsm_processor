module dummy_load(
	input load
);

reg dummy;
always @(posedge load)
	dummy <= !dummy;
	
endmodule