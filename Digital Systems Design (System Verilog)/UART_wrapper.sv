
module UART_wrapper(
input clr_cmd_rdy,			//allows consumer to clear
input RX,					//data we are recieving
input [7:0] resp,			//tx_data for our UART
input clk, rst_n,			//system clk and low-reset
input trmt,					//prompts our UART block to transmit
output logic cmd_rdy,		//lets us know when the command is ready
output logic [15:0] cmd,	//our actual data that we are sending out
output logic tx_done,		//high when we are done transmitting
output logic TX				//data we are transmitting
);

/*
Instantiate intermediate signals coming from/to the UART Block
*/
logic rx_rdy; //output from UART block to dictate a ready reciever
logic [7:0] rx_data; //Data recieved, output from the UART block

/*
Instantiate intermediate signals coming from/to the State Machine
*/
logic byteSelect; //Select bit for the high/low byte, output from the SM
logic clr_rx_rdy; //output from SM to clear the reciever ready
logic set_cmd_rdy; // Goes to SR flop to dictate when ready

logic [7:0] selectedByte; //The byte that is currently selected (high or low byte)

/*
Instantiate our UART
*/
UART iUART(.clk(clk),.rst_n(rst_n),.RX(RX), .TX(TX), .rx_rdy(rx_rdy), 
		   .clr_rx_rdy(clr_rx_rdy), .rx_data(rx_data), .trmt(trmt), .tx_data(resp), 
		   .tx_done(tx_done));



/*
Define Enumerated Types
*/
typedef enum logic {BYTEONE, BYTETWO} state_t;
state_t state, nxt_state;

/*
Flip Flop logic for the next state
*/
always_ff @(posedge clk or negedge rst_n) begin
  if (!rst_n)
    state <= BYTEONE;
  else
    state <= nxt_state;
end

/*
State Machine
*/
always_comb begin
	/*
	Set default behavior
	*/
	clr_rx_rdy = 0;
	byteSelect = 0;
	set_cmd_rdy = 0;
	nxt_state = state;
	
	case(state)
		
		/*
		This state recieves the first byte and waits for it to be 
		recieved in its entirety before transitioning to the next byte
		of our 16 bit signal. A signal can pop up at any time, 
		we just need to wait until rx_ready is asserted.
		*/
		BYTEONE : begin
			if(rx_rdy) begin
				byteSelect = 1;
				clr_rx_rdy = 1;
				nxt_state = BYTETWO;
			end
		end		
			
			
		/*
		This state recieves the second byte and waits for it to be 
		recieved in its entirety before transitioning back to the first byte
		of our 16 bit signal.
		*/	
		BYTETWO : begin
			if(rx_rdy) begin
				set_cmd_rdy = 1;
				clr_rx_rdy = 1;
				nxt_state = BYTEONE;
			end

		end
	endcase
end

/*
Mux and Flip Flop: Make the selection of our byte
*/
always_ff @(posedge clk, negedge rst_n) begin
	if(!rst_n)
		selectedByte <= 8'b0000_0000;
	else if (byteSelect) //selects rx_data when high
		selectedByte <= rx_data;
	//else holds its value	
end

//Concatenate our 2 byte command together
assign cmd = {selectedByte, rx_data};

/*
SR FF for the cmd_ready signal
*/
always_ff @(posedge clk, negedge rst_n) begin
	if(!rst_n)
		cmd_rdy <= 1'b0; //reset
	else if (set_cmd_rdy)
		cmd_rdy <= 1'b1; //set
	else if(clr_cmd_rdy | rx_rdy)
		cmd_rdy <= 1'b0; //reset	
end
endmodule