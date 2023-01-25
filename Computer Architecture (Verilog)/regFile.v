/*
   This module creates a 16-bit register.  It has 1 write port, 2 read
   ports, 3 register select inputs, a write enable, a reset, and a clock
   input.  All register state changes occur on the rising edge of the
   clock. 
*/
module regFile (
                // Outputs
                read1Data, read2Data, err,
                // Inputs
                clk, rst, read1RegSel, read2RegSel, writeRegSel, writeData, writeEn
                );

   input        clk, rst;
   input [2:0]  read1RegSel;
   input [2:0]  read2RegSel;
   input [2:0]  writeRegSel;
   input [15:0] writeData;
   input        writeEn;

   output reg [15:0] read1Data;
   output reg [15:0] read2Data;
   output        err;

   parameter NUM_REGS = 8;
   parameter WIDTH = 16;
   parameter SIZE = 128; //16*8

   wire [SIZE-1:0] totalReadData;
   wire [NUM_REGS-1:0] regWriteEnable;

   assign regWriteEnable = (writeEn == 1'b0) ? 8'b00000000 :
                           //Have to be 8 bit signal where 1 of them is set so that vector
                           //instantiation works -> ONE-HOT
                           (writeRegSel == 3'b111) ? 8'b10000000:
                           (writeRegSel == 3'b110) ? 8'b01000000:
                           (writeRegSel == 3'b101) ? 8'b00100000:
                           (writeRegSel == 3'b100) ? 8'b00010000:
                           (writeRegSel == 3'b011) ? 8'b00001000:
                           (writeRegSel == 3'b010) ? 8'b00000100:
                           (writeRegSel == 3'b001) ? 8'b00000010:
                                                     8'b00000001;

   //Instantiate 8 16-bit registers
   register16 #(WIDTH) iREGISTERS[NUM_REGS-1:0](.en(regWriteEnable), .clk(clk), .rst(rst), .in(writeData), .out(totalReadData));

   reg errRead1; //Error signal for Read 1
   always @* case(read1RegSel)
      3'b111 : begin read1Data = totalReadData[SIZE-1   : SIZE-16];  errRead1 = 1'b0; end
      3'b110 : begin read1Data = totalReadData[SIZE-17  : SIZE-32];  errRead1 = 1'b0; end
      3'b101 : begin read1Data = totalReadData[SIZE-33  : SIZE-48];  errRead1 = 1'b0; end
      3'b100 : begin read1Data = totalReadData[SIZE-49  : SIZE-64];  errRead1 = 1'b0; end
      3'b011 : begin read1Data = totalReadData[SIZE-65  : SIZE-80];  errRead1 = 1'b0; end
      3'b010 : begin read1Data = totalReadData[SIZE-81  : SIZE-96];  errRead1 = 1'b0; end
      3'b001 : begin read1Data = totalReadData[SIZE-97  : SIZE-112]; errRead1 = 1'b0; end
      3'b000 : begin read1Data = totalReadData[SIZE-113 : SIZE-128]; errRead1 = 1'b0; end
      default : errRead1 = 1'b0;
   endcase


   reg errRead2; //Error signal for Read 2
   always @* case(read2RegSel)
      3'b111 : begin read2Data = totalReadData[SIZE-1   : SIZE-16];  errRead2 = 1'b0; end
      3'b110 : begin read2Data = totalReadData[SIZE-17  : SIZE-32];  errRead2 = 1'b0; end
      3'b101 : begin read2Data = totalReadData[SIZE-33  : SIZE-48];  errRead2 = 1'b0; end
      3'b100 : begin read2Data = totalReadData[SIZE-49  : SIZE-64];  errRead2 = 1'b0; end
      3'b011 : begin read2Data = totalReadData[SIZE-65  : SIZE-80];  errRead2 = 1'b0; end
      3'b010 : begin read2Data = totalReadData[SIZE-81  : SIZE-96];  errRead2 = 1'b0; end
      3'b001 : begin read2Data = totalReadData[SIZE-97  : SIZE-112]; errRead2 = 1'b0; end
      3'b000 : begin read2Data = totalReadData[SIZE-113 : SIZE-128]; errRead2 = 1'b0; end
      default : errRead2 = 1'b0;
   endcase

   assign err = errRead1 | errRead2 | ~(~writeEn | writeEn);

endmodule
