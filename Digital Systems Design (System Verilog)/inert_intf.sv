
//////////////////////////////////////////////////////
// Interfaces with ST 6-axis inertial sensor.  In  //
// this application we only use Z-axis gyro for   //
// heading of robot.  Fusion correction comes    //
// from "gaurdrail" signals lftIR/rghtIR.       //
/////////////////////////////////////////////////
module inert_intf(  clk,rst_n,strt_cal,cal_done,heading,rdy,lftIR,
                    rghtIR,SS_n,SCLK,MOSI,MISO,INT,moving);
  
  parameter FAST_SIM = 1;               // used to accelerate simulation

  input clk, rst_n;
  input MISO;                           // SPI input from inertial sensor
  input INT;                            // goes high when measurement ready
  input strt_cal;                       // initiate claibration of yaw readings
  input moving;                         // Only integrate yaw when going
  input lftIR,rghtIR;                   // gaurdrail sensors
  
  output cal_done;                      // pulses high for 1 clock when calibration done
  output signed [11:0] heading;         // heading of robot.  000 = Orig dir 3FF = 90 CCW 7FF = 180 CCW
  output rdy;                           // goes high for 1 clock when new outputs ready (from inertial_integrator)
  output SS_n,SCLK,MOSI;                // SPI outputs

  //////////////////////////////////////////////
  // Declare outputs of SM are of type logic //
  ////////////////////////////////////////////
  logic C_Y_H, C_Y_L;     // state machine output indicating to sample and hold yaw values
  logic wrt;              // state machine output indicating to start transaction
  logic done;
  logic vld;

  //////////////////////////////////////////////////////////////
  // Declare any needed internal signals that connect blocks //
  ////////////////////////////////////////////////////////////
  wire signed [15:0] ptch_rt,roll_rt,yaw_rt;  // feeds inertial_integrator
  wire signed [15:0] ax,ay;                   // accel data to inertial_integrator
  logic [15:0] cmd;                           // command to send over SPI (connected to wt_data)
  logic [15:0] inert_data;                    // connects to the rd_data of SPI

  ////////////////////////////////////////////
  // Declare any needed internal registers //
  //////////////////////////////////////////
  // holding registers, timer, double flop of INT, ...
  logic INT_ff1, INT_ff2;     // intermediates for double flopping INT
  logic [15:0] timer;         // 16-bit timer used for intertial sensor reset sequence
  logic [7:0] yawL, yawH;     // store yaw values

  // double flop of INT
  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      INT_ff1 <= 1'b0;
      INT_ff2 <= 1'b0;
    end
    else begin
      INT_ff1 <= INT;
      INT_ff2 <= INT_ff1;
    end
  end

  // holding registers for rd_data into yawH and yawL
  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      yawH <= 8'h00;
    end
    else if (C_Y_H) begin
      yawH <= inert_data;
    end
  end
  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      yawL <= 8'h00;
    end
    else if (C_Y_L) begin
      yawL <= inert_data;
    end
  end

  // continuous packaging of yaw low and high bits
  assign yaw_rt = {yawH, yawL};

  /*
  16-bit timer used to control waiting of the intertial sensor
  to be ready after reset
  */
  always @(posedge clk, negedge rst_n) begin
    if (!rst_n) begin
      timer <= 0;
    end
    else begin
      timer <= timer + 1; 
    end
  end

  ///////////////////////////////////////
  // Create enumerated type for state //
  /////////////////////////////////////
  typedef enum reg [2:0] {INIT1, INIT2, INIT3, INIT4, READ_WAIT, READ_HIGH, READ_LOW, READ_DONE} state_t;
  state_t state,nstate;

  ////////////////////////
  // Infer State Flops //
  //////////////////////
  always_ff @(posedge clk, negedge rst_n) begin
    if (!rst_n)
      state <= INIT1;
    else
      state <= nstate;
  end
    
  //////////////////////////////////////
  // state transition & output logic //
  ////////////////////////////////////
  always_comb begin
    //////////////////////
    // Default outputs //
    ////////////////////
    cmd = 16'h0000;
    C_Y_H = 0;
    C_Y_L = 0;
    wrt = 0;
    vld = 0;
    nstate = state;      // default to avoid latches
    
    case (state)
      ////////////////////////////////////
      // INITIALIZING INTERTIAL SENSOR //
      //////////////////////////////////
      // init phase 1
      INIT1 : begin
        // enable interrupt upon data ready
        cmd = 16'h0D02;
        if (&timer) begin
          wrt = 1;
          nstate = INIT2;
        end
      end
      // init phase 2
      INIT2 : begin
        // setup gyro for 416Hz data raate, +/- 250 deg/sec range
        cmd = 16'h1160;
        if (done) begin
          wrt = 1;
          nstate = INIT3;
        end
      end
      // init phase 3
      INIT3 : begin
        // turn rounding on for gyro readings
        cmd = 16'h1440;
        if (done) begin
          wrt = 1;
          nstate = INIT4;
        end
      end
      INIT4: begin
        if (done) begin
          nstate = READ_WAIT;
        end
    end
      ////////////////////////////
      // READING GYRO YAW DATA //
      //////////////////////////
      READ_WAIT: begin
        if(INT_ff2) begin
          // read and store yawL from Gyro
          wrt = 1;
          cmd = 16'hA6xx;
          nstate = READ_LOW;
        end
      end
      READ_LOW : begin
        if (done) begin
          // let holding flop know yawL ready
          C_Y_L = 1;

          // read and store yawH from Gyro
          nstate = READ_HIGH;
          cmd = 16'hA7xx;
          wrt = 1;
        end
      end
      READ_DONE: begin
        nstate = READ_WAIT;
        // Indicate to the Inertial_integrator valid yaw_rt reading is ready (vld)
        vld = 1;
      end
      // using default as READ_HIGH
      default : begin
        if (done) begin
          // let holding flop know yawH ready
          C_Y_H = 1;
          nstate = READ_DONE;
        end
      end
    endcase
  end
  
  ////////////////////////////////////////////////////////////
  // Instantiate SPI monarch for Inertial Sensor interface //
  //////////////////////////////////////////////////////////
  SPI_mnrch iSPI( .clk(clk),.rst_n(rst_n),.SS_n(SS_n),.SCLK(SCLK), 
                  .MISO(MISO),.MOSI(MOSI),.wrt(wrt),.done(done),
                  .rd_data(inert_data),.wt_data(cmd));
  
  ////////////////////////////////////////////////////////////////////
  // Instantiate Angle Engine that takes in angular rate readings  //
  // and acceleration info and produces a heading reading         //
  /////////////////////////////////////////////////////////////////
  inertial_integrator #(FAST_SIM) iINT( .clk(clk), .rst_n(rst_n), .strt_cal(strt_cal),.vld(vld),
                                        .rdy(rdy),.cal_done(cal_done), .yaw_rt(yaw_rt),.moving(moving),
                                        .lftIR(lftIR),.rghtIR(rghtIR),.heading(heading));

endmodule
