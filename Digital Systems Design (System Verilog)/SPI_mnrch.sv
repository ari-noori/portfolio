
module SPI_mnrch(clk, rst_n, SS_n, SCLK, MOSI, MISO, wrt, wt_data, done, rd_data);

    input clk,rst_n;		            // 50MHz system clock and reset
    input MISO;                         // SPI protocol signals
    input wrt;                          // A high for 1 clock period would initiate a SPI transaction
    input [15:0] wt_data;               // Data (command) being sent to intertial sensor
    output logic SCLK;                  // SPI protocol signals
    output logic SS_n;                  // SPI protocol signals
    output logic MOSI;                  // SPI protocol signals
    output logic done;                  // Asserted when SPI transaction is complete. Should stay asserted till next wrt
    output logic [15:0]rd_data;         // Data from SPI serf. For intertial sensor we will only ever use [7:0]
    
    /**************************************
    STATE MACHINE PORTION
    This is the state machine controlling
    the transmission of data.
    **************************************/
    // state machine signals
    logic   done15,         // indicates when bit_cntr is full (15 cycles)
            smpl,           // indicates to sample MISO
            shft,           // indicates to shift MOSI
            ld_SCLK,        // loads SCLK with our "special" value
            init,           // init bit_cntr and output signals
            set_sigs,       // set the output signals to high
            rise_iminent,   // indicates rise is about to occur based on SCLK value
            fall_iminent;   // indicates fall is about to occur based on SCLK value
    logic [3:0]bit_cntr;    // count the bits shifted

    // state enum and declaration
    typedef enum reg[1:0] {IDLE, START, TRANSMIT, END} state_t;
    state_t state, nxt_state;

    ////////////////////////////
    // State maintaining FF  //
    //////////////////////////
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            state <= IDLE;
        else
            state <= nxt_state;
    end 
    
    //////////////////////////////
    // State maintaining Logic //
    ////////////////////////////
    always_comb begin
        nxt_state = state;      // default to avoid latches
        
        // default all outputs to prevent latches
        smpl = 0;
        shft = 0;
        ld_SCLK = 0;
        init = 0;
        set_sigs = 0;

        case (state)
            IDLE : begin
                /* 
                in IDLE state we wait until SS_n first goes low
                move into "front porch"  state
                */
                ld_SCLK = 1;
                if (wrt) begin
                    nxt_state = START;
                    init = 1;
                end
            end
            START : begin
                /*
                the "front porch" state where we wait for SCLK rise
                which we know will occur when sclk_div = 01111 without shifting
                */
                if (rise_iminent) begin 
                    // move into main working state
                    nxt_state = TRANSMIT;
                end
            end
            TRANSMIT : begin
                /* 
                In transmit phase, the main working state.
                
                Wait for rise to occur and sample when that happens. If at 
                that point we have done 15 cycles, we move into the ending 
                state (back porch) and set our output signals high.

                Else, if a fall is about to occur we shift instead.
                */
                if (rise_iminent) begin
                    smpl = 1;
                    if(done15) begin
                        nxt_state = END;
                    end
                end
                else if(fall_iminent) begin
                    shft = 1;
                end
            end
            END : begin
                /*
                the "back porch" where wee shift one last time (prevent fall
                of SCLK) so "C0" captured on SCLK rise is shiften into register
                */
                if(fall_iminent) begin
                    set_sigs = 1;
                    shft = 1;
                    ld_SCLK = 1;
                    nxt_state = IDLE;
                end   
            end
        endcase
    end

    /**************************************
    ALWAYS BLOCK FOR SCLK divider
    This block adds to SCLK once a clock 
    cycle, or if ld_SCLK is asserted we
    load SCLK with the "special" value
    used to determine rise/fall edges.
    **************************************/    
    logic [4:0]sclk_div;

    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            sclk_div <= 5'b10111;
        else if (ld_SCLK)
            sclk_div <= 5'b10111;
        else
            sclk_div <= sclk_div + 1;
    end

    // continuous assign to set SCLK
    assign SCLK = sclk_div[4];

    // rise iminent or fall iminent to take into sm since shft and smpl should come out of SM 
    assign rise_iminent = sclk_div == 5'b01111;     // if sclk_div = 01111 then a rise of SCLK is going to occur on the next clk edge
    assign fall_iminent = sclk_div == 5'b11111;     // if sclk_div = 11111 then a fall of SCLK is going to occur on the next clk edge

    /***********************************************
    ALWAYS BLOCK FOR SHIFT AND BIT COUNTER
    Here we simply count the number of shifts we
    have completed.
    ***********************************************/
    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            bit_cntr <= 4'b0000;
        else if (init)
            bit_cntr <= 4'b0000;
        else if (shft)
            bit_cntr <= bit_cntr + 1;
    end

    // Continuous assign to alert when bit_cntr is full and reached 15 cycles 
    assign done15 = &bit_cntr;

    /**************************************
    MISO and MOSI logic
    Here we sample MISO in, and then on
    shift we concatenate shft_reg upper 15
    bits with the sampled MISO value.
    **************************************/
    logic MISO_smpl;
    logic [15:0]shft_reg;

    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            MISO_smpl <= 1'b0;
        else if (smpl)
            MISO_smpl <= MISO;
        // state is held for logic, do not need else
    end
    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            shft_reg <= 16'h0000;
        else if (init)
            shft_reg <= wt_data;
        else if (shft)
            shft_reg <= {shft_reg[14:0], MISO_smpl};
    end

    // continuous assign to set next value of MOSI, the output
    assign MOSI = shft_reg[15];

    // continuous assign to rd_data make sure it equals shft_reg
    assign rd_data = shft_reg;

    /**************************************
    ALWAYS BLOCK TO GENERATE DONE and SS_n
    SR flop to generate the outputs based
    on signal recieved from state machine.
    **************************************/
    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n) begin
            SS_n <= 1'b1;
            done <= 1'b0;
        end
        else if (set_sigs) begin
            SS_n <= 1'b1;  // S input, set flop
            done <= 1'b1; 
        end
        else if (init) begin
            SS_n <= 1'b0;  // R input, reset flop
            done <= 1'b0;
        end
        // if neither S or R are high, maintain state
    end
endmodule