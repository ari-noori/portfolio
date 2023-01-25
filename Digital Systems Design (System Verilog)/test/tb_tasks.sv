///////////////////////////////////////
//              TASKS                //
///////////////////////////////////////
// Contains all the tasks that are   //
// repeated between testbenches     //
/////////////////////////////////////

/*
Zeros all the input signals to the DUT and asserts/deasserts reset. 
*/
task automatic InitializeTB();
  //Zero input signals and establish clk/rst_n
  clk = 0;
  RST_n = 0;
  cmd = 16'd0;
  send_cmd = 1'b0;
  @(posedge clk);
  @(negedge clk) RST_n = 1; //deassert reset
  $display("DONE - InitializeTB");
endtask

/*
Checks for midrail PWM values and if NEMO_setup asserts after we reset the DUT
*/
task automatic CheckDUTInitialize();
  fork
    begin

      //Check that the PWMs fire right after reset is asserted
      fork
        begin repeat(25) @(posedge iDUT.iMTR.lftPWM1); end 
        begin repeat(25) @(posedge iDUT.iMTR.rghtPWM1); end
      join

      //Check if NEMO_setup is asserted
      @(posedge iPHYS.iNEMO.NEMO_setup) begin
        disable timeout;
        $display("PASS - CheckDUTInitialize");
      end
    end
    begin : timeout
      repeat(TIMEOUT_NORMAL) @(posedge clk); //Timeout if NEMO_setup is never asserted
      $display("The CheckDUTInitialize test timed out!");
      $stop();
    end
  join
endtask

/*
Sends a calibrate command thru UART and checks if cal_done is asserted
and remoteComm recieves a positive A5 acknowledge.
*/
task automatic SendCalibrateCmd();
  cmd = 16'h0000; //calibrate command (into RemoteComm)
  @(posedge clk);
  send_cmd = 1;  //Tell RemoteComm to send the commmand
  @(posedge clk);
  @(negedge clk) send_cmd = 0;
  fork
    begin
      @(posedge iDUT.cal_done) begin  //wait for cal_done
        @(posedge resp_rdy) begin     //wait for RemoteComm resp to be ready
          if(resp !== 8'hA5) begin    //positive acknowledge
            $display("Positive acknowledge not recieved (A5)");
            $stop();
          end
          disable timeout;
          $display("PASS - SendCalibrateCmd");
        end
      end
    end
    begin : timeout
      repeat(TIMEOUT_NORMAL) @(posedge clk); //Timeout if cal_done is never asserted
      $display("The test timed out!");
      $stop();
    end
  join
endtask

/*
This task waits for two send resp signals indicating that both of the two parts
of the move has been completed. 

It then checks that the sqaure the knight is in from KnightPhysics is correct,
and that the resp recieved from tour command is 5A (in progress).
*/
task automatic run(input integer x, input integer y);
  // wait for the response
  @(posedge iDUT.iCMD.send_resp);
  @(posedge iDUT.iCMD.send_resp) begin
    // print current square the knight is in
    $display("(%0d, %0d)", iPHYS.xx/BOARD_WIDTH,iPHYS.yy/BOARD_WIDTH);

    // verify that the square is correct based on tour logic solution
    if(iPHYS.xx/BOARD_WIDTH !== x || iPHYS.yy/BOARD_WIDTH !== y) begin
      $display("Knight not on the correct square, supposed to be (%0d, %0d), but was (%0d, %0d)", x, y, iPHYS.xx/BOARD_WIDTH,iPHYS.yy/BOARD_WIDTH);
      $stop();
    end

    // verify that the response from tour_cmd is 0x5A (in progress)
    if (resp !== 8'h5A) begin
      $display("resp recieved was incorrect");
      $stop();
    end
  end
endtask