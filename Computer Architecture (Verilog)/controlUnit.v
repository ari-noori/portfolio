`default_nettype none
/*
    This module decides control signals depending on the input opcode.
*/
module controlUnit(
    // Inputs
    input wire  [4:0] opcode,     // Upper 5 bits from the instruction
    input wire  [1:0] ALUMode,    // Lower 2 bits from the instruction

    // Outputs
    output reg RegWrite,    // RegWrite for registers
    output reg LBI_sel,     // Select for LBI mux
    output reg [2:0] ALUOp, // Opcode sent to the ALU
    output reg [2:0] ALUSrc,// Select for ALU Source mux
    output reg c_in,        // Carry in to the ALU (for subtraction)
    output reg invA,        // Invert ALU input A
    output reg invB,        // Invert ALU input B
    output reg MemWrite,    // Write EN for data memory
    output reg MemRead,     // Read enable for data memory
    output reg MemToReg,    // Write memory to registers
    output reg HALT,        // HALT operation
    output reg PCtoReg,     // Select PC+2 to register
    output reg SLBI,        // SLBI Operation
    output reg [1:0] RegDst,// Selects the write register
    output reg BTR,         // Selects the BTR operation
    output reg [2:0] ALUCondition,// Selects the condition to the ALU: 0 for checking (<), 1 for checking (<=)
    output reg SET,         // Selects the condition if TRUE
    output reg JMP,         // Indicates a J or JAL instruction (NOT JR or JALR)
    output reg JALR_op,     // Indicates a JR or JALR
    output reg BR,          // Indicates any branch instruction
    output reg STU_out,     // indicates an STU instruction


    output reg err          // General purpose error signal
);

//////////////
// OPCODES //
////////////
parameter ADD_SUB_XOR_ANDN = 5'b11011;
parameter ROL_SLL_ROR_SRL = 5'b11010;
parameter LBI_op = 5'b11000;
parameter HALT_op = 5'b00000;
parameter NOP = 5'b00001;
parameter ADDI = 5'b01000;
parameter SLBI_op = 5'b10010; 
parameter SUBI = 5'b01001;
parameter XORI = 5'b01010;
parameter ANDNI = 5'b01011;
parameter ROLI = 5'b10100;
parameter SLLI = 5'b10101;
parameter RORI = 5'b10110;
parameter SRLI = 5'b10111;
parameter ST = 5'b10000;
parameter LD = 5'b10001;
parameter STU = 5'b10011;
parameter BTR_op = 5'b11001;
parameter SEQ = 5'b11100;
parameter SLT = 5'b11101;
parameter SLE = 5'b11110;
parameter SCO = 5'b11111;
parameter J = 5'b00100;
parameter JR = 5'b00101;
parameter JAL = 5'b00110;
parameter JALR = 5'b00111;
parameter BEQZ = 5'b01100;
parameter BNEZ = 5'b01101;
parameter BLTZ = 5'b01110;
parameter BGEZ = 5'b01111;
parameter SIIC = 5'b00010;
parameter RTI = 5'b00011;

always @(*) begin
    //default behavior
    RegWrite = 1'b0;     
    LBI_sel = 1'b0;    
    ALUOp = 4'b0000; 
    ALUSrc = 3'b000; 
    c_in = 1'b0;      
    invA = 1'b0;
    invB = 1'b0;
    MemWrite = 1'b0;
    MemRead = 1'b0;
    MemToReg = 1'b0;
    HALT = 1'b0;
    PCtoReg = 1'b0;
    SLBI = 1'b0;
    RegDst = 2'b00;
    BTR = 1'b0;
    ALUCondition = 3'b000;
    SET = 1'b0;
    JMP = 1'b0;
    JALR_op = 1'b0;
    BR = 1'b0;
    STU_out = 1'b0;
    err = 1'b0;

    case(opcode)
        NOP : HALT = 1'b0;

        LBI_op : begin LBI_sel = 1'b1; RegWrite = 1'b1; RegDst = 2'b10; end

        HALT_op : HALT = 1'b1;

        ADDI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b010; ALUOp = 3'b100; end

        SUBI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b010; ALUOp = 3'b100; invA = 1'b1; invB = 1'b0; c_in = 1'b1; end

        XORI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b100; ALUOp = 3'b111; end

        ANDNI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b100; invB = 1'b1; ALUOp = 3'b101; end

        ROLI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b100; ALUOp = 3'b000; end

        SLLI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b100; ALUOp = 3'b001; end

        RORI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b100; ALUOp = 3'b010; end
        
        SRLI : begin RegWrite = 1'b1; RegDst = 2'b01; ALUSrc = 3'b100; ALUOp = 3'b011; end
        
        ST : begin MemRead = 1'b1; MemWrite = 1'b1; ALUSrc = 3'b010; ALUOp = 3'b100; end

        LD : begin MemRead = 1'b1; ALUSrc = 3'b010; ALUOp = 3'b100; RegWrite = 1'b1; RegDst = 2'b01; MemToReg = 1'b1; end

        STU : begin STU_out = 1'b1; MemRead = 1'b1; MemWrite = 1'b1; ALUSrc = 3'b010; ALUOp = 3'b100; RegWrite = 1'b1; RegDst = 2'b10; end

        BTR_op : begin BTR = 1'b1; RegWrite = 1'b1; RegDst = 2'b00; end

        ADD_SUB_XOR_ANDN : begin 
            RegWrite = 1'b1;
            RegDst = 2'b00; //ins[4:2]
            ALUSrc = 2'b00; //read2Data
            case(ALUMode)
                2'b00 : begin ALUOp = 3'b100; end //ADD
                2'b01 : begin ALUOp = 3'b100; invA = 1'b1; invB = 1'b0; c_in = 1'b1; end //SUB
                2'b10 : begin ALUOp = 3'b111; end //XOR
                2'b11 : begin invB = 1'b1; ALUOp = 3'b101; end //ANDN
                default : err = 1'b1;
            endcase
        end

        ROL_SLL_ROR_SRL : begin
            RegWrite = 1'b1;
            RegDst = 2'b00; //ins[4:2]
            ALUSrc = 2'b00; //read2Data
            case(ALUMode)
                2'b00 : ALUOp = 3'b000; //ROL
                2'b01 : ALUOp = 3'b001; //SLL
                2'b10 : ALUOp = 3'b010; //ROR
                2'b11 : ALUOp = 3'b011; //SRL
                default : err = 1'b1;
            endcase
        end
        
        SEQ : begin ALUCondition = 3'b000; SET = 1'b1; RegWrite = 1'b1; RegDst = 2'b00; ALUOp = 3'b100; invB = 1'b1; c_in = 1'b1; end

        SLT : begin ALUCondition = 3'b010; SET = 1'b1; RegWrite = 1'b1; RegDst = 2'b00; ALUOp = 3'b100; invB = 1'b1; c_in = 1'b1; end

        SLE : begin ALUCondition = 3'b001; SET = 1'b1; RegWrite = 1'b1; RegDst = 2'b00; ALUOp = 3'b100; invB = 1'b1; c_in = 1'b1; end

        SCO : begin ALUCondition = 3'b011; SET = 1'b1; RegWrite = 1'b1; RegDst = 2'b00; ALUOp = 3'b100; end

        BEQZ : begin ALUCondition = 3'b000; ALUOp = 3'b100; ALUSrc = 3'b011; BR = 1'b1; end

        BNEZ : begin ALUCondition = 3'b100; ALUOp = 3'b100; ALUSrc = 3'b011; BR = 1'b1; end

        BLTZ : begin ALUCondition = 3'b010; ALUOp = 3'b100; ALUSrc = 3'b011; BR = 1'b1; end

        BGEZ : begin ALUCondition = 3'b001; ALUOp = 3'b100; ALUSrc = 3'b011; BR = 1'b1; invA = 1'b1; c_in = 1'b1; end

        SLBI_op : begin SLBI = 1'b1; RegWrite = 1'b1; RegDst = 2'b10; ALUSrc = 3'b001; ALUOp = 3'b110; end

        J : begin JMP = 1'b1; end

        JR : begin JALR_op = 1'b1; end

        JAL : begin JMP = 1'b1; RegWrite = 1'b1; RegDst = 2'b11; PCtoReg = 1'b1; end

        JALR : begin JALR_op = 1'b1; RegWrite = 1'b1; RegDst = 2'b11; PCtoReg = 1'b1; end

        SIIC : err = 1'b0;

        RTI : err = 1'b0;
        
        default : begin err = 1'b1; end
    endcase
end
endmodule

`default_nettype wire