//////////////////////////////////////
//              PARAMS              //
//////////////////////////////////////
// Different durations for timeout           
localparam TIMEOUT_NORMAL = 1000000;    // Number of cycles to wait for timeout
localparam TIMEOUT_LONG = 8000000;      // Number of cycles to wait for timeout
localparam WAIT_SHORT = 150000;         // Time period to wait between tasks

// Board specifications
localparam BOARD_WIDTH = 13'd4096;      // Width of the board used to determine square in

// Response values
localparam TOUR_COMPLETE = 8'hA5;
localparam MOVE_COMPLETE = 8'h5A;