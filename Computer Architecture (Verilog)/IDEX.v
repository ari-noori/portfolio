`default_nettype none
/*
    The second pipeline register
*/
module IDEX(
    input wire clk, rst, stall,

    input wire IDEX_en,

    input wire [15:0] IFID_PC_inc,
    input wire [15:0] read1Data,
    input wire [15:0] read2Data,
    input wire RegWrite,
    input wire LBI_sel,
    input wire [2:0] ALUOp,
    input wire [2:0] ALUSrc,
    input wire c_in,
    input wire invA,
    input wire invB,
    input wire MemWrite,
    input wire MemRead,
    input wire MemToReg,
    input wire HALT,
    input wire PCtoReg,
    input wire SLBI,
    input wire BTR,
    input wire [2:0] ALUCondition,
    input wire SET,
    input wire JMP,
    input wire JALR_op,
    input wire BR,
    input wire [15:0] i8SE,   
    input wire [15:0] i5SE,
    input wire [15:0] i50E,
    input wire [15:0] i80E,
    input wire [15:0] d11SE,
    input wire [1:0] RegDst,
    input wire [2:0] writeRegSel,
    input wire [15:0] instruction,
    
    input wire branch_jump_op,

    input wire flush,
    input wire STU, 
    input wire Stall_DM,

    output wire [15:0] IDEX_IFID_PC_inc,
    output wire [15:0] IDEX_read1Data,
    output wire [15:0] IDEX_read2Data,
    output wire IDEX_RegWrite,
    output wire IDEX_LBI_sel,
    output wire [2:0] IDEX_ALUOp,
    output wire [2:0] IDEX_ALUSrc,
    output wire IDEX_c_in,
    output wire IDEX_invA,
    output wire IDEX_invB,
    output wire IDEX_MemWrite,
    output wire IDEX_MemRead,
    output wire IDEX_MemToReg,
    output wire IDEX_HALT,
    output wire IDEX_PCtoReg,
    output wire IDEX_SLBI,
    output wire IDEX_BTR,
    output wire [2:0] IDEX_ALUCondition,
    output wire IDEX_SET,
    output wire IDEX_JMP,
    output wire IDEX_JALR_op,
    output wire IDEX_BR,
    output wire [15:0] IDEX_i8SE,   
    output wire [15:0] IDEX_i5SE,
    output wire [15:0] IDEX_i50E,
    output wire [15:0] IDEX_i80E,
    output wire [15:0] IDEX_d11SE,

    output wire [1:0] IDEX_RegDst,
    output wire [2:0] IDEX_writeRegSel,

    // Forwarding
    output wire [15:0] IDEX_instruction,
    output wire IDEX_STU,

    output wire IDEX_branch_jump_op
    
);

    register16 #(.WIDTH(16)) IDEX_IFID_PC(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(IFID_PC_inc), .out(IDEX_IFID_PC_inc)); 

    register16 #(.WIDTH(16)) IDEX_read1(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(read1Data), .out(IDEX_read1Data));

    register16 #(.WIDTH(16)) IDEX_read2(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(read2Data), .out(IDEX_read2Data));  

    register16 #(.WIDTH(1)) IDEX_LBI_s(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(LBI_sel), .out(IDEX_LBI_sel));

    register16 #(.WIDTH(3)) IDEX_ALUO(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(ALUOp), .out(IDEX_ALUOp));

    register16 #(.WIDTH(3)) IDEX_ALUS(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(ALUSrc), .out(IDEX_ALUSrc));

    register16 #(.WIDTH(1)) IDEX_c_i(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(c_in), .out(IDEX_c_in));

    register16 #(.WIDTH(1)) IDEX_inA(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(invA), .out(IDEX_invA));

    register16 #(.WIDTH(1)) IDEX_inB(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(invB), .out(IDEX_invB)); 

    wire RegWrite_in;
    assign RegWrite_in = (stall | flush) ? 1'b0 : RegWrite;
    register16 #(.WIDTH(1)) IDEX_RegW(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(RegWrite_in), .out(IDEX_RegWrite));

    wire MemWrite_in;
    assign MemWrite_in = (stall | flush) ? 1'b0 : MemWrite;
    register16 #(.WIDTH(1)) IDEX_MemW(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(MemWrite_in), .out(IDEX_MemWrite)); 

    wire MemRead_in;
    assign MemRead_in = (stall | flush) ? 1'b0 : MemRead;
    register16 #(.WIDTH(1)) IDEX_MemR(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(MemRead_in), .out(IDEX_MemRead));

    register16 #(.WIDTH(1)) IDEX_MemToR(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(MemToReg), .out(IDEX_MemToReg));

    wire HALT_in;
    assign HALT_in = flush ? 1'b0 : HALT;
    register16 #(.WIDTH(1)) IDEX_HAL(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(HALT_in), .out(IDEX_HALT));

    register16 #(.WIDTH(1)) IDEX_PCtoR(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(PCtoReg), .out(IDEX_PCtoReg));

    register16 #(.WIDTH(1)) IDEX_SLB(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(SLBI), .out(IDEX_SLBI));

    register16 #(.WIDTH(1)) IDEX_BT(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(BTR), .out(IDEX_BTR));

    register16 #(.WIDTH(3)) IDEX_ALUCond(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(ALUCondition), .out(IDEX_ALUCondition));

    register16 #(.WIDTH(1)) IDEX_SE(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(SET), .out(IDEX_SET));

    wire JMP_in;
    assign JMP_in = flush ? 1'b0 : JMP;
    register16 #(.WIDTH(1)) IDEX_JM(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(JMP_in), .out(IDEX_JMP));

    wire JALR_op_in;
    assign JALR_op_in = flush ? 1'b0 : JALR_op;
    register16 #(.WIDTH(1)) IDEX_JALR_o(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(JALR_op_in), .out(IDEX_JALR_op));

    wire BR_in;
    assign BR_in = flush ? 1'b0 : BR;
    register16 #(.WIDTH(1)) IDEX_B(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(BR_in), .out(IDEX_BR));

    register16 #(.WIDTH(16)) IDEX_i8S(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(i8SE), .out(IDEX_i8SE));

    register16 #(.WIDTH(16)) IDEX_i5S(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(i5SE), .out(IDEX_i5SE));

    register16 #(.WIDTH(16)) IDEX_i50(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(i50E), .out(IDEX_i50E));

    register16 #(.WIDTH(16)) IDEX_i80(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(i80E), .out(IDEX_i80E));

    register16 #(.WIDTH(16)) IDEX_d11S(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(d11SE), .out(IDEX_d11SE));

    register16 #(.WIDTH(2)) IDEX_RegDs(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(RegDst), .out(IDEX_RegDst));

    register16 #(.WIDTH(3)) IDEX_writeRegSe(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(writeRegSel), .out(IDEX_writeRegSel));

    wire[15:0] instruction_in;
    assign instruction_in = (stall | flush) ? 16'h0800 : instruction;
    register16 #(.WIDTH(16)) IDEX_instructio(.en(~Stall_DM), .clk(clk), .rst(rst), 
    .in(instruction_in), .out(IDEX_instruction));

    register16 #(.WIDTH(1)) IDEX_branch_jump_o(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(branch_jump_op), .out(IDEX_branch_jump_op));

    wire STU_in;
    assign STU_in = flush ? 1'b0 : STU;
    register16 #(.WIDTH(1)) IDEX_STYou(.en(~Stall_DM), .clk(clk), .rst(rst | flush | (stall & ~Stall_DM)), 
    .in(STU), .out(IDEX_STU));

endmodule

`default_nettype wire