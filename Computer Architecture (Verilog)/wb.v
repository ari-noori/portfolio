`default_nettype none
/*
   Filename        : wb.v
   Description     : This is the module for the overall Write Back stage of the processor.
*/
module wb (
   //Control signals
   input wire  LBI_sel, 
   input wire  MemToReg,
   input wire  PCtoReg,
 
   input wire  [15:0] readData,
   input wire  [15:0] i8SE,
   input wire  [15:0] ALUResult, 
   input wire  [15:0] PC_2, 

   output wire [15:0] writeData,
   output wire err
);

   assign writeData = PCtoReg  ? PC_2 :
                      MemToReg ? readData :
                                 ALUResult;
   
   assign err = 1'b0; // Nothing to error at this stage

endmodule

`default_nettype wire