/*
    A 4-bit CLA module
*/
module cla_4b(sum, c_out, a, b, c_in, P_total, G_total);

    // declare constant for size of inputs, outputs (N)
    parameter   N = 4;

    output [N-1:0] sum;
    output         c_out;
    output P_total, G_total;
    input [N-1: 0] a, b;
    input          c_in;

    wire [N-1:0] C, g, p;

    wire dummy;

    wire c1_n, c1, c2_n, c2, c3_n, c3;
    wire p0c0_n, p0c0, p1c1_n, p1c1, p2c2_n, p2c2, p0c3_n, p0c3;
    

    fullAdder_1b iFA0(.s(sum[0]), .c_out(dummy), .a(a[0]), .b(b[0]), .c_in(c_in), .g(g[0]), .p(p[0]));
    fullAdder_1b iFA1(.s(sum[1]), .c_out(dummy), .a(a[1]), .b(b[1]), .c_in(C[0]), .g(g[1]), .p(p[1]));
    fullAdder_1b iFA2(.s(sum[2]), .c_out(dummy), .a(a[2]), .b(b[2]), .c_in(C[1]), .g(g[2]), .p(p[2]));
    fullAdder_1b iFA3(.s(sum[3]), .c_out(dummy), .a(a[3]), .b(b[3]), .c_in(C[2]), .g(g[3]), .p(p[3]));

    //Carry logic
    cla_logic_4b iCLA(.C({c_out, C[2:0]}), .p(p), .g(g), .c_in(c_in), .P_total(P_total), .G_total(G_total));

endmodule
