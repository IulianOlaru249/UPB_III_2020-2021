module task02(
	output reg serial_out,
	input serial_in,
	input clk,
	input reset
);

	// TODO
	wire count = 0;
	 reg [7:0] memory;
	 always @ (posedge clk) begin
	   if (reset == 1'b1) begin
	   memory = 8'b00000000;
	       memory[0] = serial_in;
	   end
	   else begin
	       //if (count == 8) begin
	           memory = memory << 1;
	       //end
	   end
	   
	   serial_out = memory[0];
	   
	  end

endmodule
