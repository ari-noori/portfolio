`default_nettype none
/*
    A multi-bit ALU module (defaults to 16-bit). It is designed to choose
    the correct operation to perform on 2 multi-bit numbers from rotate
    left, shift left, shift right arithmetic, shift right logical, add,
    or, xor, & and.  Upon doing this, it should output the multi-bit result
    of the operation, as well as drive the output signals Zero and Overflow
    (OFL).
*/
module alu (InA, InB, Cin, Oper, invA, invB, sign, Out, Zero, Ofl, condition, condition_true);

    parameter OPERAND_WIDTH = 16;    
    parameter NUM_OPERATIONS = 3;
       
    input wire [OPERAND_WIDTH -1:0] InA ; // Input operand A
    input wire [OPERAND_WIDTH -1:0] InB ; // Input operand B
    input wire                      Cin ; // Carry in
    input wire [NUM_OPERATIONS-1:0] Oper; // Operation type
    input wire                      invA; // Signal to invert A
    input wire                      invB; // Signal to invert B
    input wire                      sign; // Signal for signed operation
    input wire [2:0] condition; // 00 for checking == 0 
                                // 01 for checking (<=) 
                                // 10 for checking (<) 
                                // 11 for checking c_out
                                // 1xx for checking != 0
    
    output reg [OPERAND_WIDTH -1:0] Out; // Result of computation
    output wire                     Ofl; // Signal if overflow occured
    output wire                    Zero; // Signal if Out is 0
    output wire condition_true;          // Signal if specified condition is met

    wire [15:0] A_inv, B_inv;
    wire [15:0] sh_out, cla_out;
    wire c_out;
    reg err;

    // Assign zero flag
    assign Zero = Out == 16'h0000;

    // Invert A and B if needed
    assign A_inv = invA ? ~InA : InA;
    assign B_inv = invB ? ~InB : InB;

    // ALU Logic: Case statement that assigns the output depending on the 
    // operation type.
    always @* casex(Oper)
        3'b0xx : begin Out = sh_out; err = 1'b0; end
        3'b100 : begin Out = cla_out; err = 1'b0; end
        3'b101 : begin Out = (A_inv & B_inv); err = 1'b0; end
        3'b110 : begin Out = (A_inv | B_inv); err = 1'b0; end
        3'b111 : begin Out = (A_inv ^ B_inv); err = 1'b0; end
        default: begin Out = 16'h0000; err = 1'b1; end
    endcase 

    // Overflow detection:
    // Add Unsigned: If there is a Cout
    // Add Signed: If operands MSB are same but output MSB is different
    assign Ofl = sign ? ( (A_inv[15] == B_inv[15]) & (B_inv[15] != Out[15]) ) : c_out;

    // condition
    assign condition_true = ( condition[2])                 ? ~Zero                : //1xx  
                            (~condition[1] & ~condition[0]) ? Zero                 : //00
                            (~condition[1] &  condition[0]) ? (Zero | ( (~Ofl & cla_out[15]) | (Ofl & ~cla_out[15]) )) : //01
                            ( condition[1] & ~condition[0]) ? ( (~Ofl & cla_out[15]) | (Ofl & ~cla_out[15]) )          : //10
                            ( condition[1] &  condition[0]) ? c_out : 1'b0;          //11
                            
    // Instantiate Adder/Shifter
    cla_16b iCLA(.sum(cla_out), .c_out(c_out), .a(A_inv), .b(B_inv), .c_in(Cin));
    shifter iSHFT(.In(A_inv), .ShAmt(B_inv[3:0]), .Oper(Oper[1:0]), .Out(sh_out));
    
endmodule

`default_nettype wire