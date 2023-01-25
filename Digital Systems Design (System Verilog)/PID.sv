
module PID(error, clk, rst_n, err_vld, moving, frwrd, lft_spd, rght_spd);

    // PID signals
    input [11:0] error;                             // the input signed error term
    input clk,rst_n;		                        // clock and active low asynch reset
    input err_vld;			                        // the new error signal is valid and should be accumulated
    input moving;		                            // knight is moving so PID should be active
    input [9:0] frwrd;                              // summed with PID to form lft_spd, rght_spd
    output logic [10:0] lft_spd, rght_spd;          // these form the input to mtr_drv

    // P Term signals
    localparam P_COEFF = 5'h08;                     // proportional component for our PID
    localparam max_neg10 = 10'b1000000000;          // the most negative value we can represent with 10 bits
    localparam max_pos10 = 10'b0111111111;          // the most positive value we can represent with 10 bits
    logic signed [9:0] sat10;                       // an intermediate to store the 10 bit saturated result
    logic signed [13:0] P_term;                     // the signed P component of PID controller

    // I term signals
    logic [14:0]err_sat_extended;                   // sign extended err_sat 
    logic [14:0]accumulated;                        // accumulated value
    logic overflow;                                 // did overflow occur
    logic valid;                                    // is overflow and err_vld both valid
    logic [14:0]valid_out;                          // the correct (frozen if overflow occured) input to integrator
    logic [14:0]nxt_integrator;                     // input to FF
    logic [14:0]integrator;                         // output of FF
    logic [8:0]I_term;                              // the I_term for eventual use in PID control

    // D term signals
    logic [9:0] ff2_out, ff1_out;                   // used for double flopping error
    logic [9:0] D_diff;                             // the difference
    logic signed [6:0] sat7;                        // an intermediate to store the 7 bit saturated result
    localparam D_COEFF = 6'h0B;                     // proportional component for our PID
    logic [12:0]D_term;                             // derivative output

    /********************************** P TERM **********************************************************/
    /*
    Below is the main logic to do the saturation. If the top bit is 0, and rest are 1's, value is too 
    large so simply set to max positive. If the value is 1 followed by all 0's, then it is too negative
    so simply replace with the maximaly negative value. If not, simply copy the sign and bits 8 to 0.
    */
    assign sat10 = (~error[11] && |error[10:9]) ? max_pos10 : (error[11] && ~&error[10:9]) ? max_neg10 : {error[11], error[8:0]};

    logic [13:0]  sat10_ff, err_vld_ff;

    // now pipeline PID sum
    always_ff @(posedge clk) begin
        sat10_ff <= sat10;
        err_vld_ff <= err_vld;
    end

    // do the signed multiply with the P coefficient (forcing it to signed)
    assign P_term = sat10_ff * $signed(P_COEFF);
    

    /********************************** I TERM **********************************************************/
    // Sign extend the input err_sat
    assign err_sat_extended = {{5{sat10_ff[9]}}, sat10_ff}; 

    // adder accumulating the output of the integrator and sign extended input err_sat
    assign accumulated = err_sat_extended + integrator;

    // check if overflow occured (check if MSB of inputs are same as output)
    // 0 = no overflow
    // 1 = overflow occurred
    assign overflow = integrator[14] == err_sat_extended[14] && accumulated[14] != err_sat_extended[14];

    // AND of the err_vld signal and overflow to check if integrator should be run
    assign valid = !overflow && err_vld_ff;

    // freeze the integrator value if overflow occured from accumulator
    assign valid_out = valid ? accumulated : integrator;

    // do not correct error if Knight is not moving (unable to correct if not moving)
    assign nxt_integrator = moving ? valid_out : 15'h0000; 
    always_ff @(posedge clk, negedge rst_n)
        if (!rst_n)
        integrator <= 15'h0000;
        else
        integrator <= nxt_integrator;

    // assign the output I term to upper 9 bits of accumulator
    assign I_term = integrator[14:6];

    /********************************** D TERM **********************************************************/
    /*
    Here is always block for double flopping err_sat
    and reseting the values if active low reset is asserted.
    If err_vld is asserted we pass err_sat, 
    otherwise hold the value periously in the FF
    */
    always_ff @(posedge clk, negedge rst_n) 
        if (!rst_n) begin
            ff1_out <= 10'h000;
            ff2_out <= 10'h000;
        end
        else if (err_vld_ff) begin
            ff1_out <= sat10_ff;
            ff2_out <= ff1_out;
        end

    // do the actual subtraction to produce D term difference
    assign D_diff = sat10_ff - ff2_out;

    // saturate the value to 7-bits
    assign sat7[5:0] = D_diff[9] ? (&D_diff[9:6] ? D_diff[5:0] : 6'h00) : 
                                    (|D_diff[9:6] ? 6'h3F : D_diff[5:0]);
    assign sat7[6] = D_diff[9];
    // do the signed multiply with the D coefficient (forcing it to signed)
    assign D_term = sat7 * $signed(D_COEFF);

    /********************************** PID CREATION **********************************************************/
    // create the sign extended versions of I_term and D_term
    logic [13:0] D_TERM_SE, I_TERM_SE, PID_SUM;
    assign I_TERM_SE = {{5{I_term[8]}}, I_term};
    assign D_TERM_SE = {D_term[12], D_term};

    // construct PID sum from P + I + D
    assign PID_SUM = P_term + I_TERM_SE + D_TERM_SE;
    logic [13:0]  PID_sum_ff1;

    // now pipeline PID sum
    always_ff @(posedge clk) begin
        PID_sum_ff1 <= PID_SUM;
    end

    // create the zero extended frwrd term
    logic [10:0] FRWRD_ZE;
    assign FRWRD_ZE = {1'b0, frwrd};

    // construct the left and right sums by adding or subtracting frwrd
    logic [10:0] LFT_SPD_SUM, RGHT_SPD_SUM;
    assign LFT_SPD_SUM = PID_sum_ff1[13:3] + FRWRD_ZE;
    assign RGHT_SPD_SUM = FRWRD_ZE - PID_sum_ff1[13:3];

    // clear PID if not moving
    logic [10:0] lft_spd_in, rght_spd_in;
    assign lft_spd_in = moving ? LFT_SPD_SUM : 11'h000;
    assign rght_spd_in = moving ? RGHT_SPD_SUM : 11'h000;

    // perform the positive saturation for left and right speed
    assign lft_spd = ((PID_sum_ff1[13] == FRWRD_ZE[10]) && (PID_sum_ff1[13] != lft_spd_in[10])) ? 11'h3FF : lft_spd_in;
    assign rght_spd = ((PID_sum_ff1[13] != FRWRD_ZE[10]) && (PID_sum_ff1[13] == rght_spd_in[10])) ? 11'h3FF : rght_spd_in;

endmodule