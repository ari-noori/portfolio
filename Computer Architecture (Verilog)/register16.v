module register16(en, clk, rst, in, out);
    parameter WIDTH = 16;
    input en;
    input clk;
    input rst;
    input [WIDTH-1:0] in;
    output [WIDTH-1:0] out;

    wire [WIDTH-1:0] data;

    //If write enabled, write in to data, otherwise, write out to data to hold value
    assign data = en ? in : out;

    //Create 16 DFFs for each bit of our register
    dff DFFs[WIDTH-1:0](.q(out), .d(data), .clk(clk), .rst(rst));

endmodule