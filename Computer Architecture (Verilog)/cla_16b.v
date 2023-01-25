/*
    a 16-bit CLA module
*/
module cla_16b(sum, c_out, a, b, c_in);

    // declare constant for size of inputs, outputs (N)
    parameter   N = 16;

    output [N-1:0] sum;
    output         c_out;
    input [N-1: 0] a, b;
    input          c_in;

    wire dummy;
    wire [3:0] P_total, G_total;
    wire [2:0] C;

    cla_4b CLA0(.sum(sum[3:0]),     .c_out(dummy), .a(a[3:0]), .b(b[3:0]), .c_in(c_in), .P_total(P_total[0]), .G_total(G_total[0]));
    cla_4b CLA1(.sum(sum[7:4]),     .c_out(dummy), .a(a[7:4]), .b(b[7:4]), .c_in(C[0]), .P_total(P_total[1]), .G_total(G_total[1]));
    cla_4b CLA2(.sum(sum[11:8]),    .c_out(dummy), .a(a[11:8]), .b(b[11:8]), .c_in(C[1]), .P_total(P_total[2]), .G_total(G_total[2]));
    cla_4b CLA3(.sum(sum[15:12]),   .c_out(dummy), .a(a[15:12]), .b(b[15:12]), .c_in(C[2]), .P_total(P_total[3]), .G_total(G_total[3]));

    cla_logic_4b iCLAL4(.C({c_out, C[2:0]}), .p(P_total), .g(G_total), .c_in(c_in), .P_total(dummy), .G_total(dummy));

endmodule
