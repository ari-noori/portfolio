/*
   This module creates a wrapper around the 8x16b register file, to do
   do the bypassing logic for RF bypassing.
*/
module regFile_bypass (
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

   output [15:0] read1Data;
   output [15:0] read2Data;
   output        err;

   wire [15:0] bypass_read1Data, bypass_read2Data;

   //instantiate register
   regFile iREGISTER(.read1Data(bypass_read1Data), .read2Data(bypass_read2Data), .err(err),
                     .clk(clk), .rst(rst), .read1RegSel(read1RegSel), .read2RegSel(read2RegSel), 
                     .writeRegSel(writeRegSel), .writeData(writeData), .writeEn(writeEn));

   //Bypassing logic - if we are reading and writing to the same reg (and WE), then bypass
   //the data
   assign read1Data = (read1RegSel == writeRegSel) & writeEn ? writeData : bypass_read1Data;
   assign read2Data = (read2RegSel == writeRegSel) & writeEn ? writeData : bypass_read2Data; 
endmodule
