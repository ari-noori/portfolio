
module reset_synch(clk, rst_n, RST_n);

    input RST_n;	        // raw input from push button
    input clk;		        // clock, and we use negative edge
    output reg rst_n;           // out synchronized output which will form the global reset of our chip

    reg ff1;
    always @(negedge clk, negedge RST_n) begin
        if(!RST_n) begin
            ff1 <= 1'b0;
            rst_n <= 1'b0;
        end
        else begin
            ff1 <= 1'b1;
            rst_n <= ff1;
        end
    end
    
endmodule