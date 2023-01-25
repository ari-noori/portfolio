
module RemoteComm(clk, rst_n, RX, TX, cmd, send_cmd, cmd_sent, resp_rdy, resp);

    input clk, rst_n;		        // clock and active low reset
    input RX;				        // serial data input
    input send_cmd;			        // indicates to tranmit 24-bit command (cmd)
    input [15:0] cmd;		        // 16-bit command

    output logic TX;				// serial data output
    output logic cmd_sent;		    // indicates transmission of command complete
    output logic resp_rdy;		    // indicates 8-bit response has been received
    output logic [7:0] resp;		// 8-bit response from DUT

    wire [7:0] tx_data;		        // 8-bit data to send to UART
    wire tx_done;			        // indicates 8-bit was sent over UART

    /*
    Intermediates
    */
    logic sel;              // used to select high or low byte
    logic trmt;             // asserted for 1 clock to initate tranmission with UART Tx

    ///////////////////////////////////////////////
    // Registers needed...state machine control //
    /////////////////////////////////////////////
    /*
    SR Flop for setting cmd_snt
    */
    logic set_cmd_snt;
    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            cmd_sent <= 1'b0;
        else if (send_cmd)
            cmd_sent <= 1'b0;  // R input, reset flop
        else if (set_cmd_snt)
            cmd_sent <= 1'b1;  // S input, set flop
        // if neither S or R are high, maintain state
    end

    /*
    FF to buffer the lower byte of the command
    */
    logic [7:0] lowByteFF;
    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            lowByteFF <= 8'h00;
        else if (send_cmd)
            lowByteFF <= cmd[7:0];
    end

    // continuous assign tx_data we send out to the upper or lower byte based on sel value
    assign tx_data = sel ? cmd[15:8] : lowByteFF;

    ///////////////////////////////
    // state definitions for SM //
    /////////////////////////////
    /*
    Define the states needed for state machine.
    Here using one state to buffer low byte, and 
    another for the high byte, and an idle state.
    */
    typedef enum reg[1:0] {LOW, HIGH, IDLE} state_t;
    state_t state, nxt_state;
    
    ////////////////////////////
    // State maintaining FF  //
    //////////////////////////
    always_ff @(posedge clk, negedge rst_n) begin
        if (!rst_n)
            state <= IDLE;
        else
            state <= nxt_state;
    end

    //////////////////////////////
    // State Machine Logic     //
    ////////////////////////////
    always_comb begin
        nxt_state = state;      // default to avoid latches
        
        // default all SM outputs
        trmt = 0;
        set_cmd_snt = 0;
        sel = 0;
    
        case (state)
            /*
            IDLE state where we wait for send_cmd to be asserted.
            Then put trmt high, select the upper byte, and move 
            to HIGH state to buffer it
            */
            IDLE : begin
                if (send_cmd) begin
                    trmt = 1;
                    sel = 1;
                    nxt_state = HIGH;
                end
            end
            /* 
            HIGH state where we wait for tx_done to indicate high 
            byte has been sent at which point we move to LOW state
            */
            HIGH : begin
                if (tx_done) begin
                    trmt = 1;
                    nxt_state = LOW;
                end
            end
            /*
            LOW state where we wait for tx_done to indicate low
            byte has been sent at which point we move back to IDLE
            and wait for another transmission to occur (using LOW 
            as default state since it transitions to IDLE state)
            */
            default : begin
                if (tx_done) begin
                    set_cmd_snt = 1;
                    nxt_state = IDLE;
                end 
            end
        endcase
    end

    ///////////////////////////////////////////////
    // Instantiate basic 8-bit UART transceiver //
    /////////////////////////////////////////////
    UART iUART(.clk(clk), .rst_n(rst_n), .RX(RX), .TX(TX), .tx_data(tx_data), .trmt(trmt),
            .tx_done(tx_done), .rx_data(resp), .rx_rdy(resp_rdy), .clr_rx_rdy(resp_rdy));
            

endmodule