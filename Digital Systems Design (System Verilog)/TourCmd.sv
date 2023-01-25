
module TourCmd(clk,rst_n,start_tour,move,mv_indx,
               cmd_UART,cmd,cmd_rdy_UART,cmd_rdy,
               clr_cmd_rdy,send_resp,resp);

input clk,rst_n;			   // 50MHz clock and asynch active low reset
input start_tour;			   // from done signal from TourLogic
input [7:0] move;			   // encoded 1-hot move to perform
output reg [4:0] mv_indx;	// "address" to access next move
input [15:0] cmd_UART;	   // cmd from UART_wrapper
input cmd_rdy_UART;		   // cmd_rdy from UART_wrapper
output [15:0] cmd;		   // multiplexed cmd to cmd_proc
output cmd_rdy;			   // cmd_rdy signal to cmd_proc
input clr_cmd_rdy;		   // from cmd_proc (goes to UART_wrapper too)
input send_resp;			   // lets us know cmd_proc is done with command
output [7:0] resp;		   // either 0xA5 (done) or 0x5A (in progress)

//////////////////////////////
// Intermediate/SM signals //
////////////////////////////

/*
SM Outputs
*/
logic sel_UART_SM;   // selects between UART and the SM
logic inc_mv_index;  // increments the mv index
logic clr_mv_index;  // decrements the mv index
logic cmd_rdy_SM;    // cmd_rdy signal to be selected between cmd_rdy_UART and this
logic update_cmd_x;  // SM control signal to update the horizontal component
logic update_cmd_y;  // SM control signal to update the vertical component
logic set_cmd_rdy_SM;
logic [15:0] cmd_SM, cmd_y, cmd_x; //cmd originating from the SM

/////////////////////////
// Move Index Counter //
///////////////////////
always_ff @(posedge clk, negedge rst_n)
   if (!rst_n)
      mv_indx = 5'b0000;
   else if (clr_mv_index)
      mv_indx = 5'b0000;
   else if (inc_mv_index)
      mv_indx = mv_indx + 1;
   // else hold value   

/*
The following assign statements decompose the move according to the one-hot encoded
move from tourLogic. 

Decompose move:
North (+Y) is 00
West  (-X) is 3F
South (-Y) is 7F
East  (+X) is BF

The horizontal component will always play with fanfare.
*/
always_comb begin
   cmd_SM = 16'h0000;

   if(update_cmd_y) begin
      cmd_SM = (move[0] || move[1]) ? 16'h2002 : // Move up by 2
               (move[4] || move[5]) ? 16'h27F2 : // Move down by 2
               (move[2] || move[7]) ? 16'h2001 : // Move up by 1
               (move[3] || move[6]) ? 16'h27F1 : // Move down by 1
               16'h0000; // No move
      end

   else if(update_cmd_x) begin
      cmd_SM = (move[0] || move[4]) ? 16'h33F1 : // Move left by 1
               (move[1] || move[5]) ? 16'h3BF1 : // Move right by 1
               (move[2] || move[3]) ? 16'h33F2 : // Move left by 2
               (move[6] || move[7]) ? 16'h3BF2 : // Move right by 2
               16'h0000; // No move
      end
end

/*
Register for asserting cmd_rdy_SM.

When the state machine tells us to form the horizontal/vertical components
of the move, we assert that the command is ready basec on the set_cmd_rdy_sm 
signal. The signal is cleared with clr_cmd_rdy.
*/

always_ff @(posedge clk, negedge rst_n) begin
   if (!rst_n)
      cmd_rdy_SM <= 1'b0;
   else if (clr_cmd_rdy)
      cmd_rdy_SM <= 1'b0;
   else if (set_cmd_rdy_SM)
      cmd_rdy_SM <= 1'b1;  
end


/////////////////////////
// MUXs for cmd(_rdy) //
///////////////////////
assign cmd = sel_UART_SM ? cmd_SM : cmd_UART;               // Select between UART and SM
assign cmd_rdy = sel_UART_SM ? cmd_rdy_SM : cmd_rdy_UART;   // Select between UART and SM

/////////////////////
// Response Logic //
///////////////////
// If we are on the last move, we select A5. For current moves,
// we select 5A. 
// If cmd originated from UART, the resp should always be A5.
assign resp = (mv_indx == 5'd23 || !sel_UART_SM) ? 8'hA5 : 8'h5A;

///////////////////////////////////////
// Create enumerated type for state //
/////////////////////////////////////
typedef enum logic [2:0] {IDLE, FORM_VERT, FORM_HORZ, WAIT_VERT, WAIT_HORZ} state_t;
state_t state, next_state;

////////////////////////
// Infer State Flops //
//////////////////////
always_ff @(posedge clk, negedge rst_n) begin
if (!rst_n)
   state <= IDLE;
else
   state <= next_state;
end

//////////////////////////
// State Machine Logic //
////////////////////////
always_comb begin
   // default outputs to avoid latches
   next_state = state; 
   update_cmd_x = 1'b0;
   update_cmd_y = 1'b0;
   sel_UART_SM = 1'b1; // By default select the cmd from SM
   inc_mv_index = 1'b0;
   clr_mv_index = 1'b0;
   set_cmd_rdy_SM = 1'b0;

   case(state)
        //IDLE: Stay unitl start_tour is asserted. Always select from UART signal. 
        IDLE : begin
            if(start_tour) begin
                clr_mv_index = 1'b1;
                next_state = FORM_VERT;
            end
            sel_UART_SM = 1'b0; //select UART signals in IDLE
        end
        /*
        FORM_VERT: Assert that we should set the command to be 
        the vertical component of the move. Once clr_cmd_rdy is asserted
        we move into the wait state.
        */
        FORM_VERT : begin
            update_cmd_y = 1;
            set_cmd_rdy_SM = 1'b1;
            if(clr_cmd_rdy) begin
               next_state = WAIT_VERT;
               
            end    
        end
        /*
         Assert that we should set the command to be 
         the horizontal component of the move. Once clr_cmd_rdy is asserted
         we move into the wait state.
        */
        FORM_HORZ : begin
            update_cmd_x = 1;
            set_cmd_rdy_SM = 1'b1;
            if(clr_cmd_rdy) begin
               next_state = WAIT_HORZ;
               inc_mv_index = 1'b1;
            end      
        end

         /*
         Waits for the vertical component of the move to complete
         as indicated by send_resp.
         */
         WAIT_VERT : begin
            update_cmd_y = 1;
            if(send_resp) begin 
               next_state = FORM_HORZ;
            end

         end
         
         /*
         Waits for the horizontal component of the move to complete
         as indicated by send_resp. Go to idle if we complete the tour.  
         */
         WAIT_HORZ : begin
            update_cmd_x = 1;
            if(send_resp) begin //Wait for send_resp and see if we are on the final move.
              
               if(mv_indx == 5'd23)begin
                  next_state = IDLE;   //If it is the last move, we go back to IDLE.
               end
               else begin
                  next_state = FORM_VERT;
               end
            end
         end
   endcase
end
endmodule