`default_nettype none
/*
    The first pipeline register
*/
module IFID(
    input wire clk, rst, flush,

    input wire IFID_en,
    input wire [15:0] instruction,   // 16 bit instruction from the Ins Memory
    input wire [15:0] PC_inc,        // The PC that is incremented by 2 at this stage
    input wire Stall_DM,
    input wire stall,

    output wire [15:0] IFID_instruction,   // 16 bit instruction from the Ins Memory
    output wire [15:0] IFID_PC_inc        // The PC that is incremented by 2 at this stage
);

    wire [15:0] ins;
    assign ins = (rst | flush) ? 16'h0800 : instruction; // insert NOP on flush or rst

   register16 #(.WIDTH(16)) IFID_ins(.en(~Stall_DM & ~stall), .clk(clk), .rst(1'b0), 
   .in(ins), .out(IFID_instruction) );   

   register16 #(.WIDTH(16)) IFID_PC(.en(~Stall_DM & ~stall), .clk(clk), .rst(rst), 
   .in(PC_inc), .out(IFID_PC_inc));

endmodule

`default_nettype wire