`default_nettype none
/*
   Filename        : fetch.v
   Description     : This is the module for the overall fetch stage of the processor.
*/
module fetch (
   // Inputs
   input wire  HALT,       // HALT commes from DECODE Control unit
   input wire  [15:0] PC,  // New PC values comes from EXECUTE
   input wire  clk,  
   input wire  rst,
   input wire  stall, 

   // Pipelined PC select mux
   input wire take_branch_PC,
   input wire Stall_DM,

   // Outputs
   output wire  [15:0] instruction,   // 16 bit instruction from the Ins Memory
   output wire  [15:0] PC_inc,        // The PC that is incremented by 2 at this stage (or 0 if HALT is asserted)
   output wire  err
);

   assign err = 1'b0; //Nothing to error right now

   //Dummy wires for stuff that doesn't need to be connected
   wire dummy;

   
   wire stall_IM;
   wire CacheHit, Done, err_align_IM;

   ///////////////
   // PC BLOCK //
   /////////////
   wire [15:0] PC_out;
   wire [15:0] PC_in;
   wire [15:0] PC_2;
   wire [15:0] savedBranchPC;
   wire PC_EN;
   wire saved_take_branch_PC;

   wire [15:0] Addr_latched;
   assign PC_EN = 1'b1;

   assign PC_in = take_branch_PC ? PC : PC_2;


   //Instantiate a 16 bit register
   register16 PC_reg(.en(PC_EN),    // Disable PC on stall
                 .clk(clk), 
                 .rst(rst), 
                 .in(PC_in),        // New PC values comes from EXECUTE
                 .out(PC_out));     // The address of the next instruction


   /////////////////////////
   // INSTRUCTION MEMORY //
   ///////////////////////
   wire [15:0] instruction_IM;
   assign instruction = (~Done | rst | (Addr_latched != PC_out)) ? 16'h0800: instruction_IM;

   mem_system_IM IM(.DataOut(instruction_IM),// 16 bit instruction from the Ins Memory //mem_system #(0)
            .DataIn(16'h0800),               // Tied off to 0 because we are only loading data at the start
            .Addr(PC_out),                   // Addr of instruction comes from PC
            .Rd(1'b1),                       // Always read enabled
            .Wr(1'b0),                       // Write enable always DISABLED
            .createdump(HALT),               // DUMP memory when HALT signal is asserted
            .err(err_align_IM), 
            .Stall(stall_IM),
            .Done(Done),
            .CacheHit(CacheHit),
            .clk(clk), 
            .rst(rst),
            .Addr_latched(Addr_latched)
            );  

   //////////////////////
   // PC INCREMENT +2 //
   ////////////////////
   // The other additions to the PC (and select signals) happen in other stages.
   wire [15:0] inc_amount;
   assign inc_amount = (stall | ~Done | Stall_DM | (Addr_latched != PC_out))  ? 16'd0 : 16'd2;
   cla_16b PC_PLUS_2(.sum(PC_2),    // Output PC+2
                     .c_out(dummy), // No need for c_out
                     .a(PC_out),    // Operand 1 is PC
                     .b(inc_amount),  // Operand 2 is +2
                     .c_in(1'b0));  // No Cin

   // HALT Mux
   assign PC_inc = HALT ? PC_out : PC_2;

endmodule

`default_nettype wire