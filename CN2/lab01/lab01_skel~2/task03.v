module task03 (
        input  wire       clk,     // clock
        input  wire       oe,      // output enable, active high
        input  wire       cs,      // chip select, active high
        input  wire       we,      // write enable: 0 = read, 1 = write
        input  wire [6:0] address, // adrese pentru 128 de intrari
        inout  wire [7:0] data     // magistrala de date de 8 biti bidirectionala
    );
	
	   // TODO: Redesign SRAM module using a single data bus
    reg [7:0] memory[0:127];
    reg [7:0] buffer;
    
    always @(clk) begin
        if (cs) begin
            if (we) begin
                memory[address] = data;
            end else begin
                buffer = memory[address];
            end
        end
    end
    assign data = ((oe == 1) && (we == 0)) ? buffer : 8'bz;
endmodule