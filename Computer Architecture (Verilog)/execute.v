`default_nettype none
/*
   Filename        : execute.v
   Description     : This is the overall module for the execute stage of the processor.
*/
module execute (
   input wire [15:0] read1Data,  // Register 1 from DECODE
   input wire  [15:0] read2Data,  // Register 2 from DECODE
   input wire  [15:0] i80E,       // Zero extended 8-immediate from DECODE
   input wire  [15:0] i5SE,       // Sign extended 5-immediate from DECODE
   input wire  [15:0] i50E,       // Zero extended 5-immediate from DECODE
   input wire  [15:0] d11SE,      // Sign extended 11-displacement from DECODE
   input wire  [15:0] i8SE,       // Sign extended 8-immediate from DECODE
   input wire  [15:0] PC,         // PC from FETCH

   // Control signals
   input wire  [2:0] ALUOp,      // ALU Opcode from DECODE
   input wire  SLBI,             // Select from DECODE
   input wire  [2:0] ALUSrc,     // Select from DECODE
   input wire  c_in,             // ALU c_in from DECODE
   input wire  invA,             // Invert ALU input A
   input wire  invB,             // Invert ALU input B
   input wire  BTR,              // Selects the BTR operation
   input wire  [2:0] condition,  // Selects the ALU condition
   input wire  SET,              // Selects the condition if true
   input wire  JMP,              // Indicates a J or JAL instruction (NOT JR or JALR)
   input wire  JALR_op,          // Indicates a JR or JALR
   input wire  BR,               // Indicates any branch instruction
   input wire LBI_sel,

   /////////////////
   // Forwarding //
   ///////////////
   
   // Instruction from IDEX
   input wire [15:0] IDEX_instruction,       // Instruction to get Rs and Rt

   // EX-EX
   input wire [15:0] EX_EX_fwd_data,  // Data to be forwarded 
   input wire [2:0] EXMEM_IDEX_writeRegSel,  // register in EXMEM
   input wire EXMEM_IDEX_RegWrite,           // If writing to register in EXMEM
   input wire EXMEM_IDEX_MemRead,            // Do not forward if doing a load
   input wire EXMEM_IDEX_MemWrite,
   input wire EXMEM_IDEX_STU,
   

   // MEM-EX
   input wire [15:0] MEM_EX_fwd_data,  // Data to be forwarded
   input wire [2:0] MEMWB_EXMEM_IDEX_writeRegSel,  // register in MEMWB
   input wire MEMWB_EXMEM_IDEX_RegWrite, 

   // Outputs
   output wire  zero,
   output wire  [15:0] newPC,
   output wire  [15:0] read2DataOut,
   output wire  [15:0] ALUResult,
   output wire  take_branch_PC,
   output wire  err
);

   wire dummy;
   wire [15:0] ALU_inA;
   wire [15:0] ALU_inB;
   wire [15:0] ALU_out;
   wire [15:0] operation_out;
   wire Ofl;
   wire condition_true;
   wire [15:0] BTR_Data;
   wire [15:0] JALR_PC;
   wire [15:0] BRJ_PC;
   wire [15:0] BRJ_out;

   /////////////////
   // Forwarding //
   ///////////////
   wire [15:0] ALU_inA_fwd;
   wire [15:0] ALU_inB_fwd;

   wire [2:0] IDEX_Rs;
   wire [2:0] IDEX_Rt;
   assign IDEX_Rs = IDEX_instruction[10:8];
   assign IDEX_Rt = IDEX_instruction[7:5];

   // Don't forward Rt if we are choosing immediate
   wire imm_sel;
   assign imm_sel = ALUSrc[2] | 
                   (ALUSrc[1] & ALUSrc [0]) | 
                   (ALUSrc[1] & ~ALUSrc [0]) |
                   (~ALUSrc[1] & ALUSrc [0]);

   // EX-EX
   wire fwd_X_X_rs_sel;
   assign fwd_X_X_rs_sel = EXMEM_IDEX_RegWrite & (EXMEM_IDEX_writeRegSel == IDEX_Rs) & (~EXMEM_IDEX_MemRead | (EXMEM_IDEX_MemRead & EXMEM_IDEX_MemWrite & EXMEM_IDEX_STU));

   wire fwd_X_X_rt_sel;
   assign fwd_X_X_rt_sel = EXMEM_IDEX_RegWrite & (EXMEM_IDEX_writeRegSel == IDEX_Rt) & (~EXMEM_IDEX_MemRead | (EXMEM_IDEX_MemRead & EXMEM_IDEX_MemWrite & EXMEM_IDEX_STU));
   

   // MEM-EX
   wire fwd_M_X_rs_sel;
   assign fwd_M_X_rs_sel = (MEMWB_EXMEM_IDEX_writeRegSel == IDEX_Rs) & MEMWB_EXMEM_IDEX_RegWrite;

   wire fwd_M_X_rt_sel;
   assign fwd_M_X_rt_sel = (MEMWB_EXMEM_IDEX_writeRegSel == IDEX_Rt) & MEMWB_EXMEM_IDEX_RegWrite;

   // Complete the forward
   assign ALU_inA_fwd = SLBI & fwd_X_X_rs_sel ? {EX_EX_fwd_data[7:0], 8'b00000000} :
                        SLBI & fwd_M_X_rs_sel ? {MEM_EX_fwd_data[7:0], 8'b00000000} :
                        fwd_X_X_rs_sel ? EX_EX_fwd_data : 
                        fwd_M_X_rs_sel ? MEM_EX_fwd_data :
                        ALU_inA;

   assign ALU_inB_fwd = imm_sel ? ALU_inB :
                        fwd_X_X_rt_sel ? EX_EX_fwd_data : 
                        fwd_M_X_rt_sel ? MEM_EX_fwd_data :
                        ALU_inB;

   assign err = 1'b0; //Nothing to error at this stage

   //////////
   // ALU //
   ////////
   alu ALU(
            // Inputs
            .InA(ALU_inA_fwd), 
            .InB(ALU_inB_fwd), 
            .Cin(c_in), 
            .Oper(ALUOp), 
            .invA(invA), 
            .invB(invB), 
            .sign(1'b1), //always signed operations
            .condition(condition), 
            
            // Outputs
            .Out(ALU_out), 
            .Zero(zero), 
            .Ofl(Ofl),
            .condition_true(condition_true)
         );

   ////////////////////////
   // ADDER FOR JR/JALR //
   //////////////////////
   cla_16b JALR_Adder(.sum(JALR_PC), // Output read1Data + SE ins[7:0]
                     .c_out(dummy), // No need for c_out
                     .a(ALU_inA_fwd), // Operand 1 read1Data
                     .b(i8SE),      // Operand 2 is SE ins[7:0]
                     .c_in(1'b0));  // No Cin

   ////////////////////////////////
   // ADDER FOR BRANCH AND JUMP //
   //////////////////////////////
   wire [15:0] BRJ_operand;
   assign BRJ_operand = JMP ? d11SE : i8SE; // If doing a jump, select the displacement, else, select the imm sign extend
   cla_16b BRJ_Adder(.sum(BRJ_PC), // Output PC + SE displacement/imm
                     .c_out(dummy), // No need for c_out
                     .a(PC),        // Operand 1 incremented PC
                     .b(BRJ_operand),      // Operand 2 is SE displacement/imm
                     .c_in(1'b0));  // No Cin


   // PC JR/JALR Mux
   assign newPC = JALR_op ? JALR_PC : ((condition_true & BR) | JMP) ? BRJ_PC : PC;

   // Signal to send to fetch to signal that we are taking a branch
   assign take_branch_PC = JALR_op | ((condition_true & BR) | JMP);

   // SLBI Mux (Shift left read1Data if SLBI or read1Data)
   assign ALU_inA = SLBI ? {read1Data[7:0], 8'b00000000} : read1Data;

   // ALU Source Mux
   // 00 = read2Data
   // 01 = i80E
   // 10 = i5SE
   // 11 = ZERO
   // 100 = i50E
   assign ALU_inB =  ALUSrc[2]                 ? i50E :     // 100
                    (ALUSrc[1] & ALUSrc [0])   ? 16'h0000 : // 11
                    (ALUSrc[1] & ~ALUSrc [0])  ? i5SE :     // 10
                    (~ALUSrc[1] & ALUSrc [0])  ? i80E :     // 01 
                                                 read2Data; // 00

   // CONSTANT SELECT Mux
   assign ALUResult = SET ? condition_true : operation_out;

   // Read2Data output
   assign read2DataOut = fwd_X_X_rt_sel ? EX_EX_fwd_data : 
                         fwd_M_X_rt_sel ? MEM_EX_fwd_data :read2Data; // Forwarding here for stores?

   //BTR - Select between ALU output or BTR operation
   wire [15:0] BTR_Data_XX;
   wire [15:0] BTR_Data_MX;
   assign operation_out = BTR & fwd_X_X_rs_sel ? BTR_Data_XX :
                          BTR & fwd_M_X_rs_sel ? BTR_Data_MX :
                          BTR ? BTR_Data : 
                          LBI_sel ? i8SE :
                          ALU_out;
   assign BTR_Data = { read1Data[0],
                       read1Data[1],
                       read1Data[2],
                       read1Data[3],
                       read1Data[4],
                       read1Data[5],
                       read1Data[6],
                       read1Data[7],
                       read1Data[8],
                       read1Data[9],
                       read1Data[10],
                       read1Data[11],
                       read1Data[12],
                       read1Data[13],
                       read1Data[14],
                       read1Data[15]};

   assign BTR_Data_XX = { EX_EX_fwd_data[0],
                       EX_EX_fwd_data[1],
                       EX_EX_fwd_data[2],
                       EX_EX_fwd_data[3],
                       EX_EX_fwd_data[4],
                       EX_EX_fwd_data[5],
                       EX_EX_fwd_data[6],
                       EX_EX_fwd_data[7],
                       EX_EX_fwd_data[8],
                       EX_EX_fwd_data[9],
                       EX_EX_fwd_data[10],
                       EX_EX_fwd_data[11],
                       EX_EX_fwd_data[12],
                       EX_EX_fwd_data[13],
                       EX_EX_fwd_data[14],
                       EX_EX_fwd_data[15]};

   assign BTR_Data_MX = { MEM_EX_fwd_data[0],
                       MEM_EX_fwd_data[1],
                       MEM_EX_fwd_data[2],
                       MEM_EX_fwd_data[3],
                       MEM_EX_fwd_data[4],
                       MEM_EX_fwd_data[5],
                       MEM_EX_fwd_data[6],
                       MEM_EX_fwd_data[7],
                       MEM_EX_fwd_data[8],
                       MEM_EX_fwd_data[9],
                       MEM_EX_fwd_data[10],
                       MEM_EX_fwd_data[11],
                       MEM_EX_fwd_data[12],
                       MEM_EX_fwd_data[13],
                       MEM_EX_fwd_data[14],
                       MEM_EX_fwd_data[15]};
   
endmodule

`default_nettype wire