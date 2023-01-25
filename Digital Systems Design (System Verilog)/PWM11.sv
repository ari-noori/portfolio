
module PWM11(clk, rst_n, duty, PWM_sig, PWM_sig_n);

input clk, rst_n;
input [10:0] duty; //Specifies duty cycle(unsigned 11-bit)

output logic PWM_sig, PWM_sig_n; //PWM signal out(glitch free)

logic PWM_sig_int;
logic [10:0] cnt;

assign PWM_sig_n = ~PWM_sig;

/*
Flip-flop for the counter. After each clock, the count will increment by 1.
It will automatically overflow when it hits 2048 since the number is 11 bits.
*/
always_ff @(posedge clk, negedge rst_n)
	if(!rst_n)
		cnt <= 11'h000;
	else
		cnt <= cnt + 1'b1;

/*
This intermediate signal represents the logic for checking if the 
count is less than the duty cycle. We only want to drive a single
bit to the flip flop for the duration of the duty cycle.
*/ 	
assign PWM_sig_int = (cnt < duty) ? 1'b1 : 1'b0;

/*
This is the flop for the PWM signal. It simply takes in the 
value from the intermediate signal which controls the duty cycle.
*/	
always_ff @(posedge clk, negedge rst_n)
	if(!rst_n)
		PWM_sig <= 1'b0;
	else
		PWM_sig <= PWM_sig_int;	
	

endmodule