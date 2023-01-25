/*
This testbench ensures that we can move the knight North, East, West, or south on command.
We do this by sending a fixed command inone of the direction and wathcing the convergence of
the heading and the disered heading. If they never converge, the test fails. Otherwise we ensure
that the knight is on the square that correspond to the directon we attempted to mov it in.
Before the move is started, we have tasks that will confirm that our knight has initialized 
and that our knight has been calibrated correctly. Before the first move, we simply tell 
RemoteComm to send the commmand. After 2 bumps of cntrIR per square we move and once the
knight finishes its move, we then check that the heading and desired heading have converged
and that the knight ended up on the right square. The last two tests also check that the fanfare
goes off. 
*/
module KnightsTour_move_tb();
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
    InitializeTB();
    CheckDUTInitialize();
    SendCalibrateCmd();
    moveEast();
    repeat (WAIT_SHORT) @(posedge clk);
    moveNorth();
    repeat (WAIT_SHORT) @(posedge clk);
    moveWestFanfare();
    repeat (WAIT_SHORT) @(posedge clk);
    moveSouthFanfare();
    $display("YAHOO - move - TESTS PASSED!");
    $stop();
  end

  /////////////
  // TASKS  //
  ///////////
  `include "tb_tasks.sv";

  /*
  Moves the knight east one square. 
  */
  task automatic moveEast();
    fork
      begin
        cmd = 16'h3BF1;
        @(posedge clk);
        @(posedge clk);
        send_cmd = 1;  //Tell RemoteComm to send the commmand
        @(posedge clk);
        @(negedge clk) send_cmd = 0;

        //check if error accumulates when the robot begins to move
        @(posedge iDUT.iCMD.moving) repeat(50) @(posedge clk);
        if (iDUT.iCMD.error === 12'h0) begin
          $display("Error never changed!");
          $stop();
        end

        @(posedge iDUT.iCMD.cntrIR); //We want 2 bumps of cntrIR
        @(posedge iDUT.iCMD.cntrIR);
        @(negedge iDUT.iCMD.moving);

        //Check convergence of heading and desired heading
        if(($signed(iDUT.iCMD.heading) > $signed(iDUT.iCMD.desired_heading) + 55) || 
          ($signed(iDUT.iCMD.heading) < $signed(iDUT.iCMD.desired_heading) - 55)) begin
            $display("heading never converged!");
            $stop();
          end

        //Check if knight is on the correct square   
        if(iPHYS.xx < 16'h3200 || iPHYS.xx > 16'h4100) begin //degrees of freedom within square
          $display("Knight not on correct square - moveEast!");
          $stop();
        end

        $display("PASS - moveEast");
        disable timeout;
      end
      begin : timeout
        repeat(TIMEOUT_LONG) @(posedge clk); //Timeout if knight never stops
        $display("The moveEast timed out!");
        $stop();
      end
    join
  endtask

  /*
  Moves the knight north two squares. 
  */
  task automatic moveNorth();
    fork
      begin
        cmd = 16'h2002;
        @(posedge clk);
        @(posedge clk);
        send_cmd = 1;  //Tell RemoteComm to send the commmand
        @(posedge clk);
        @(negedge clk) send_cmd = 0;
        @(posedge iDUT.iCMD.cntrIR); //We want 4 bumps of cntrIR
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.send_resp); //Check if knight stops

        //Check convergence of heading and desired heading
        if($signed(iDUT.iCMD.heading) > $signed(iDUT.iCMD.desired_heading) + 55 || 
          $signed(iDUT.iCMD.heading) < $signed(iDUT.iCMD.desired_heading) - 55 ) begin
            $display("heading never converged!");
            $stop();
          end

        //Check if knight is on the correct square   
        if(iPHYS.yy < 16'h4000 || iPHYS.yy > 16'h5000) begin //degrees of freedom within square
          $display("Knight not on correct square - moveNorth!");
          $stop();
        end

        $display("PASS - moveNorth");
        disable timeout;
      end
      begin : timeout
        repeat(TIMEOUT_LONG) @(posedge clk); //Timeout if knight never stops
        $display("The moveNorth timed out!");
        $stop();
      end
    join
  endtask

  /*
  Moves the knight west two squares with fanfare. 
  */
  task automatic moveWestFanfare();
    fork
      begin
        cmd = 16'h33F2;
        @(posedge clk);
        @(posedge clk);
        send_cmd = 1;  //Tell RemoteComm to send the commmand
        @(posedge clk);
        @(negedge clk) send_cmd = 0;
        @(posedge iDUT.iCMD.cntrIR); //We want 4 bumps of cntrIR
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.fanfare_go) $display("CHARGE!"); //Check if fanfare is asserted
        @(posedge iDUT.send_resp); //Check if knight stops

        //Check convergence of heading and desired heading
        if($signed(iDUT.iCMD.heading) > $signed(iDUT.iCMD.desired_heading) + 55 || 
          $signed(iDUT.iCMD.heading) < $signed(iDUT.iCMD.desired_heading) - 55 ) begin
            $display("heading never converged!");
            $stop();
          end

        //Check if knight is on the correct square   
        if(iPHYS.xx < 16'h1000 || iPHYS.xx > 16'h2000) begin //degrees of freedom within square
          $display("Knight not on correct square - moveWestFanfare!");
          $stop();
        end

        $display("PASS - moveWestFanfare");
        disable timeout;
      end
      begin : timeout
        repeat(TIMEOUT_LONG) @(posedge clk); //Timeout if knight never stops
        $display("The moveWestFanfare timed out!");
        $stop();
      end
    join
  endtask

  /*
  Moves the knight south two squares with fanfare. 
  */
  task automatic moveSouthFanfare();
    fork
      begin
        cmd = 16'h37F2;
        @(posedge clk);
        @(posedge clk);
        send_cmd = 1;  //Tell RemoteComm to send the commmand
        @(posedge clk);
        @(negedge clk) send_cmd = 0;
        @(posedge iDUT.iCMD.cntrIR); //We want 4 bumps of cntrIR
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.cntrIR);
        @(posedge iDUT.iCMD.fanfare_go) $display("CHARGE!"); //Check if fanfare is asserted
        @(posedge iDUT.send_resp); //Check if knight stops

        //Check convergence of heading and desired heading
        if(iDUT.iCMD.heading[11:0] > $signed(iDUT.iCMD.desired_heading) + 55 || 
          iDUT.iCMD.heading[11:0] < $signed(iDUT.iCMD.desired_heading) - 55 ) begin
            $display("heading never converged!");
            $stop();
          end

        //Check if knight is on the correct square   
        if(iPHYS.yy < 16'h2000 || iPHYS.yy > 16'h3000) begin //degrees of freedom within square
          $display("Knight not on correct square - moveSouthFanfare!");
          $stop();
        end

        $display("PASS - moveSouthFanfare");
        disable timeout;
      end
      begin : timeout
        repeat(TIMEOUT_LONG) @(posedge clk); //Timeout if knight never stops
        $display("The moveSouthFanfare timed out!");
        $stop();
      end
    join
  endtask

  always
    #5 clk = ~clk;

endmodule