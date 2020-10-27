module task04 (
        input  wire clk,        // clock
        input  wire rst,        // reset
        input  wire [3:0] address,
        output wire [7:0] data
    );

    reg [1:0]  state;
    reg [1:0]  next_state;
   
    /* Memory params */
    reg we, oe, cs;
   
    // TODO: Define other required params
    reg [7:0] sram_buffer;
    reg [7:0] data_buffer;
    wire [7:0] rom_buffer;
    wire [7:0] sram_wire;

    localparam TRUE = 1'b1,
                  FALSE = 1'b0,
                  STATE_IDLE = 2'd0,
                  STATE_SRAM_READ_INIT = 2'b01,
                  STATE_SRAM_READ = 2'b10;

    // TODO: Implement transition from state to next state
    always @(clk) begin
        if (rst) begin
            state <= STATE_IDLE;
            next_state <= STATE_IDLE;
        end else begin
             state <= next_state;
        end
    end

    // TODO: Implement the process of reading/writing data
    //                 how to change the state
    // Hint: Pay attention to the list of signals which trigger the block.
    //       When do we want it to run?
    always @(*) begin
        case (state)
            STATE_IDLE: begin
                oe = 0;
                cs = 0;
                we = 0;
               
                // impedanta marita
                if (address == 4'dz) begin
                    next_state = STATE_IDLE;
                end else begin
                    next_state = STATE_SRAM_READ_INIT;
                end
            end
            STATE_SRAM_READ_INIT: begin
                next_state = STATE_SRAM_READ;
                oe = 1;
                cs = 1;
                we = 0;
            end
            STATE_SRAM_READ: begin               
                // fir neconectat
                if (sram_buffer != 8'dx) begin
                    data_buffer = sram_buffer;
                    next_state = STATE_IDLE;
                end else begin
                    data_buffer = rom_buffer;
                    sram_buffer = data_buffer;
                    oe = 0;
                    cs = 1;
                    we = 1;
                    next_state = STATE_IDLE;
                end
            end
        endcase
    end
   
    // TODO: Assign a value to data bus
    assign sram_wire = (we == 1) ? data_buffer: 8'dz;
    assign data = data_buffer;
   
    // TODO: Create an instance for each memory module
    // Hint: Pay attention to the address width
    task03 sram(clk, oe, cs, we, {4'b0, address}, sram_wire);
    task01 rom(address, rom_buffer);
endmodule