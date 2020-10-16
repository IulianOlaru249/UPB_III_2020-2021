module task01a(
	output out,
	input in0,
	input in1,
	input in2,
	input in3
);

	// TODO
	wire or1;
    wire nand1;
    wire or2;
    wire not1;
    wire and1;
    
    or OR1(or1, in1, in2);
    nand NAND1(nand1, in3, or1);
    or OR2(or2, in1, in3);
    not NOT1(not1, in0);
    and AND1(and1, not1, or2);
    and AND2(out, nand1, and1);

endmodule
