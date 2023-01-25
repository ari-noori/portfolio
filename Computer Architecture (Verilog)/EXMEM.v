`default_nettype none
/*
    The third pipeline register
*/
module EXMEM(
    input wire clk, rst,

    input wire EXMEM_en,

    input wire [15:0] newPC,
    input wire [15:0] read2DataOut,
    input wire [15:0] ALUResult,
    input wire IDEX_HALT, 
    input wire IDEX_MemWrite,
    input wire IDEX_MemRead,
    input wire IDEX_LBI_sel,
    input wire IDEX_MemToReg,
    input wire IDEX_PCtoReg,
    input wire [15:0] IDEX_i8SE,
    input wire [15:0] IDEX_IFID_PC_inc,

    input wire [1:0] IDEX_RegDst,
    input wire IDEX_RegWrite,
    input wire [2:0] IDEX_writeRegSel,

    input wire IDEX_branch_jump_op,

    input wire IDEX_STU,
    input wire Stall_DM,

    output wire [15:0] EXMEM_newPC,
    output wire [15:0] EXMEM_read2DataOut,
    output wire [15:0] EXMEM_ALUResult,
    output wire EXMEM_IDEX_HALT, 
    output wire EXMEM_IDEX_MemWrite,
    output wire EXMEM_IDEX_MemRead,
    output wire EXMEM_IDEX_LBI_sel,
    output wire EXMEM_IDEX_MemToReg,
    output wire EXMEM_IDEX_PCtoReg,
    output wire [15:0] EXMEM_IDEX_i8SE,
    output wire [15:0] EXMEM_IDEX_IFID_PC_inc,
    output wire [1:0] EXMEM_IDEX_RegDst,
    output wire EXMEM_IDEX_RegWrite,
    output wire [2:0] EXMEM_IDEX_writeRegSel,

    output wire EXMEM_IDEX_branch_jump_op,

    // Forwarding
    output wire [15:0] EX_EX_fwd_data,
    output wire EXMEM_IDEX_STU
);

    assign EX_EX_fwd_data = EXMEM_ALUResult; // Is this supposed to be input or output of reg signal?

   register16 #(.WIDTH(16)) EXMEM_newP(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(newPC), .out(EXMEM_newPC));

   register16 #(.WIDTH(16)) EXMEM_read2DataO(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(read2DataOut), .out(EXMEM_read2DataOut));

   register16 #(.WIDTH(16)) EXMEM_ALURes(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(ALUResult), .out(EXMEM_ALUResult));

   register16 #(.WIDTH(1)) EXMEM_IDEX_HAL(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_HALT), .out(EXMEM_IDEX_HALT));

   register16 #(.WIDTH(1)) EXMEM_IDEX_MemWrit(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_MemWrite), .out(EXMEM_IDEX_MemWrite));

   register16 #(.WIDTH(1)) EXMEM_IDEX_MemRea(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_MemRead), .out(EXMEM_IDEX_MemRead));

   register16 #(.WIDTH(1)) EXMEM_IDEX_LBI_se(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_LBI_sel), .out(EXMEM_IDEX_LBI_sel));

   register16 #(.WIDTH(1)) EXMEM_IDEX_MemToR(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_MemToReg), .out(EXMEM_IDEX_MemToReg));

   register16 #(.WIDTH(1)) EXMEM_IDEX_PCtoRe(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_PCtoReg), .out(EXMEM_IDEX_PCtoReg));

   register16 #(.WIDTH(16)) EXMEM_IDEX_i8S(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_i8SE), .out(EXMEM_IDEX_i8SE));

   register16 #(.WIDTH(16)) EXMEM_IDEX_IFID_PC_in(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_IFID_PC_inc), .out(EXMEM_IDEX_IFID_PC_inc));

   register16 #(.WIDTH(2)) EXMEM_IDEX_RegDs(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_RegDst), .out(EXMEM_IDEX_RegDst));

   register16 #(.WIDTH(1)) EXMEM_IDEX_RegWrit(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_RegWrite), .out(EXMEM_IDEX_RegWrite));

   register16 #(.WIDTH(3)) EXMEM_IDEX_writeRegSe(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_writeRegSel), .out(EXMEM_IDEX_writeRegSel));

   register16 #(.WIDTH(1)) EXMEM_IDEX_branch_jump_o(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_branch_jump_op), .out(EXMEM_IDEX_branch_jump_op));

   register16 #(.WIDTH(1)) EXMEM_IDEX_STyou(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(IDEX_STU), .out(EXMEM_IDEX_STU));

endmodule

`default_nettype wire