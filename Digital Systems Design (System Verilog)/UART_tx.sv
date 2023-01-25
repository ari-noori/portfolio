
module UART_tx(clk, rst_n, TX, trmt, tx_data, tx_done);

input clk, rst_n; //50MHz system clock & active low reset
input trmt; //Asserted for 1 clock to initiate transmission
input [7:0] tx_data; //Byte to transmit

output logic tx_done; //Asserted when byte is done transmitting. Stays high till next byte transmitted.
output logic TX; //Serial data output

logic init, transmitting, set_done; //outputs of the SM
logic shift; //output of the baud count block that dictates shifting behavior
logic [3:0] bit_cnt; //output of the counter block

/*
Enumerate Types
*/
typedef enum logic {IDLE,TRANSMITTING} state_t;
state_t state, nxt_state;

/*
State Machine
*/

//FF for the state and next state
always_ff @(posedge clk or negedge rst_n) begin
  if (!rst_n)
    state <= IDLE;
  else
    state <= nxt_state;
end

always_comb begin
	//Default behavior
	nxt_state = state;
	init = 0;
	transmitting = 0;
	set_done = 1;

	case(state)
		//IDLE case when we are not transmitting anything. If we get the trmt signal
		//in this state, we will begin the process of transmitting by changing states
		IDLE : begin
			if(trmt) 
			begin
				init = 1;
				nxt_state = TRANSMITTING;
				set_done = 0;
			end	
		end	
		
		//TRANSMITTING state
		TRANSMITTING : begin
			if(bit_cnt == 4'b1010) //if we have shifted 10 total bits, then 
								   //we are done.
			begin
				nxt_state = IDLE; //return to idle
				set_done = 1;	  //record that the transmission is done
			end
			else begin
				nxt_state = TRANSMITTING; //if we havent shifted 10 bits, keep transmitting
				transmitting = 1; //output that a transmission is currently happening
				set_done = 0;
			end
		end	
	endcase	
end



/*
Shift Counter Block
*/

always_ff @(posedge clk) begin
	if(init)
		bit_cnt <= 4'b0000; //initialize to 0s
	else if (shift)
		bit_cnt <= bit_cnt + 1; //count when shift is asserted
	//else, keep the same value.	
end

/*
Counter until Baud
*/
logic [11:0] baud_cnt;

always_ff @(posedge clk) begin
	if(init || shift) //When we initialize or are shifting, we want to clear our count
		baud_cnt <= 12'b0000_0000_0000;
	else if(transmitting) //while transmitting, increment the count
		baud_cnt <= baud_cnt + 1;
end

assign shift = baud_cnt == 2604; //Check if we have gotten to our Baud count.

/*
Shifter Block
*/
logic [8:0] tx_shft_reg;

always_ff @(posedge clk, negedge rst_n) begin
	if(!rst_n)
		tx_shft_reg <= 9'b1_1111_1111; //clear with all 0s
	else if(init)
		tx_shft_reg <= {tx_data, 1'b0}; //manually shift in an extra 0 to our 8 bit tx_data (start bit)
	else if(shift)
		tx_shft_reg <= {1'b1, tx_shft_reg[8:1]}; //shift in a 1 each time shift is asserted
end

assign TX = tx_shft_reg[0]; //The serial data output. LSB shifted out.

/*
SR Flip Flop
*/
always_ff @(posedge clk, negedge rst_n) begin
  if (!rst_n)
    tx_done <= 1'b0; //reset with 0
  else if(set_done)
    tx_done <= 1'b1; //set with a 1 when the transmission has finished
  else if (init)
	tx_done <= 1'b0; //when initialized, reset with 0
end


endmodule