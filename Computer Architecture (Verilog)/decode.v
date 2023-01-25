`default_nettype none
/*
   Filename        : decode.v
   Description     : This is the module for the overall decode stage of the processor.
*/
module decode (
   // Inputs
   input wire  [15:0] instruction,  // 16 bit instruction from FETCH
   input wire  [15:0] writeData,    // Data to be written to the registers from WB    
   input wire  clk, 
   input wire  rst,
   input wire  stall, 

   //input wire [1:0] RegDstWB, // RegDst signal that has been passed through the pipeline
   input wire RegWriteWB,
   input wire [2:0] writeRegSelWB,

   // Outputs
   output wire  [15:0] read1Data,  // Output from read operation
   output wire  [15:0] read2Data,  // Output from read operation

   // Control unit outputs
   output wire RegWrite,     // RegWrite for registers
   output wire  LBI_sel,     // Select for LBI mux
   output wire  [2:0] ALUOp, // Opcode sent to the ALU
   output wire  [2:0] ALUSrc,// Select for ALU Source mux
   output wire  c_in,        // Carry in to the ALU (for subtraction)
   output wire  invA,        // Invert ALU input A
   output wire  invB,        // Invert ALU input B
   output wire  MemWrite,    // Write EN for data memory
   output wire  MemRead,     // Read enable for data memory
   output wire  MemToReg,    // Write memory to registers
   output wire  HALT,        // HALT operation
   output wire  PCtoReg,     // Select PC+2 to register
   output wire  SLBI,        // SLBI Operation
   output wire  BTR,         // Selects the BTR operation
   output wire  [2:0] ALUCondition,// Selects the condition to the ALU: 0 for checking (<), 1 for checking (<=)
   output wire  SET,         // Selects the condition if TRUE
   output wire  JMP,         // Indicates a J or JAL instruction (NOT JR or JALR)
   output wire  JALR_op,     // Indicates a JR or JALR
   output wire  BR,          // Indicates any branch instruction 
   output wire  STU_out, 

   //output wire [1:0] RegDst,  // Destination reg select signal that is to be sent thru pipeline

   // Sign extend outputs
   output wire [15:0] i8SE,
   output wire [15:0] i5SE,
   output wire [15:0] i50E,
   output wire [15:0] i80E,
   output wire [15:0] d11SE, 
   output wire [2:0] writeRegSel, // Register select that will be written to 

   // Forwarding
   output wire [15:0] instruction_out,

   output wire branch_jump_op,

   output wire err
);

   assign instruction_out = instruction;
   assign branch_jump_op = rst ? 1'b0 : BR | JMP | JALR_op;

   // Wires for stalling??
   wire MemWrite_control, MemRead_control, RegWrite_control;
   assign MemWrite =  MemWrite_control; //stall ? 1'b0 :
   assign MemRead =  MemRead_control;
   assign RegWrite =  RegWrite_control;

   //wire [2:0] writeRegSel; 
   wire [1:0] RegDstWB;      // Selects the write register
   wire err_control, err_reg;

   assign err = err_control | err_reg;

   // Write register selection MUX
   // 00 instruction [4:2]
   // 01 instruction [7:5]
   // 10 instruction [10:8]
   // 11 R7 -> 3'b111
   assign writeRegSel = ( RegDstWB[1] &  RegDstWB[0]) ? 3'b111 :            //11
                        ( RegDstWB[1] & ~RegDstWB[0]) ? instruction[10:8]:  //10 
                        (~RegDstWB[1] &  RegDstWB[0]) ? instruction[ 7:5]:  //01
                                                    instruction[ 4:2];  //00

   ///////////////////
   // CONTROL UNIT //
   /////////////////
   controlUnit iCONTROL_UNIT(
      // Inputs
      .opcode(instruction[15:11]),  // Upper 5 bits from the instruction
      .ALUMode(instruction[1:0]),   // Lower 2 bits from the instruction

      // Outputs
      .RegWrite(RegWrite_control),    // RegWrite for registers
      .LBI_sel(LBI_sel),      // Select for LBI mux
      .ALUOp(ALUOp),          // Opcode sent to the ALU
      .ALUSrc(ALUSrc),        // Select for ALU Source mux
      .c_in(c_in),            // Carry in to the ALU (for subtraction)
      .invA(invA),            // Invert ALU input A
      .invB(invB),            // Invert ALU input B
      .MemWrite(MemWrite_control),    // Write EN for data memory
      .MemRead(MemRead_control),      // Read enable for data memory
      .MemToReg(MemToReg),    // Write memory to registers
      .HALT(HALT),            // HALT operation
      .PCtoReg(PCtoReg),      // Select PC+2 to register
      .SLBI(SLBI),            // SLBI Operation
      .RegDst(RegDstWB),        // Selects the write register
      .BTR(BTR),
      .ALUCondition(ALUCondition),
      .SET(SET),
      .JMP(JMP),
      .JALR_op(JALR_op),
      .BR(BR),
      .STU_out(STU_out),
      .err(err_control)          // General purpose error signal from control unit
   );

   ////////////////
   // REGISTERS //
   //////////////
   regFile_bypass regFile0(
                // Outputs
                .read1Data(read1Data), // Output from read operation
                .read2Data(read2Data), // Output from read operation
                .err(err_reg),             // Register file error signal
                // Inputs
                .clk(clk), .rst(rst), 
                .read1RegSel(instruction[10:8]),   // Always select [10:8] for first read reg
                .read2RegSel(instruction[7:5]),    // Select which register to read from
                .writeRegSel(writeRegSelWB),         // Select which register to write to (input to DECODE)
                .writeData(writeData),             // Data that is to be written to registers (input from WB)
                .writeEn(RegWriteWB)                 // writeEn signal comes from CONTROL UNIT
                );

   // Sign extend all signals
   assign i8SE = { {8{instruction[7]}}, instruction[7:0]};
   assign i5SE = { {11{instruction[4]}}, instruction[4:0]};
   assign i50E = { 11'h000, instruction[4:0]};
   assign i80E = { 8'h00, instruction[7:0]};
   assign d11SE = { {5{instruction[10]}}, instruction[10:0]};
   
endmodule

`default_nettype wire