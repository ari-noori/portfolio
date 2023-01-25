/*
    Module for abstracting away the carry lookahead logic
*/
module cla_logic_4b(C, p, g, c_in, P_total, G_total);

    output [3:0] C;
    output P_total, G_total;
    input [3:0] p, g;
    input c_in;


    //C0
    wire p0cin;
    and2 iAND0(p0cin, p[0], c_in);
    or2 iOR0(C[0], p0cin, g[0]);

    //C1
    wire p1p0cin, g0p1;
    and2 iAND1(p1p0cin, p0cin, p[1]);
    and2 iAND2(g0p1, g[0], p[1]);
    or3 iOR1(C[1], p1p0cin, g0p1, g[1]);

    //C2
    wire p2p1p0c0, g0p2p1, g1p2;
    and2 iAND3(p2p1p0c0, p1p0cin, p[2]);
    and2 iAND4(g0p2p1, g0p1, p[2]);
    and2 iAND5(g1p2, g[1], p[2]);
    or4 iOR2(C[2], g[2], g1p2, g0p2p1, p2p1p0c0);

    //C3
    wire p3p2p1p0c0, g0p3p2p1, g1p3p2, g2p3;
    and2 iAND6(p3p2p1p0c0, p2p1p0c0, p[3]);
    and2 iAND7(g0p3p2p1, g0p2p1, p[3]);
    and2 iAND8(g1p3p2, g1p2, p[3]);
    and2 iAND9(g2p3, g[2], p[3]);
    or5 iOR3(C[3], g[3], g2p3, g1p3p2, g0p3p2p1, p3p2p1p0c0);

    //Total P
    and4 iANDP(P_total, p[3], p[2], p[1], p[0]);

    //Total G
    or4 iORG(G_total, g[3], g2p3, g1p3p2, g0p3p2p1);

endmodule