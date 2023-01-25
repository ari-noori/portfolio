`default_nettype none
/*
   Filename        : memory.v
   Description     : This module contains all components in the Memory stage of the 
                     processor.
*/
module memory (
   // Control signals
   input wire  HALT,       // HALT from DECODE
   input wire  MemWrite,   // Write enable from DECODE
   input wire  MemRead,    // Read enable from DECODE
   
   
   input wire  clk,  
   input wire  rst, 

   input wire  [15:0] dataAddress,  // Addr to read/write
   input wire  [15:0] writeData,    // Data to be written

   output wire  [15:0] readData,    // Data to be read from
   output wire  [15:0] ALUResult,   // Result from the ALU
   output wire  Stall_DM,

   output wire err

);

   assign ALUResult = dataAddress;
   assign err = 1'b0; // Nothing to error at this stage
   wire CacheHit, done_DM, err_DM;


   //////////////////
   // DATA MEMORY //
   ////////////////
   wire read;
   assign read = MemRead & ~MemWrite;
   mem_system #(.memtype(1)) dataMemory(
                      .DataOut(readData), 
                      .Done(done_DM), 
                      .Stall(Stall_DM), 
                      .CacheHit(CacheHit), 
                      .err(err_DM), 
                      .Addr(dataAddress), 
                      .DataIn(writeData), 
                      .Rd(read), 
                      .Wr(MemWrite), 
                      .createdump(HALT), 
                      .clk(clk), 
                      .rst(rst));

endmodule

`default_nettype wire