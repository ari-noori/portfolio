`default_nettype none
/*
    The fourth pipeline register
*/
module MEMWB(
    input wire clk, rst, Stall_DM,

    input wire MEMWB_en,

    input wire [15:0] readData,
    input wire [15:0] ALUResult_MEM,
    input wire EXMEM_IDEX_LBI_sel,
    input wire EXMEM_IDEX_MemToReg,
    input wire EXMEM_IDEX_PCtoReg,
    input wire [15:0] EXMEM_IDEX_i8SE,
    input wire [15:0] EXMEM_IDEX_IFID_PC_inc,
    input wire [1:0] EXMEM_IDEX_RegDst,
    input wire EXMEM_IDEX_RegWrite,
    input wire [2:0] EXMEM_IDEX_writeRegSel,
    
    output wire [15:0] MEMWB_readData,
    output wire [15:0] MEMWB_ALUResult_MEM,
    output wire MEMWB_EXMEM_IDEX_LBI_sel,
    output wire MEMWB_EXMEM_IDEX_MemToReg,
    output wire MEMWB_EXMEM_IDEX_PCtoReg,
    output wire [15:0] MEMWB_EXMEM_IDEX_i8SE,
    output wire [15:0] MEMWB_EXMEM_IDEX_IFID_PC_inc,
    output wire [1:0] MEMWB_EXMEM_IDEX_RegDst,
    output wire MEMWB_EXMEM_IDEX_RegWrite,    // RegWrite for registers
    output wire [2:0] MEMWB_EXMEM_IDEX_writeRegSel,

    // Forwarding
    output wire [15:0] MEM_EX_fwd_data 
);

    assign MEM_EX_fwd_data = MEMWB_ALUResult_MEM;

   register16 #(.WIDTH(16)) MEMWB_readDat(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(readData), .out(MEMWB_readData));

   register16 #(.WIDTH(16)) MEMWB_ALUResult_M(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(ALUResult_MEM), .out(MEMWB_ALUResult_MEM));

   register16 #(.WIDTH(1)) MEMWB_EXMEM_IDEX_LBI_se(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_LBI_sel), .out(MEMWB_EXMEM_IDEX_LBI_sel));

   register16 #(.WIDTH(1)) MEMWB_EXMEM_IDEX_MemToRe(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_MemToReg), .out(MEMWB_EXMEM_IDEX_MemToReg));

   register16 #(.WIDTH(1)) MEMWB_EXMEM_IDEX_PCtoRe(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_PCtoReg), .out(MEMWB_EXMEM_IDEX_PCtoReg));

   register16 #(.WIDTH(16)) MEMWB_EXMEM_IDEX_i8S(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_i8SE), .out(MEMWB_EXMEM_IDEX_i8SE));

   register16 #(.WIDTH(16)) MEMWB_EXMEM_IDEX_IFID_PC_in(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_IFID_PC_inc), .out(MEMWB_EXMEM_IDEX_IFID_PC_inc));

    register16 #(.WIDTH(2)) MEMWB_EXMEM_IDEX_RegDs(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_RegDst), .out(MEMWB_EXMEM_IDEX_RegDst));

   register16 #(.WIDTH(1)) MEMWB_EXMEM_IDEX_RegWrit(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_RegWrite), .out(MEMWB_EXMEM_IDEX_RegWrite));

   register16 #(.WIDTH(3)) MEMWB_EXMEM_IDEX_writeRegSe(.en(~Stall_DM), .clk(clk), .rst(rst), 
   .in(EXMEM_IDEX_writeRegSel), .out(MEMWB_EXMEM_IDEX_writeRegSel));

endmodule

`default_nettype wire