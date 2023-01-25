/*
    A barrel shifter module.  It is designed to shift a number via rotate
    left, shift left, shift right arithmetic, or shift right logical based
    on the 'Oper' value that is passed in.  It uses these
    shifts to shift the value any number of bits.
 */
module shifter (In, ShAmt, Oper, Out);

    // declare constant for size of inputs, outputs, and # bits to shift
    parameter OPERAND_WIDTH = 16;
    parameter SHAMT_WIDTH   =  4;
    parameter NUM_OPERATIONS = 2;

    input  [OPERAND_WIDTH -1:0] In   ; // Input operand
    input  [SHAMT_WIDTH   -1:0] ShAmt; // Amount to shift/rotate
    input  [NUM_OPERATIONS-1:0] Oper ; // Operation type
    output [OPERAND_WIDTH -1:0] Out  ; // Result of shift/rotate

    wire appendBit; //Decides between logical and arithmetic shifts
    wire [15:0] right1, right2, right4, right8; //Each bit of the rshift
    wire [15:0] left1, left2, left4, left8; //Each bit of the lshift


    //Pick Left vs. Right depending on MSB of opcode
    assign Out = Oper[1] ? right8 : left8;
   
    /*
    Right Shifts:

    Depending on which bits are set, we will shift a certain amount. Each assign
    statement will conduct a shift by the power-of-2 bit position that represents
    its select bit. That result is then fed into the next assign statement which
    will conduct another shift depending on the ShAmt input. 
    */
    //assign appendBit = Oper[0] ? 1'b0 : In[15]; //1 LSB -> Logical, 0 LSB -> Arithmetic
    
    assign right1 = ShAmt[0] ? {(Oper[0] ? 1'b0        : In[0]),       In[15:1]} : In;
    assign right2 = ShAmt[1] ? {(Oper[0] ? 2'b00       : right1[1:0]), right1[15:2]} : right1;
    assign right4 = ShAmt[2] ? {(Oper[0] ? 4'b0000     : right2[3:0]), right2[15:4]} : right2;
    assign right8 = ShAmt[3] ? {(Oper[0] ? 8'b00000000 : right4[7:0]), right4[15:8]} : right4;


    /*
    Left Shifts:

    Similar to right shift. Each assign statement will shift the operand by the
    power-of-2 amount specified in the bit. Each previous bit is fed into the next
    assign statement. The LSB of Oper controls whether we shift in 0s or rotate
    the MSBs. 
    */
    assign left1 = ShAmt[0] ? {In   [14:0], (Oper[0] ? 1'b0        : In[15])}       : In;
    assign left2 = ShAmt[1] ? {left1[13:0], (Oper[0] ? 2'b00       : left1[15:14])} : left1;
    assign left4 = ShAmt[2] ? {left2[11:0], (Oper[0] ? 4'b0000     : left2[15:12])} : left2;
    assign left8 = ShAmt[3] ? {left4[ 7:0], (Oper[0] ? 8'b00000000 : left4[15: 8])} : left4;
   
endmodule
