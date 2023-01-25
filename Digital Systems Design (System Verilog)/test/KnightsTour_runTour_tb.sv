/* 
This testbench simulates a run through the first 3 moves of the knights tour solution.
In this particular tb, we start at the 2,2 square (x,y) system and run through the moves.
Before the tour is started, we have tasks that will confirm that our knight has initialized 
and that our knight has been calibrated correctly. Before the first move, we simply check to 
make sure the knight is on the correct starting piece. After two posedge's of send_resp, we then
check that the xx and yy coordinates were updated correctly and continue the tour. This specific
test also checks that A5 is on the resp signal when the knight completes its tour. It does this
by forcing the mv_indx signal to be 23 thus prompting the end of the tour. 
*/
module KnightsTour_runTour_tb();
    `include "tb_params.sv";

/////////////////////////////
// Stimulus of type reg //
/////////////////////////
reg clk, RST_n;
reg [15:0] cmd;
reg send_cmd;

///////////////////////////////////
// Declare any internal signals //
/////////////////////////////////
wire SS_n,SCLK,MOSI,MISO,INT;
wire lftPWM1,lftPWM2,rghtPWM1,rghtPWM2;
wire TX_RX, RX_TX;
logic cmd_sent;
logic resp_rdy;
logic [7:0] resp;
wire IR_en;
wire lftIR_n,rghtIR_n,cntrIR_n;
logic piezo, piezo_n;

//////////////////////
// Instantiate DUT //
////////////////////
KnightsTour iDUT(.clk(clk), .RST_n(RST_n), .SS_n(SS_n), .SCLK(SCLK),
                .MOSI(MOSI), .MISO(MISO), .INT(INT), .lftPWM1(lftPWM1),
                .lftPWM2(lftPWM2), .rghtPWM1(rghtPWM1), .rghtPWM2(rghtPWM2),
                .RX(TX_RX), .TX(RX_TX), .piezo(piezo), .piezo_n(piezo_n),
                .IR_en(IR_en), .lftIR_n(lftIR_n), .rghtIR_n(rghtIR_n),
                .cntrIR_n(cntrIR_n));
        
/////////////////////////////////////////////////////
// Instantiate RemoteComm to send commands to DUT //
///////////////////////////////////////////////////
RemoteComm iRMT(.clk(clk), .rst_n(RST_n), .RX(RX_TX), .TX(TX_RX), .cmd(cmd),
                .send_cmd(send_cmd), .cmd_sent(cmd_sent), .resp_rdy(resp_rdy), .resp(resp));
          
//////////////////////////////////////////////////////
// Instantiate model of Knight Physics (and board) //
////////////////////////////////////////////////////
KnightPhysics iPHYS(.clk(clk),.RST_n(RST_n),.SS_n(SS_n),.SCLK(SCLK),.MISO(MISO),
                    .MOSI(MOSI),.INT(INT),.lftPWM1(lftPWM1),.lftPWM2(lftPWM2),
                    .rghtPWM1(rghtPWM1),.rghtPWM2(rghtPWM2),.IR_en(IR_en),
                    .lftIR_n(lftIR_n),.rghtIR_n(rghtIR_n),.cntrIR_n(cntrIR_n)); 

initial begin
  InitializeTB();       // Set initial values, clk, rst_n
  CheckDUTInitialize(); // Check if our Knight initializes
  SendCalibrateCmd();   // Calibrate the Knight
  repeat(400000) @(posedge clk); //Give it a second to rest before charging into battle
  runTour();          //  Simulate a tour from (2,2)

  /*
  Force the move index to be 23 in order to check if the response 
  changes to 0xA5 (done)
  */
  force iDUT.iTC.mv_indx = 5'd23;
  repeat(40000) @(posedge clk); // wait a bit for the signal to propegate
  if(resp !== 8'hA5) begin
    $display("Response not A5 at the end of the tour!");
    $stop();
  end 

  $display("YAHOO - runTour - TESTS PASSED!");
  $stop();
end

/////////////
// TASKS  //
///////////
`include "tb_tasks.sv";

/*
Issues a command to start the tour at (2,2), then goes through 3 moves and checks if the robot is in the correct square. 
*/
task automatic runTour();
  cmd = 16'b0100_0000_0010_0010; //start tour at (2,2)
  @(posedge clk);
  send_cmd = 1;  //Tell RemoteComm to send the commmand
  @(posedge clk);
  @(negedge clk) send_cmd = 0;
  @(posedge iDUT.tour_go);
  @(posedge iDUT.start_tour);

  // check the starting position
  if(iDUT.iTL.x_start !== 2 || iDUT.iTL.y_start !== 2) begin
    $display("start_x or start_y in tourLogic wrong!");
    $stop();
  end

  // print the current position (starting position)
  $display("(%0d, %0d)", iPHYS.xx/BOARD_WIDTH,iPHYS.yy/BOARD_WIDTH);

  //Check move to (1,4)
  run(1,4);

  //Check move to (0,2)
  run(0,2);

  //Check move to (1,0)
  run(1,0);
endtask

always
  #5 clk = ~clk;

endmodule