
module MtrDrv(lft_spd, rght_spd, lftPWM1, lftPWM2, rghtPWM1, rghtPWM2, clk, rst_n);

    input clk,rst_n;		            // clock and active low asynch reset
    input [10:0]lft_spd;		        // signed left motor speed
    input [10:0]rght_spd;		        // signed right motor speed
    output logic lftPWM1, lftPWM2;	    // To power MOSFET's that drive left motor
    output logic rghtPWM1,rghtPWM2;     // to power MOSFET's that drive right motor

    /*
    Do the adding of left speed and right speed to a constant value of 11'h400 which
    adds 50% to the motor duty cycle. These two adder outputs will then be fed into the PWM11 module
    */
    logic [10:0] adder_lft, adder_rght;
    assign adder_lft = lft_spd + 11'h400;
    assign adder_rght = rght_spd + 11'h400;

    /*
    Feed the output of the above adders into PWM11, and connect left and right PWM values 1 and 2
    to the correct ports to get the PWM signal as well as negated PWM signal.
    */
    PWM11 PMW11_lft(.clk(clk),.rst_n(rst_n),.duty(adder_lft),.PWM_sig(lftPWM1),.PWM_sig_n(lftPWM2));
    PWM11 PMW11_rght(.clk(clk),.rst_n(rst_n),.duty(adder_rght),.PWM_sig(rghtPWM1),.PWM_sig_n(rghtPWM2));

endmodule