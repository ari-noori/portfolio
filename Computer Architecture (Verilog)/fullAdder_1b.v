/*
    a 1-bit full adder
*/
module fullAdder_1b(s, c_out, a, b, c_in, g, p);
    output s;
    output c_out;
    output g;
    output p;
    input   a, b;
    input  c_in;

    wire ab, ab_n, AxorB, c_in_AxorB_n, c_in_AxorB, nor_sig, p_n;

    //Sum
    xor3 iXOR3(s, a, b, c_in);
    
    /*
    Carry
    */
    // c_in(A XOR B)
    xor2 iXOR2(AxorB, a, b); //A XOR B
    nand2 iNAND21(c_in_AxorB_n, AxorB, c_in); //c_in NAND (A XOR B)
    not1 iNOT1(c_in_AxorB, c_in_AxorB_n); // revert to AND

    // AB
    nand2 iNAND22(ab_n, a, b);
    not1 iNOT2(ab, ab_n);

    // AB + c_in(A XOR B)
    nor2 iNOR2(nor_sig, c_in_AxorB, ab);
    not1 iNOT3(c_out, nor_sig);

    //generate
    not1 iNOT4(g, ab_n);

    //propegate
    // nor2 iNOR3(p_n, a, b);
    // not1 iNOT5(p, p_n);
    xor2 iXORC(p, a, b);

endmodule
