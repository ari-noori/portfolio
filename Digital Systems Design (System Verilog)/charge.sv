
module charge(  input clk,              //50MHz clk
                input rst_n,            //reset
                input go,               //initiates tune
                output logic piezo,     //differential piezo drive
                output logic piezo_n);
    
    /*
    Fast Simulation
    */
    logic [5:0] increment;      
    parameter FAST_SIM = 1;             //accelerate simulation         
    
    generate if(FAST_SIM) begin
        assign increment = 5'h10;       //increment by 16 for fast sim
    end else begin
        assign increment = 1'b1;        //increment by 1 by default
    end
    endgenerate

    /*
    Note Frquency Counter
    */
    logic [14:0] note_counter;
    logic [14:0] frequency;
    logic clr_timer;

    always_ff @(posedge clk, negedge rst_n)
        if(!rst_n)
            note_counter <= 15'h0000;
        else if (note_counter >= frequency) //Our threshold frequency
            note_counter <= 15'h0000;       //reset the counter once we surpass it
        else 
            note_counter <= note_counter + 1;    

    /*
    Duration Counter
    */
    logic [24:0] timer;
    logic [23:0] duration;
    always_ff @(posedge clk, negedge rst_n)
        if(!rst_n)
            timer <= 24'h000000;
        else if(clr_timer)              //reset the timer on every state transition
            timer <= 24'h000000;
        else if(timer <= duration)
            timer <= timer + increment; //reset the timer when it reaches duration
        else timer <= 24'h000000;     
            
    /*
    State Machine
    */
    //Enumerated Types for State Machine
	typedef enum logic [2:0] {IDLE, N1, N2, N3, N4, N5, N6} state_t;
	state_t state, nxt_state;
    
    ////////////////////////////
    // Infer state flop next //
    //////////////////////////
    always @(posedge clk, negedge rst_n)
    if (!rst_n)
        state <= IDLE;
    else
        state <= nxt_state;

    /*
    State logic: start in idle and wait for go. Then, go through
    each of the 6 remaining states for each of the notes that 
    should be played. At each transition, the clr_timer signal
    is asserted and we set our threshold frequency and duration.
    */
    always_comb begin
        //default behavior  
        nxt_state = state;
        duration = 24'd8388608;        //2^23 clock cycles
        frequency = 15'hxxxx;
        clr_timer = 0;

        case(state)
        //NOTE 1
        N1 : begin
            duration = 24'd8388608;     // 2^23 clock cycles
            frequency = 15'd31888;      // 50E6 / 1568
            if(timer >= duration) begin
                nxt_state = N2;
                clr_timer = 1;
            end
        end

        //NOTE 2
        N2 : begin
            duration = 24'd8388608;    //2^23 clock cycles
            frequency = 15'd23889;     // 50E6 / 2093
            if(timer >= duration) begin
                nxt_state = N3;
                clr_timer = 1;
            end
        end

        //NOTE 3
        N3 : begin
            duration = 24'd8388608;    //2^23 clock cycles
            frequency = 15'd21124;     // 50E6 / 2637
            if(timer >= duration) begin
                nxt_state = N4;
                clr_timer = 1;
            end
        end

        //NOTE 4
        N4 : begin
            duration = 24'd12582912;    //2^23 + 2^22 clock cycles
            frequency = 15'd15944;     // 50E6 / 3136
            if(timer >= duration) begin
                nxt_state = N5;
                clr_timer = 1;
            end
        end 

        //NOTE 5
        N5 : begin
            duration = 24'd4194304;    //2^22 clock cycles
            frequency = 15'd21124;     // 50E6 / 2637
            if(timer >= duration) begin
                nxt_state = N6;
                clr_timer = 1;
            end
        end    

        // NOTE 6
        N6 : begin
            duration = 24'd16777215;    //2^24 clock cycles
            frequency = 15'd15944;     // 50E6 / 3136
            if(timer >= duration) begin
                nxt_state = IDLE;
                clr_timer = 1;
            end
        end     

        //Default case: wait for GO to be asserted then Charge!
        default : begin
            if(go) begin
                nxt_state = N1;
                clr_timer = 1;
            end
        end    

        endcase
    end

    //MSB of the counter, we dont care what the duty cycle is, so we can 
    //just set piezo to the MSB of our counter and call it a day, that will
    //produce our signal.       
    assign piezo = (state != IDLE) ? 
                    note_counter[14] || note_counter[13] :
                    1'b0;
    assign piezo_n = (state != IDLE) ? ~piezo : 1'b0;

endmodule