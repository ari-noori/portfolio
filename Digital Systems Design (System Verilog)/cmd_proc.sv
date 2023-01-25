
module cmd_proc(clk,rst_n,cmd,cmd_rdy,clr_cmd_rdy,send_resp,strt_cal,
                cal_done,heading,heading_rdy,lftIR,cntrIR,rghtIR,error,
                frwrd,moving,tour_go,fanfare_go);

  parameter FAST_SIM = 1;		    // speeds up incrementing of frwrd register for faster simulation

  input clk,rst_n;			        // 50MHz clock and asynch active low reset
  input [15:0] cmd;			        // command from BLE
  input cmd_rdy;			          // command ready
  output logic clr_cmd_rdy;		  // mark command as consumed
  output logic send_resp;		    // command finished, send_response via UART_wrapper/BT
  output logic strt_cal;		    // initiate calibration of gyro
  input cal_done;			          // calibration of gyro done
  input signed [11:0] heading;	// heading from gyro
  input heading_rdy;			      // pulses high 1 clk for valid heading reading
  input lftIR;				          // nudge error +
  input cntrIR;				          // center IR reading (have I passed a line)
  input rghtIR;				          // nudge error -
  output reg signed [11:0] error;	// error to PID (heading - desired_heading)
  output reg [9:0] frwrd;		    // forward speed register
  output logic moving;			    // asserted when moving (allows yaw integration)
  output logic tour_go;			    // pulse to initiate TourCmd block
  output logic fanfare_go;		  // kick off the "Charge!" fanfare on piezo

  /////////////////////
  // frwrd register //
  ///////////////////
  logic clr_frwrd, dec_frwrd, inc_frwrd;  // cmd_proc_SM outputs
  logic zero;				                      // frwrd is ramped back down to zero when this is asserted
  logic [7:0]increment;                   // 0x04 when FAST_SIM is 0, 0x20 when FAST_SIM is 1
  logic [7:0]decrement;                   // always twice the magnitude of increment

  /*
  Fast Simulation: increment is always 2x decrement. 
  */        
  generate if(FAST_SIM) 
    begin
      assign increment = 8'h20;       // simulation is 8 times faster during FAST_SIM
      assign decrement = 2 * increment;
    end 
    else begin
        assign increment = 8'h04;
        assign decrement = 2 * increment;
    end
  endgenerate

  /*
  This is the frwrd register block, including the enable_FF that takes
  in the ALU result of the FF output and the incremental amount that
  is either 0x04 or 0x20 depending on whether FAST_SIM is asserted
  */
  always_ff @(posedge clk, negedge rst_n) begin
    // frwrd is incremented or decremented on heading_rdy rising edge
    if(!rst_n)
      frwrd <= 10'h000;
    else if(clr_frwrd)
      frwrd <= 10'h000;
    else if(heading_rdy) begin
      // increment forward
      if(inc_frwrd && ~&frwrd[9:8])
        frwrd <= frwrd + increment;
      // decrement forward
      else if(dec_frwrd) begin
        // check if the decrement would make forward negative
        if (decrement > frwrd) begin
          frwrd <= 10'h000;
        end
        // else just decrement
        else begin
          frwrd <= frwrd - decrement;
        end
      end
    end
  end
  
  assign zero = ~|frwrd; // asserted if all bits of frwrd are 0's

  ///////////////////////
  // counting squares //
  /////////////////////
  logic move_cmd;
  logic move_done;
  logic cntrIR_delay;
  logic cntrIR_rise;
  logic [3:0]num_lines, line_cnt;

  // The following always block makes sure cntrIR_delay is always 1 clock edge behind cntrlIR
  always @(posedge clk) begin
    cntrIR_delay <= cntrIR;
  end

  /*
  Because cntrlIR_delay is asserted 1 clock edge after cntrIR is asserted, cntrl_rise
  is only high for one clock edge per line encountered by the cntrIR sensor
  */
  assign cntrIR_rise = (cntrIR && ~cntrIR_delay);

  /*
  FF that increments line_cnt by 1 everytime cntrIR has a rising edge, which is
  everytime that the cntrIR sensor detects a line in a one of the square
  */
  always_ff @(posedge clk, posedge move_cmd, negedge rst_n)
    if(!rst_n)
      line_cnt <= 4'b0000;        // asynchronous reset signal
    else if(move_cmd)
      line_cnt <= 4'b0000;        // move_cmd's assertion acts as a synchronous reset signal for line_cnt
    else if(cntrIR_rise)
      line_cnt <= line_cnt + 1;   // line_cnt is incremented by 1 at every rising edge of cntrIR

  /*
  FF that determines how many lines should be detected by doubling the number of
  squares to be moved. The cntrIR detects two lines for every square moved.
  */
  always_ff @(posedge clk, negedge rst_n)
    if(!rst_n)
      num_lines <= 4'b0000;
    else if(move_cmd)
      num_lines <= {cmd[2:0], 1'b0};

  /*
  move_done is asserted once line_cnt equals specified number of lines to be
  detected, which is just twice the number of squares to be moved
  */
  assign move_done = (num_lines == line_cnt != 0);
    
  ////////////////////
  // PID interface //
  //////////////////
  logic [11:0]desired_heading, heading_ff_in;
  logic [11:0]err_nudge, err_abs;

  /*
  The 12-bit heading_ff_in signal is, as you might've guessed, the input to
  the FF that outputs to the desired_heading signal.
  The middle 8 bits of the cmd signal specify the heading. If the middle 8 bits
  of cmd are zeroes, then heading_ff_in is assigned to be 12'h000. If any of
  the middle 8 bits of cmd are non-zero, heading_ff_in is assigned to be 
  cmd[11:4] appended with four ones at the four LSB (4'b1111 or 4'hF).
  */
  assign heading_ff_in = (cmd[11:4] != 8'h00) ? {cmd[11:4], 4'hF} : 12'h000;
    
  // FF for assigning heading_ff_in to desired_heading when move_cmd is asserted
  always_ff @(posedge clk, negedge rst_n)
    if(!rst_n)
      desired_heading <= 12'h000;
    else if(move_cmd)
      desired_heading <= heading_ff_in;

  /*
  If lftIR is asserted, we will add an additional 12'h05F to the error term.
  If rghtIR is asserted, we will add an additional 12'hFA1 to the error term.
  If neither sensor is detecting anything, err_nudge remains zero.
  If the FAST_SIM parameter is asserted, the values to add to error based on the 
  assertion of lftIR or rghtIR are changed to 12'h1FF and 12'hE00 respectively.
  */
  assign err_nudge = FAST_SIM ? lftIR ? 12'h1FF : rghtIR ? 12'hE00 : 12'h000 :
                        lftIR ? 12'h05F : rghtIR ? 12'hFA1 : 12'h000;

  /*
  The error term is the difference between heading and desired_heading summed
  with err_nudge (if err_nudge is needed).
  */
  assign error = heading - desired_heading + err_nudge;
  assign err_abs = error[11] ? -error : error;

  ////////////////////////////////////
  // command processing/SM control //
  //////////////////////////////////
  /*
  Extract the signals from move command:
    cmd[15:12] = opcode
    cmd[11:4] = heading. If non-zero promote 4-bits and append 4'hF
    cmd[3:0] = number squares to move
  */
  logic [3:0] opcode;
  assign opcode = cmd[15:12];

  /* There are only four commands that The Knight can interpret:
    1. 4'b0000: Calibrate command
    2. 4'b0010: Move command
    3. 4'b0011: Move command with the fanfare theme playing
    4. 4'b0100: Start Knight's Tour movement path 
  */
  enum logic [3:0] {CALIBRATE = 4'b0000, MOVE = 4'b0010, MOVE_FANFARE = 4'b0011, BEGIN_TOUR = 4'b0100 } opcodes;
    
  // state enum and declaration
  typedef enum reg[2:0] {IDLE, CALIBRATE_STATE, MOVE_STATE, UPDATE, SLOW_DOWN} state_t;
  state_t state, nxt_state;

  ////////////////////////////
  // State maintaining FF  //
  //////////////////////////
  always_ff @(posedge clk, negedge rst_n) begin
      if (!rst_n)
          state <= IDLE;
      else
          state <= nxt_state;
  end 
  
  //////////////////////////////
  // State maintaining Logic //
  ////////////////////////////
  always_comb begin
      nxt_state = state;  // default to avoid latches
      
      // default all outputs to prevent latches
      clr_cmd_rdy = 0;
      strt_cal = 0;
      send_resp = 0;
      tour_go = 0;
      move_cmd = 0;
      inc_frwrd = 0;
      dec_frwrd = 0;
	    clr_frwrd = 0;
      moving = 0;
      fanfare_go = 0;

      case (state)
          /*
          This state waits for cmd ready and goes into the respective state based
          on the opcode it recieves. 
          */
          IDLE : begin
            // wait in IDLE until there is a cmd_rdy
            if (cmd_rdy) begin
              // dispatch to the appropriate state based on opcode
              case (opcode)
                CALIBRATE : begin
                  // command is calibrate
                  nxt_state = CALIBRATE_STATE;
				  
                  // assert strt_cal
                  strt_cal = 1;
                  
                  // assert clr_cmd_rdy
                  clr_cmd_rdy = 1;
                end
                BEGIN_TOUR : begin
                  // assert clr_cmd_rdy
                  clr_cmd_rdy = 1;
                  
                  // simply switch control to TourCmd by asserting tour_go
                  tour_go = 1;
                end
                // using as MOVE and MOVE_FANFARE states
                default : begin
                  // command is to move 
                  nxt_state = UPDATE;
                  move_cmd = 1;
                  clr_cmd_rdy = 1;
                end
              endcase
            end
          end
          /*
          Calibrate waits for cal_done to be asserted and tell a response to be sent.
          Go back to IDLE after we calibrate. 
          */
          CALIBRATE_STATE : begin
            // wait for cal_done
            if (cal_done) begin
              // send_resp (0xA5 positive acknowledge) when cal_done
              send_resp = 1;
              nxt_state = IDLE;
            end
          end
          /*
          Update state send us into the move state if the errror is within
          a certain acceptable bound. 
          */
          UPDATE : begin
            if (error < $signed(12'h030) && error > $signed(12'hFD0)) begin
              // Once our signed error falls between +- 12'h030, then our heading is "close enough"
              nxt_state = MOVE_STATE; // We can move to this state and start to ramp up fwrd spd
            end
            moving = 1; //Start moving
            clr_frwrd = 1;
          end
          /*
          Move state goes into slowdown if our movement is done, and also 
          initiates fanfare based on the 12th bit of our command. 
          */
          MOVE_STATE : begin
            if (move_done) begin
              nxt_state = SLOW_DOWN;   // As the squares are being counted, we wait for the move to complete.
              if(cmd[12]) begin        // When the lower bit of the opcode is 1, we play the fanfare at the end of the move.
                fanfare_go = 1;
              end
            end
            else begin
              inc_frwrd = 1;    // Continue ramping up the speed
            end
            moving = 1;
          end
          /*
          Slow down will ramp down our speed of our robot and send us back to IDLE when
          it is done moving. 
          */
          SLOW_DOWN : begin     // After we move the appropiate number of squares, we ramp down the spd
            if (zero) begin
              send_resp = 1;    // Send_resp when the speed reaches zero
              nxt_state = IDLE;
            end
            else begin
              moving = 1;
              dec_frwrd = 1;    // Keep decrementing the speed until we reach zero.
            end
          end
      endcase
  end
endmodule