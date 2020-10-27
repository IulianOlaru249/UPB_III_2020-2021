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
	reg [7:0] sram_buff;
	reg [7:0] buff;
	wire [7:0] srom_buff;
	wire [7:0] sram_w_buff;
 
 
	localparam TRUE = 1'b1,
				  FALSE = 1'b0,
				  STATE_IDLE = 2'd0,
				  STATE_SRAM_READ_INIT = 2'b01,
				  STATE_SRAM_READ = 2'b10;
 
	// TODO: Implement transition from state to next state
	always @(clk) begin
		if (rst) begin
		  state <= STATE_IDLE; //just in case
		  next_state <= STATE_IDLE;
			//state <= 1'dx; 
		end else begin
		  state = next_state;
		end
	end
 
	// TODO: Implement the process of reading/writing data
	//                 how to change the state
	// Hint: Pay attention to the list of signals which trigger the block.
	//       When do we want it to run?
	always @(*) begin
		case (state)
			STATE_IDLE: begin
			     //verifica adresa la intrare
			     we = 0;
			     oe = 0; 
			     cs = 0;
			     
			     //daca reg de 4 biti e gol
			     if (address == 4'dz) begin
			         //ramane idle
			         next_state = STATE_IDLE;
			     end else begin
			         // incearca sa citesti
			         next_state = STATE_SRAM_READ_INIT;
			     end
			end
			STATE_SRAM_READ_INIT: begin
                //read sram
                next_state = STATE_SRAM_READ;
                we = 0;
			    oe = 1; 
			    cs = 1;
			end
			STATE_SRAM_READ: begin
                //vezi daca ai date in sram
                if (sram_buff != 8'dx) begin
                    //pune pe out buffer si du-te in idle
                    buff = sram_buff;
                    next_state = STATE_IDLE;
                //altfel citim din SROM
                end else begin
                     we = 1;
			         oe = 0;
			         cs = 1;
                
                     buff = srom_buff;
                     sram_buff = srom_buff;
                     next_state = STATE_IDLE;
                end
			end
		endcase
	end
 
	// TODO: Assign a value to data bus
	assign buff_w_sram = (we == 1) ? buff : 8'dz;
	assign data = buff;
 
	// TODO: Create an instance for each memory module
	// Hint: Pay attention to the address width
	
	task03 sram(clk, oe, cs, we, {3'b0, address}, sram_w_buff);
    task01 rom(address, srom_buff);
 
endmodule