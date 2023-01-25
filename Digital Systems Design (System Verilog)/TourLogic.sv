
module TourLogic(clk,rst_n,x_start,y_start,go,done,indx,move);

  input clk,rst_n;				// 50MHz clock and active low asynch reset
  input [2:0] x_start, y_start;	// starting position on 5x5 board
  input go;						// initiate calculation of solution
  input [4:0] indx;				// used to specify index of move to read out
  output logic done;			 // pulses high for 1 clock when solution complete
  output logic [7:0] move;			// the move addressed by indx (1 of 24 moves)
  
  ////////////////////////////////////////
  // Declare needed internal registers //
  //////////////////////////////////////
  // 2-D array of 5-bit vectors that keep track of where on the board the knight
  // has visited.  Will be reduced to 1-bit boolean after debug phase
  logic [4:0] board [4:0][4:0];

  // 1-D array (of size 24) to keep track of last move taken from each move index
  logic [7:0] last_move [23:0];

  // 1-D array (of size 24) to keep track of possible moves from each move index
  logic [7:0] possible_moves [23:0];

  logic [7:0] move_index; // since we are using 1-hot states, I believe move_index should
                          // be an 8-bit signal so we can use it as the try parameter for
                          // off_x and off_y

  // 5-bit vector that counts the number of completed moves. When move_num reaches 24,
  // the tour is complete. move_num is decremented everytime the robot backs up.
  logic [4:0] move_num;

  // xx, yy couple of 3-bit vectors that represent the current x/y coordinates of the knight
  logic [2:0] xx, yy;

  // state machine outputs to control logic
  logic   clear_board, set_start_pos, update_pos, backup_pos, update_offset, update_offset_backup, 
          progress_move_index, update_possible, update_index;

  /////////////////////////////////////////////////
  //           STATE MACHINE LOGIC              //
	///////////////////////////////////////////////
  // state enum and declaration
  typedef enum reg[2:0] {IDLE, INIT, POSSIBLE, MAKE_MOVE, BACKUP, FINISH} state_t;
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
      // default all outputs to prevent latches
      nxt_state = state;
      clear_board = 0;
      set_start_pos = 0;
      update_pos = 0;
      backup_pos = 0;
      update_offset = 0;
      update_offset_backup = 0;
      done = 0;
      progress_move_index = 0;
      update_possible = 0;
      update_index = 1;

      case (state)
          IDLE : begin
            if(go) begin
              // clear the board to all zero's and reset move_num
              clear_board = 1;

              // initialize the first board position
              nxt_state = INIT;
                    update_index = 1;
            end
          end
          INIT : begin
            // mark the starting positon as visited
            set_start_pos = 1;

            // move into possible state
            nxt_state = POSSIBLE;
          end
          POSSIBLE : begin
            update_possible = 1;

            // make the move
            nxt_state = MAKE_MOVE;
            update_offset = 1;
          end
          MAKE_MOVE : begin
            update_offset = 1;
            // get the x and y offset for current move
            // check the move is valid (the move is in-bounds and we have not visited)
            if (
              // move in-bounds if the one-hot for that move number is high
              // do a bit wise and over the possible moves and one-hot move index and then reduce with OR
              (|(possible_moves[move_num])) && 
              // square is not visited if board position value is 0
              (board[xx + off_x(move_index)][yy + off_y(move_index)] == 0)
            ) begin
              // update board and xx and yy
              update_pos = 1;
              
              // check if we are the last move, if so move into FINISH
              if(move_num == 8'd23) begin
                nxt_state = FINISH;
              end
              else begin
                // move onto getting possible moves for next square
                nxt_state = POSSIBLE;
              end

              // increment the move number
            end
            else if (move_index != 8'h80) begin
              progress_move_index = 1;
            end
            else begin
              // no more possible moves, need to backup
              update_offset_backup = 1;
              nxt_state = BACKUP;
            end
          end
          FINISH: begin
            // FINISH state asserts done and then moves back to IDLE
            nxt_state = IDLE;
            done = 1;
          end
          // using default as BACKUP state
          default : begin
            update_offset_backup = 1;
            // undo having visited last square
            backup_pos = 1;

            // try to make the next move
            if (last_move[move_num - 1] != 8'h80) begin
              update_offset = 1;
              nxt_state = MAKE_MOVE;
            end
          end
      endcase
  end
  
  /////////////////////////////////////////////////
  //         CALCULATE POSSIBLE MOVES           //
	///////////////////////////////////////////////
  /*
  Returns a packed byte of all the possible moves 
  (at least in bound) moves given coordinates of Knight.
  
  The best case secenario is all 8 moves are possible 
  for the knight. 

  All of the possible moves are represented by a one 
  hot encoding in a byte. The LSB would correspond 
  to move 0 which is +2 in Y and -1 in X.

  This function looks at all of the 8 possible moves and 
  ensures they are in bounds of the board given the 
  current position.
  */
  function [7:0] calc_poss();
    // LSB & move 0, represents Y+2, X-1
    return {
      (xx + 2 < 5) && (yy + 1 < 5),
      (xx + 2 < 5) && (yy - 1 >= 0),
      (xx + 1 < 5) && (yy - 2 >= 0),
      (xx - 1 >= 0) && (yy - 2 >= 0),
      (xx - 1 >= 0) && (yy - 2 >= 0),
      (xx - 2 >= 0) && (yy - 1 >= 0),
      (xx - 2 >= 0) && (yy + 1 < 5),
      (xx + 1 < 5) && (yy + 2 < 5),
      (xx - 1 >= 0) && (yy + 2 < 5)
    };
  endfunction
  
  /*
	Consider writing a function that returns a the x-offset
	the Knight will move given the encoding of the move you
	are going to try.  Can also be useful when backing up
	by passing in last move you did try, and subtracting 
	the resulting offset from xx.
	*/
  function signed [2:0] off_x(input [7:0] try);
    // all moves match order of moves in project specification
    logic signed [2:0] x_offset;
    case (try)
      8'b00000001: x_offset = 3'b111; // left 1
      8'b00000010: x_offset = 3'b001; // right 1
      8'b00000100: x_offset = 3'b110; // left 2
      8'b00001000: x_offset = 3'b110; // left 2
      8'b00010000: x_offset = 3'b111; // left 1
      8'b00100000: x_offset = 3'b001; // right 1
      8'b01000000: x_offset = 3'b010; // right 2
      8'b10000000: x_offset = 3'b010; // right 2
    endcase
    return x_offset;
  endfunction
  
  /*
	Consider writing a function that returns a the y-offset
	the Knight will move given the encoding of the move you
	are going to try.  Can also be useful when backing up
	by passing in last move you did try, and subtracting 
	the resulting offset from yy
	*/
  function signed [2:0] off_y(input [7:0] try);
    logic signed [2:0] y_offset;
    case (try)
      8'b00000001: y_offset = 3'b010; // up 2
      8'b00000010: y_offset = 3'b010; // up 2
      8'b00000100: y_offset = 3'b001; // up 1
      8'b00001000: y_offset = 3'b111; // down 1
      8'b00010000: y_offset = 3'b110; // down 2
      8'b00100000: y_offset = 3'b110; // down 2
      8'b01000000: y_offset = 3'b111; // down 1
      8'b10000000: y_offset = 3'b001; // up 1
    endcase
    return y_offset;
  endfunction

  /////////////////////////////////////////////////
  //      UPDATE BOARD STATUS AND POSITION      //
	///////////////////////////////////////////////
  always_ff @(posedge clk ) begin
    if(clear_board) begin 
      // create the board
      board <= '{'{0,0,0,0,0}, '{0,0,0,0,0}, '{0,0,0,0,0}, '{0,0,0,0,0}, '{0,0,0,0,0}};
      // clear num moves
      move_num <= 0;
    end
    else if (set_start_pos) begin
      // The starting board position is set to 1, representing it has been visited
      board[x_start][y_start] <= 1'b1;
      xx <= x_start; // The current x-position is set to the starting x-position
      yy <= y_start; // The current y-position is set to the starting y-position
    end
    else if (update_pos) begin
      // Current x and y coordinates are updated based on the value of move_index
      xx <= xx + off_x(move_index);
      yy <= yy + off_y(move_index);
      board[xx][yy] <= 1; // The current board position is marked as visited
      last_move[move_num] <= move_index;
      move_num <= move_num + 1;
    end
    else if (backup_pos) begin
      board[xx][yy] <= 1'b0;
      xx <= xx - off_x(last_move[move_num - 1]);
      yy <= yy - off_y(last_move[move_num - 1]);

      // next move to try is the last one advanced by 1
      move_index <= (last_move[move_num - 1] << 1);

      // If all possible moves have been tried, backup and try possible moves
      // from the previous position.
      move_num <= move_num - 1;
    end
    else if (progress_move_index) begin
      // if move is not possible, try next
      move_index <= (move_index << 1);
    end
    else if (update_possible) begin
      // get all possible moves for this square
      possible_moves[move_num] <= calc_poss();

      // try the LSB move first
      move_index <= 8'h01;
    end
  end
  always_ff @(posedge clk ) begin
    if (update_index) begin
      // get all possible moves for this square
      move <= last_move[indx];
    end
  end
endmodule