module task03(
	output reg out,
	input [7:0] in,
	input clk,
	input reset
);

	localparam STATE_NONE	= 3'd0;
	// TODO: Adaugati starile necesare.

	reg [2:0] state;
	// TODO: Implementati logica tranzitiilor intre stari.
	// TODO: Implementati logica iesirii.
	always @(posedge clk) begin
		if (reset) begin
			state <= STATE_NONE;
		    out = 0;
		end 
		else begin
		  case(state)
		      3'b111: begin //Daca am gasit 3 litere din secventa o verific pe a 4-a
		          case(in)
		              8'b01000011: begin //Daca e C am gasit secventa
		                  state = STATE_NONE;
		                  out = 1;
		              end
		              default : state = STATE_NONE; //Daca nu, resetez
		          endcase
		      end

		      3'b011: begin //Daca am gasit 2 litere din secventa o verific pe a 3-a
		          case(in)
		              8'b01000100: state = 3'b111; //Daca e D am 3 litere gasite
		              default : state = STATE_NONE; //Daca nu, resetez
		          endcase
		      end
		      
		      3'b001: begin //Daca am gasit o litera din secventa o verific pe a 2-a
		          case(in)
		              8'b01000001: state = 3'b011; //Daca e A am 2 litere gasite
		              default : state = STATE_NONE; //Daca nu, resetez
		          endcase
		      end
		  
		      3'b000: begin //Daca am starea initiala verific prima litera
		          case(in)
		              8'b01000010: state = 3'b001; //Daca e B am o litera gasita
		              default : state = STATE_NONE; //Daca nu, resetez
		          endcase
		      end
		   endcase
		end
	end

endmodule
