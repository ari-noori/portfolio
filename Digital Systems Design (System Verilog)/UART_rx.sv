
module UART_rx(clk, rst_n, RX, clr_rdy, rx_data, rdy);

input clk, rst_n; //50MHz system clock & active low reset
input RX; //Serial data carrying command from host computer
input clr_rdy; //Asserted to knock down the rdy signal. 

output logic [7:0] rx_data; //Byte received
output logic rdy; //Asserted when a byte has been received

logic receiving; //This signal is high if the RX is recieving a signal
logic shift; //Controls the shifting behavior
logic [3:0] bit_cnt; //counter to 10
logic start; //starts the counting of the baud and counter
logic set_rdy; //Output of the SM that dictates if it is ready to recieve a signal
logic [8:0] rx_shft_reg; //the signal that is being fed
logic stableRX1; //double flop signal
logic stableRX2; //double flop signal

logic [11:0] baud_cnt; //our 2604 baud counter

/*
Enumerate Types
*/
typedef enum logic {IDLE,RECEIVING} state_t;
state_t state, nxt_state;


/*
Double flop RX for meta stability
*/
always_ff @(posedge clk, negedge rst_n) begin
	if(!rst_n)
		stableRX1 <= 1'b1;
	else
		stableRX1 <= RX;
end
always_ff @(posedge clk, negedge rst_n) begin
	if(!rst_n)
		stableRX2 <= 1'b1;
	else
		stableRX2 <= stableRX1;
end

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
	start = 0;
	receiving = 0;
	set_rdy = 0;

	/*
	IDLE state: Wait for the start bit from the RX signal, then initialize the start
	of our reception and transition into the RECEIVING state.
	*/
	case(state)
		IDLE : begin
			if(!stableRX2) 
			begin
				start = 1;
				nxt_state = RECEIVING;
			end	
		end	
		
		/*
		RECEIVING state: wait until we shift 10 bits, then send our state back
		to idle then report that we are ready for another byte.
		*/
		default : begin
			if(bit_cnt == 4'b1010) 
			begin
				nxt_state = IDLE;
				set_rdy = 1;
			end
			else begin
				nxt_state = RECEIVING;
				receiving = 1;
			end
		end	
	endcase	
end

/*
Shift Counter Block
*/
always_ff @(posedge clk) begin
	if(start)
		bit_cnt <= 4'b0000; //initialize to 0s
	else if (shift)
		bit_cnt <= bit_cnt + 1; //count when shift is asserted
	//else, keep the same value.	
end

/*
Counter until Baud
*/
always_ff @(posedge clk) begin
	if(start)
		baud_cnt <= 12'd1302; //Start at half the baud count
	else if(shift)
		baud_cnt <= 12'd2604; //If we are just shifting, do a full baud count
	else if(receiving)
		baud_cnt <= baud_cnt - 1; //count down until 0
end

assign shift = baud_cnt == 0; //shift when baud_cnt == 0;

/*
Shifter Block
*/
always_ff @(posedge clk, negedge rst_n) begin
	if(!rst_n)
		rx_shft_reg <= 9'b1_1111_1111; //clear when reset
	else if(shift)
		rx_shft_reg <= {stableRX2,rx_shft_reg[8:1]}; //shift in our stable RX bit
end

/*
SR Flip Flop
*/
always_ff @(posedge clk, negedge rst_n) begin
  if (!rst_n)
    rdy <= 1'b0;
  else if (start || clr_rdy) //clear the signal when starting a new byte or we are told to clear it
	rdy <= 1'b0;
  else if(set_rdy)
    rdy <= 1'b1; //output that we are ready when the SM is done
end

assign rx_data = rx_shft_reg[7:0]; //our final data is the byte from the shift register

endmodule