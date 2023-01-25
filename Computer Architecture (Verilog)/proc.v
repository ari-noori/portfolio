`default_nettype none
/*
   This module combines all pipeline stages.
*/
module proc (
   err, 
   clk, rst
   );

   input wire clk;
   input wire rst;

   output wire err;

   //////////////////////
   // ERROR DETECTION //
   ////////////////////
   // Error wires from each stage
   wire err_FETCH, err_DECODE, err_EXECUTE, err_MEMORY, err_WB;
   // Or error signals from each module
   assign err = err_FETCH | err_DECODE | err_EXECUTE | err_MEMORY | err_WB;

   /////////////////////////
   // FETCH Output Wires //
   ///////////////////////
   wire [15:0] instruction;   // 16 bit instruction from the Ins Memory
   wire [15:0] PC_inc;        // The PC that is incremented by 2 at this stage

   //////////////////////////
   // DECODE Output Wires //
   ////////////////////////
   wire [15:0] read1Data;  // Output from read operation
   wire [15:0] read2Data;  // Output from read operation

   wire [2:0] writeRegSel;

   // Control unit outputs
   wire RegWrite;    // RegWrite for registers
   wire LBI_sel;     // Select for LBI mux
   wire [2:0] ALUOp; // Opcode sent to the ALU
   wire [2:0] ALUSrc;// Select for ALU Source mux
   wire c_in;        // Carry in to the ALU (for subtraction)
   wire invA;        // Invert ALU input A
   wire invB;        // Invert ALU input B
   wire MemWrite;    // Write EN for data memory
   wire MemRead;     // Read enable for data memory
   wire MemToReg;    // Write memory to registers
   wire HALT;        // HALT operation
   wire PCtoReg;     // Select PC+2 to register
   wire SLBI;        // SLBI Operation
   wire BTR;
   wire [2:0] ALUCondition;
   wire SET;
   wire JMP;         // Indicates a J or JAL instruction (NOT JR or JALR)
   wire JALR_op;     // Indicates a JR or JALR
   wire BR;          // Indicates any branch instruction 
   wire STU;
   // Sign extend outputs
   wire [15:0] i8SE;
   wire [15:0] i5SE;
   wire [15:0] i50E;
   wire [15:0] i80E;
   wire [15:0] d11SE;
   // Extra Pipeline outputs
   wire [1:0] RegDst;  // Destination reg select signal that is to be sent thru pipeline

   wire branch_jump_op;

   ///////////////////////////
   // EXECUTE Output Wires //
   /////////////////////////
   wire zero;  // This might not do anything right now
   wire [15:0] newPC;
   wire [15:0] read2DataOut;
   wire [15:0] ALUResult;
   wire take_branch_PC;
   //////////////////////////
   // MEMORY Output Wires //
   ////////////////////////
   wire [15:0] readData;      // Data to be read from
   wire [15:0] ALUResult_MEM; // Result from the ALU
   wire Stall_DM;

   //////////////////////
   // WB Output Wires //
   ////////////////////
   wire [15:0] writeData;
   
   ///////////////////////
   // HAZARD DETECTION // 
   /////////////////////
   wire [2:0] IFID_Rs, IFID_Rt;
   wire stall_load;
   wire stall_branch_jump;
   wire stall;
   wire flush;
   

   ///////////////////////
   // FORWARDING WIRES //
   /////////////////////
   wire [15:0] EX_EX_fwd_data;
   wire [15:0] MEM_EX_fwd_data;


   /////////////////////////////////////////////////////////////////////////////
   //                            PIPELINE REGISTERS                           //
   /////////////////////////////////////////////////////////////////////////////

   //////////////////////////////
   // IF/ID Pipeline Register //
   ////////////////////////////
   wire IFID_en;  // Enable signal for pipeline register
   assign IFID_en = ~stall;

   wire [15:0] IFID_instruction;   // 16 bit instruction from the Ins Memory
   wire [15:0] IFID_PC_inc;        // The PC that is incremented by 2 at this stage

   IFID iIFID(
              .clk(clk), .rst(rst), .flush(flush),
              .IFID_en(IFID_en), .instruction(instruction), .PC_inc(PC_inc),
              .Stall_DM(Stall_DM),
              .stall(stall),
              .IFID_instruction(IFID_instruction), .IFID_PC_inc(IFID_PC_inc)
             );

   //////////////////////////////
   // ID/EX Pipeline Register //
   ////////////////////////////
   wire IDEX_en;  // Enable signal for pipeline register
   assign IDEX_en = 1'b1;

   wire [15:0] IDEX_IFID_PC_inc; // PC_inc from IF/ID register
   wire [15:0] IDEX_read1Data;  // Output from read operation
   wire [15:0] IDEX_read2Data;  // Output from read operation
   // Control unit outputs
   wire IDEX_RegWrite;    // RegWrite for registers
   wire IDEX_LBI_sel;     // Select for LBI mux
   wire [2:0] IDEX_ALUOp; // Opcode sent to the ALU
   wire [2:0] IDEX_ALUSrc;// Select for ALU Source mux
   wire IDEX_c_in;        // Carry in to the ALU (for subtraction)
   wire IDEX_invA;        // Invert ALU input A
   wire IDEX_invB;        // Invert ALU input B
   wire IDEX_MemWrite;    // Write EN for data memory
   wire IDEX_MemRead;     // Read enable for data memory
   wire IDEX_MemToReg;    // Write memory to registers
   wire IDEX_HALT;        // HALT operation
   wire IDEX_PCtoReg;     // Select PC+2 to register
   wire IDEX_SLBI;        // SLBI Operation
   wire IDEX_BTR;
   wire [2:0] IDEX_ALUCondition;
   wire IDEX_SET;
   wire IDEX_JMP;         // Indicates a J or JAL instruction (NOT JR or JALR)
   wire IDEX_JALR_op;     // Indicates a JR or JALR
   wire IDEX_BR;          // Indicates any branch instruction 
   // Sign extend outputs
   wire [15:0] IDEX_i8SE;
   wire [15:0] IDEX_i5SE;
   wire [15:0] IDEX_i50E;
   wire [15:0] IDEX_i80E;
   wire [15:0] IDEX_d11SE;

   wire [1:0] IDEX_RegDst;
   wire [2:0] IDEX_writeRegSel;

   wire [15:0] instruction_decode;
   wire [15:0] IDEX_instruction;
   wire IDEX_branch_jump_op;
   wire IDEX_STU;

   IDEX iIDEX(
               .clk(clk), .rst(rst), .stall(stall),

               .IDEX_en(IDEX_en),

               .IFID_PC_inc(IFID_PC_inc),
               .read1Data(read1Data),
               .read2Data(read2Data),
               .RegWrite(RegWrite),
               .LBI_sel(LBI_sel),
               .ALUOp(ALUOp),
               .ALUSrc(ALUSrc),
               .c_in(c_in),
               .invA(invA),
               .invB(invB),
               .MemWrite(MemWrite),
               .MemRead(MemRead),
               .MemToReg(MemToReg),
               .HALT(HALT),
               .PCtoReg(PCtoReg),
               .SLBI(SLBI),
               .BTR(BTR),
               .ALUCondition(ALUCondition),
               .SET(SET),
               .JMP(JMP),
               .JALR_op(JALR_op),
               .BR(BR),
               .i8SE(i8SE),   
               .i5SE(i5SE),
               .i50E(i50E),
               .i80E(i80E),
               .d11SE(d11SE),
               .RegDst(RegDst),
               .writeRegSel(writeRegSel),
               .instruction(instruction_decode),
               .branch_jump_op(branch_jump_op),
               .flush(flush),
               .STU(STU),
               .Stall_DM(Stall_DM),

               .IDEX_IFID_PC_inc(IDEX_IFID_PC_inc),
               .IDEX_read1Data(IDEX_read1Data),
               .IDEX_read2Data(IDEX_read2Data),
               .IDEX_RegWrite(IDEX_RegWrite),
               .IDEX_LBI_sel(IDEX_LBI_sel),
               .IDEX_ALUOp(IDEX_ALUOp),
               .IDEX_ALUSrc(IDEX_ALUSrc),
               .IDEX_c_in(IDEX_c_in),
               .IDEX_invA(IDEX_invA),
               .IDEX_invB(IDEX_invB),
               .IDEX_MemWrite(IDEX_MemWrite),
               .IDEX_MemRead(IDEX_MemRead),
               .IDEX_MemToReg(IDEX_MemToReg),
               .IDEX_HALT(IDEX_HALT),
               .IDEX_PCtoReg(IDEX_PCtoReg),
               .IDEX_SLBI(IDEX_SLBI),
               .IDEX_BTR(IDEX_BTR),
               .IDEX_ALUCondition(IDEX_ALUCondition),
               .IDEX_SET(IDEX_SET),
               .IDEX_JMP(IDEX_JMP),
               .IDEX_JALR_op(IDEX_JALR_op),
               .IDEX_BR(IDEX_BR),
               .IDEX_i8SE(IDEX_i8SE),   
               .IDEX_i5SE(IDEX_i5SE),
               .IDEX_i50E(IDEX_i50E),
               .IDEX_i80E(IDEX_i80E),
               .IDEX_d11SE(IDEX_d11SE),
               .IDEX_RegDst(IDEX_RegDst),
               .IDEX_writeRegSel(IDEX_writeRegSel),
               .IDEX_instruction(IDEX_instruction),
               .IDEX_branch_jump_op(IDEX_branch_jump_op),
               .IDEX_STU(IDEX_STU)
   );
   
   ///////////////////////////////
   // EX/MEM Pipeline Register //
   /////////////////////////////
   wire EXMEM_en;  // Enable signal for pipeline register
   assign EXMEM_en = 1'b1;

   wire EXMEM_zero;  // This might not do anything right now
   wire [15:0] EXMEM_newPC;
   wire [15:0] EXMEM_read2DataOut;
   wire [15:0] EXMEM_ALUResult;
   wire EXMEM_IDEX_HALT;        // HALT operation
   wire EXMEM_IDEX_MemWrite;    // Write EN for data memory
   wire EXMEM_IDEX_MemRead;     // Read enable for data memory
   wire EXMEM_IDEX_LBI_sel;     // Select for LBI mux
   wire EXMEM_IDEX_MemToReg;    // Write memory to registers
   wire EXMEM_IDEX_PCtoReg;     // Select PC+2 to register
   wire [15:0] EXMEM_IDEX_i8SE;
   wire [15:0] EXMEM_IDEX_IFID_PC_inc; // PC_inc from IF/ID register

   wire [1:0] EXMEM_IDEX_RegDst;
   wire EXMEM_IDEX_RegWrite;    // RegWrite for registers
   wire [2:0] EXMEM_IDEX_writeRegSel;
   wire EXMEM_IDEX_STU;

   wire EXMEM_IDEX_branch_jump_op;
   EXMEM iEXMEM(
    .clk(clk), .rst(rst),

    .EXMEM_en(EXMEM_en),

    .newPC(newPC),
    .read2DataOut(read2DataOut),
    .ALUResult(ALUResult),
    .IDEX_HALT(IDEX_HALT), 
    .IDEX_MemWrite(IDEX_MemWrite),
    .IDEX_MemRead(IDEX_MemRead),
    .IDEX_LBI_sel(IDEX_LBI_sel),
    .IDEX_MemToReg(IDEX_MemToReg),
    .IDEX_PCtoReg(IDEX_PCtoReg),
    .IDEX_i8SE(IDEX_i8SE),
    .IDEX_IFID_PC_inc(IDEX_IFID_PC_inc),
    .IDEX_RegDst(IDEX_RegDst),
    .IDEX_RegWrite(IDEX_RegWrite),
    .IDEX_writeRegSel(IDEX_writeRegSel),
    .IDEX_branch_jump_op(IDEX_branch_jump_op),
    .IDEX_STU(IDEX_STU),
    .Stall_DM(Stall_DM),

    .EXMEM_newPC(EXMEM_newPC),
    .EXMEM_read2DataOut(EXMEM_read2DataOut),
    .EXMEM_ALUResult(EXMEM_ALUResult),
    .EXMEM_IDEX_HALT(EXMEM_IDEX_HALT), 
    .EXMEM_IDEX_MemWrite(EXMEM_IDEX_MemWrite),
    .EXMEM_IDEX_MemRead(EXMEM_IDEX_MemRead),
    .EXMEM_IDEX_LBI_sel(EXMEM_IDEX_LBI_sel),
    .EXMEM_IDEX_MemToReg(EXMEM_IDEX_MemToReg),
    .EXMEM_IDEX_PCtoReg(EXMEM_IDEX_PCtoReg),
    .EXMEM_IDEX_i8SE(EXMEM_IDEX_i8SE),
    .EXMEM_IDEX_IFID_PC_inc(EXMEM_IDEX_IFID_PC_inc),
    .EXMEM_IDEX_RegDst(EXMEM_IDEX_RegDst),
    .EXMEM_IDEX_RegWrite(EXMEM_IDEX_RegWrite),
    .EXMEM_IDEX_writeRegSel(EXMEM_IDEX_writeRegSel),
    .EX_EX_fwd_data(EX_EX_fwd_data),
    .EXMEM_IDEX_branch_jump_op(EXMEM_IDEX_branch_jump_op),
    .EXMEM_IDEX_STU(EXMEM_IDEX_STU)
   );

   ///////////////////////////////
   // MEM/WB Pipeline Register //
   /////////////////////////////
   wire MEMWB_en;  // Enable signal for pipeline register
   assign MEMWB_en = 1'b1;
   
   wire [15:0] MEMWB_readData;      // Data to be read from
   wire [15:0] MEMWB_ALUResult_MEM; // Result from the ALU
   wire MEMWB_EXMEM_IDEX_LBI_sel;     // Select for LBI mux
   wire MEMWB_EXMEM_IDEX_MemToReg;    // Write memory to registers
   wire MEMWB_EXMEM_IDEX_PCtoReg;     // Select PC+2 to register
   wire [15:0] MEMWB_EXMEM_IDEX_i8SE;
   wire [15:0] MEMWB_EXMEM_IDEX_IFID_PC_inc; // PC_inc from IF/ID register

   wire [1:0] MEMWB_EXMEM_IDEX_RegDst;
   wire MEMWB_EXMEM_IDEX_RegWrite;    // RegWrite for registers
   wire [2:0] MEMWB_EXMEM_IDEX_writeRegSel;

   MEMWB iMEMWB(
    .clk(clk), .rst(rst),

    .MEMWB_en(MEMWB_en),

    .readData(readData),
    .ALUResult_MEM(ALUResult_MEM),
    .EXMEM_IDEX_LBI_sel(EXMEM_IDEX_LBI_sel),
    .EXMEM_IDEX_MemToReg(EXMEM_IDEX_MemToReg),
    .EXMEM_IDEX_PCtoReg(EXMEM_IDEX_PCtoReg),
    .EXMEM_IDEX_i8SE(EXMEM_IDEX_i8SE),
    .EXMEM_IDEX_IFID_PC_inc(EXMEM_IDEX_IFID_PC_inc),
    .EXMEM_IDEX_RegDst(EXMEM_IDEX_RegDst),
    .EXMEM_IDEX_RegWrite(EXMEM_IDEX_RegWrite),
    .EXMEM_IDEX_writeRegSel(EXMEM_IDEX_writeRegSel),
    .Stall_DM(Stall_DM),

    .MEMWB_readData(MEMWB_readData),
    .MEMWB_ALUResult_MEM(MEMWB_ALUResult_MEM),
    .MEMWB_EXMEM_IDEX_LBI_sel(MEMWB_EXMEM_IDEX_LBI_sel),
    .MEMWB_EXMEM_IDEX_MemToReg(MEMWB_EXMEM_IDEX_MemToReg),
    .MEMWB_EXMEM_IDEX_PCtoReg(MEMWB_EXMEM_IDEX_PCtoReg),
    .MEMWB_EXMEM_IDEX_i8SE(MEMWB_EXMEM_IDEX_i8SE),
    .MEMWB_EXMEM_IDEX_IFID_PC_inc(MEMWB_EXMEM_IDEX_IFID_PC_inc),
    .MEMWB_EXMEM_IDEX_RegDst(MEMWB_EXMEM_IDEX_RegDst),
    .MEMWB_EXMEM_IDEX_RegWrite(MEMWB_EXMEM_IDEX_RegWrite),
    .MEMWB_EXMEM_IDEX_writeRegSel(MEMWB_EXMEM_IDEX_writeRegSel),

    .MEM_EX_fwd_data(MEM_EX_fwd_data)
   );
   


   /////////////////////////////////////////////////////////////////////////////
   //                               STAGES                                    //
   /////////////////////////////////////////////////////////////////////////////

   ////////////
   // FETCH //
   //////////
   fetch fetch0(
   // Inputs
   .HALT(HALT),      // HALT commes from DECODE Control unit
   .PC(newPC),       // New PC values comes from EXECUTE
   .clk(clk),  
   .rst(rst),
   .take_branch_PC(take_branch_PC),
   .stall(stall),
   .Stall_DM(Stall_DM),

   // Outputs
   .instruction(instruction),    // 16 bit instruction from the Ins Memory
   .PC_inc(PC_inc),              // The PC that is incremented by 2 at this stage
   .err(err_FETCH)
   );

   /////////////
   // DECODE //
   ///////////
   decode decode0(
   // Inputs
   .instruction(IFID_instruction),    // 16 bit instruction from FETCH
   .writeData(writeData),        // Data to be written to the registers from WB    
   .clk(clk), 
   .rst(rst),
   .stall(stall),
   //.RegDstWB(MEMWB_EXMEM_IDEX_RegDst),
   .RegWriteWB(MEMWB_EXMEM_IDEX_RegWrite),
   .writeRegSelWB(MEMWB_EXMEM_IDEX_writeRegSel),

   // Outputs
   .read1Data(read1Data),     // Output from read operation
   .read2Data(read2Data),     // Output from read operation

   .writeRegSel(writeRegSel),

   // Control unit outputs
   .RegWrite(RegWrite),       // RegWrite for registers
   .LBI_sel(LBI_sel),         // Select for LBI mux
   .ALUOp(ALUOp),             // Opcode sent to the ALU
   .ALUSrc(ALUSrc),           // Select for ALU Source mux
   .c_in(c_in),               // Carry in to the ALU (for subtraction)
   .invA(invA),               // Invert ALU input A
   .invB(invB),               // Invert ALU input B
   .MemWrite(MemWrite),       // Write EN for data memory
   .MemRead(MemRead),         // Read enable for data memory
   .MemToReg(MemToReg),       // Write memory to registers
   .HALT(HALT),               // HALT operation
   .PCtoReg(PCtoReg),         // Select PC+2 to register
   .SLBI(SLBI),               // SLBI Operation
   .BTR(BTR),
   .ALUCondition(ALUCondition), // Selects the condition to the ALU: 0 for checking (<), 1 for checking (<=)
   .SET(SET),
   .JMP(JMP),
   .JALR_op(JALR_op),
   .BR(BR), 
   .STU_out(STU),

   // Extra pipeline outputs
   //.RegDst(RegDst),

   // Sign extend outputs
   .i8SE(i8SE),
   .i5SE(i5SE),
   .i50E(i50E),
   .i80E(i80E),
   .d11SE(d11SE),

   // Forwarding
   .instruction_out(instruction_decode),

   .branch_jump_op(branch_jump_op),

   .err(err_DECODE)
   );


   //////////////
   // EXECUTE //
   ////////////
   execute execute0(
      .read1Data(IDEX_read1Data),  // Register 1 from DECODE
      .read2Data(IDEX_read2Data),  // Register 2 from DECODE
      .i80E(IDEX_i80E),            // Zero extended 8-immediate from DECODE
      .i5SE(IDEX_i5SE),            // Sign extended 5-immediate from DECODE
      .i50E(IDEX_i50E),            // Zero extended 5-immediate from DECODE
      .d11SE(IDEX_d11SE),
      .i8SE(IDEX_i8SE),
      .PC(IDEX_IFID_PC_inc),            // PC from FETCH
      .EXMEM_IDEX_MemWrite(EXMEM_IDEX_MemWrite),
      .EXMEM_IDEX_STU(EXMEM_IDEX_STU),

      // Forwarding
      .EX_EX_fwd_data(EX_EX_fwd_data),
      .EXMEM_IDEX_writeRegSel(EXMEM_IDEX_writeRegSel),
      //.IDEX_writeRegSel(IDEX_writeRegSel),
      .MEMWB_EXMEM_IDEX_RegWrite(MEMWB_EXMEM_IDEX_RegWrite),
      .IDEX_instruction(IDEX_instruction),
      .EXMEM_IDEX_RegWrite(EXMEM_IDEX_RegWrite),
      .EXMEM_IDEX_MemRead(EXMEM_IDEX_MemRead),
      .MEM_EX_fwd_data(writeData),
      .MEMWB_EXMEM_IDEX_writeRegSel(MEMWB_EXMEM_IDEX_writeRegSel),


      // Control signals
      .ALUOp(IDEX_ALUOp),          // ALU Opcode from DECODE
      .SLBI(IDEX_SLBI),            // Select from DECODE
      .ALUSrc(IDEX_ALUSrc),        // Select from DECODE
      .c_in(IDEX_c_in),            // ALU c_in from DECODE
      .invA(IDEX_invA),            // Invert ALU input A
      .invB(IDEX_invB),            // Invert ALU input B
      .BTR(IDEX_BTR),
      .condition(IDEX_ALUCondition),
      .SET(IDEX_SET),
      .JMP(IDEX_JMP),
      .JALR_op(IDEX_JALR_op),
      .BR(IDEX_BR), 
      .LBI_sel(IDEX_LBI_sel),

      // Outputs
      .zero(zero),
      .newPC(newPC),
      .read2DataOut(read2DataOut),
      .ALUResult(ALUResult),
      .take_branch_PC(take_branch_PC),
      .err(err_EXECUTE)
   );

   /////////////
   // MEMORY //
   ///////////
   memory memory0(
      // Control signals
      .HALT(EXMEM_IDEX_HALT),            // HALT from DECODE
      .MemWrite(EXMEM_IDEX_MemWrite),    // Write enable from DECODE
      .MemRead(EXMEM_IDEX_MemRead),      // Read enable from DECODE
      .clk(clk),  
      .rst(rst), 

      .dataAddress(EXMEM_ALUResult),   // Addr to read/write
      .writeData(EXMEM_read2DataOut),  // Data to be written THIS IS PROBABLY WRONG

      .readData(readData),       // Data to be read from
      .ALUResult(ALUResult_MEM), // Result from the ALU
      .Stall_DM(Stall_DM),

      .err(err_MEMORY)
   );

   /////////
   // WB //
   ///////
   wb wb0(
      //Control signals
      .LBI_sel(MEMWB_EXMEM_IDEX_LBI_sel), 
      .MemToReg(MEMWB_EXMEM_IDEX_MemToReg),
      .PCtoReg(MEMWB_EXMEM_IDEX_PCtoReg),
   
      .readData(MEMWB_readData),
      .i8SE(MEMWB_EXMEM_IDEX_i8SE),
      .ALUResult(MEMWB_ALUResult_MEM), 
      .PC_2(MEMWB_EXMEM_IDEX_IFID_PC_inc), 

      .writeData(writeData),
      .err(err_WB)
   );

   ////////////////////////////
   // HAZARD DETECTION UNIT //
   //////////////////////////
   assign IFID_Rs = IFID_instruction[10:8];
   assign IFID_Rt = IFID_instruction[7:5];
   assign stall_load = IDEX_RegWrite & (IDEX_MemRead & ~IDEX_MemWrite) &
                      ((IFID_Rs == IDEX_writeRegSel) | (IFID_Rt == IDEX_writeRegSel));

   assign flush = take_branch_PC & ~Stall_DM; // Flush if we are executing a branch or jump
   assign stall = stall_load;


endmodule
`default_nettype wire