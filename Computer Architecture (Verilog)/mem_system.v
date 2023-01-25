`default_nettype none
/*
   TWO-WAY SET-ASSOCIATIVE CACHE
*/
module mem_system(
   // Outputs
   DataOut, Done, Stall, CacheHit, err, 
   // Inputs
   Addr, DataIn, Rd, Wr, createdump, clk, rst
   );
   
   ///////////////////////////////
   //          Inputs          //
   /////////////////////////////
   // module inputs
   input wire [15:0] Addr;
   input wire [15:0] DataIn;
   input wire        Rd;
   input wire        Wr;
   input wire        createdump;
   input wire        clk;
   input wire        rst;

   // four bank mem inputs
   reg[15:0] data_out_mem;
   reg[15:0] addr_mem;
   reg wr_mem;
   reg rd_mem;

   // cache inputs
   reg enable;
   reg[4:0] tag;
   reg[7:0] index;
   reg[2:0] offset;
   reg[15:0] data_out_cache;
   reg comp;
   reg write;
   reg valid_out;

   ///////////////////////////////
   //        Outputs           //
   /////////////////////////////
   // module outputs
   output wire [15:0] DataOut;
   output reg Done;
   output reg Stall;
   output reg CacheHit;
   output wire err;

   // four bank mem outputs
   wire[15:0] data_in_mem;
   wire stall_in_mem;
   wire[3:0] busy_mem;
   wire err_mem;

   // cache outputs
   wire hit1, hit2, hit;
   wire dirty1, dirty2, dirty;
   wire[4:0] tag_in_cache1, tag_in_cache2, tag_in;
   wire[15:0] data_in_cache1, data_in_cache2, data_in_cache;
   wire valid1, valid2, valid;
   wire err_cache1, err_cache2;

   // Errors
   reg err_controller;
   assign err = err_controller | err_cache1 | err_cache2 | err_mem;

   // Victim way
   wire cache_select;
   reg invert_way;

   // DFF for storing the current victim
   wire victim_way, victim_way_in;
   // Invert and hold the value of the victim. Initialize to 0.
   assign victim_way_in = rst ? 1'b0 : invert_way ? ~victim_way : victim_way;
   dff victim (.d(victim_way_in), .q(victim_way), .clk(clk), .rst(rst));

   // Evict the line that is not valid or evict according to victim way
   assign cache_select = (hit1 & valid1) ?   1'b0 : 
                         (hit2 & valid2) ?   1'b1 : 
                         (~valid1) ?         1'b0 :
                         (~valid2) ?         1'b1 :
                         victim_way;
   
   assign DataOut = data_in_cache;
   assign data_in_cache = cache_select ? data_in_cache2 : data_in_cache1;
   assign tag_in = cache_select ? tag_in_cache2 : tag_in_cache1;
   assign dirty = cache_select ? dirty2 : dirty1;
   assign valid = cache_select ? valid2 : valid1;
   assign hit = cache_select ? hit2 : hit1;

   wire write1, write2;
   assign write1 = write & ~cache_select;
   assign write2 = write & cache_select;

   ///////////////////////////////
   //        Cache             //
   /////////////////////////////
   /* data_mem = 1, inst_mem = 0 *
    * needed for cache parameter */
   parameter memtype = 0;
   cache #(0 + memtype) c0(// Outputs
                          .tag_out              (tag_in_cache1),
                          .data_out             (data_in_cache1),
                          .valid                (valid1),
                          .hit                  (hit1),
                          .dirty                (dirty1),
                          .err                  (err_cache1),
                          // Inputs
                          .tag_in               (tag),
                          .data_in              (data_out_cache),
                          .valid_in             (valid_out),
                          .write                (write1),
                           .*);
   cache #(2 + memtype) c1(// Outputs
                           .tag_out              (tag_in_cache2),
                           .data_out             (data_in_cache2),
                           .valid                (valid2),
                           .hit                  (hit2),
                           .dirty                (dirty2),
                           .err                  (err_cache2),
                           // Inputs
                           .tag_in               (tag),
                           .data_in              (data_out_cache),
                           .valid_in             (valid_out),
                           .write                (write2),
                            .*);
   
   ///////////////////////////////
   //    Four Bank Memory      //
   /////////////////////////////
   four_bank_mem mem(// Outputs
   .data_out          (data_in_mem),
   .stall             (stall_in_mem),
   .busy              (busy_mem),
   .err               (err_mem),
   // Inputs
   .addr              (addr_mem),
   .data_in           (data_out_mem),
   .wr                (wr_mem),
   .rd                (rd_mem),
   .*);
   
   ///////////////////////////////
   // state definitions for SM //
   /////////////////////////////
   localparam  IDLE = 4'd0, 
               READ_DONE = 4'd1, 
               DONE = 4'd2, 
               COMPARE_READ = 4'd3, 
               COMPARE_WRITE = 4'd4, 
               ACCESS_WRITE_0 = 4'd5,
               ACCESS_WRITE_1 = 4'd6,
               ACCESS_WRITE_2 = 4'd7,
               ACCESS_WRITE_3 = 4'd8, 
               ACCESS_WRITE_4 = 4'd9,
               ACCESS_WRITE_5 = 4'd10, 
               ACCESS_READ_0 = 4'd11,
               ACCESS_READ_1 = 4'd12,
               ACCESS_READ_2 = 4'd13,
               ACCESS_READ_3 = 4'd14,
               HIT = 4'd15;

    wire [3:0] state;
    reg [3:0] next_state;
    
    ///////////////////////////
    // State maintaining FF //
    /////////////////////////
    dff state_ff [3:0] (.d(next_state), .q(state), .*);

    //////////////////////////
    // State Machine Logic //
    ////////////////////////
    always @(*) begin
      // default all SM outputs
      enable = 1'b1; 
      tag = Addr[15:11];
      index = Addr[10:3];
      offset = Addr[2:0];
      data_out_mem = DataOut;
      data_out_cache = DataIn;
      comp = 1'b0;
      write = 1'b0;
      valid_out = 1'b0;
      addr_mem = 16'hxxxx;
      wr_mem = 1'b0;
      rd_mem = 1'b0;
      CacheHit = 1'b0;
      Stall = 1'b1;
      Done = 1'b0;
      invert_way = 1'b0;
      err_controller = 1'b0;
 
      case (state)
            IDLE : begin
               Stall = Wr | Rd;
               enable = 1'b0;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = Addr[2:0];
               invert_way = Wr | Rd;
               next_state =   Wr ? COMPARE_WRITE : 
                              Rd ? COMPARE_READ : 
                              IDLE;
            end

            COMPARE_READ : begin
               comp = 1'b1;
               write = 1'b0;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = Addr[2:0];

               next_state = (hit & valid) ? HIT : 
                            (~hit & dirty & valid) ? ACCESS_READ_0 : 
                            ( (~hit & ~dirty) | (hit & ~valid) ) ? ACCESS_WRITE_0 :
                            COMPARE_READ;
            end

            COMPARE_WRITE : begin
               comp = 1'b1;
               write = 1'b1;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = Addr[2:0];

               next_state = (hit & valid) ? HIT : 
                            (~hit & dirty & valid) ? ACCESS_READ_0 : 
                            (~valid | (~hit & ~dirty) | (hit & ~valid)) ? ACCESS_WRITE_0 :
                            COMPARE_WRITE;
            end
            
            ACCESS_WRITE_0 : begin
               comp = 1'b0;
               write = 1'b1;
               addr_mem = {Addr[15:3], 3'b000};
               rd_mem = 1'b1;

               next_state = ACCESS_WRITE_1;
            end
            
            ACCESS_WRITE_1 : begin
               comp = 1'b0;
               write = 1'b1;
               addr_mem = {Addr[15:3], 3'b010};
               rd_mem = 1'b1;

               next_state = ACCESS_WRITE_2;
            end

            ACCESS_WRITE_2 : begin
               comp = 1'b0;
               write = 1'b1;
               valid_out = 1'b1;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = 3'b000;
               data_out_cache = data_in_mem;
               addr_mem = {Addr[15:3], 3'b100};
               rd_mem = 1'b1;

               next_state = ACCESS_WRITE_3;
            end

            ACCESS_WRITE_3 : begin
               comp = 1'b0;
               write = 1'b1;
               valid_out = 1'b1;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = 3'b010;
               data_out_cache = data_in_mem;
               addr_mem = {Addr[15:3], 3'b110};
               rd_mem = 1'b1;

               next_state = ACCESS_WRITE_4;
            end

            ACCESS_WRITE_4 : begin
               comp = 1'b0;
               write = 1'b1;
               valid_out = 1'b1;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = 3'b100;
               data_out_cache = data_in_mem;

               next_state = ACCESS_WRITE_5;
            end

            ACCESS_WRITE_5 : begin
               comp = 1'b0;
               write = 1'b1;
               valid_out = 1'b1;
               tag = Addr[15:11];
               index = Addr[10:3];
               offset = 3'b110;
               data_out_cache = data_in_mem;

               next_state = Rd ? READ_DONE : DONE;
            end

            ACCESS_READ_0 : begin
               comp = 1'b0;
               write = 1'b0;
               index = Addr[10:3];
               offset = 3'b0;
               addr_mem = {tag_in, Addr[10:3], offset};
               data_out_mem = data_in_cache;
               wr_mem = 1'b1;

               next_state = ACCESS_READ_1;
            end

            ACCESS_READ_1 : begin
               comp = 1'b0;
               write = 1'b0;
               index = Addr[10:3];
               offset = 3'b010;
               addr_mem = {tag_in, Addr[10:3], offset};
               data_out_mem = data_in_cache;
               wr_mem = 1'b1;

               next_state = ACCESS_READ_2;
            end

            ACCESS_READ_2 : begin
               comp = 1'b0;
               write = 1'b0;
               index = Addr[10:3];
               offset = 3'b100;
               addr_mem = {tag_in, Addr[10:3], offset};
               data_out_mem = data_in_cache;
               wr_mem = 1'b1;

               next_state = ACCESS_READ_3;
            end
            
            ACCESS_READ_3 : begin
               comp = 1'b0;
               write = 1'b0;
               index = Addr[10:3];
               offset = 3'b110;
               addr_mem = {tag_in, Addr[10:3], offset};
               data_out_mem = data_in_cache;
               wr_mem = 1'b1;
               next_state = ACCESS_WRITE_0;
            end

            HIT : begin
               Stall = 1'b0;
               Done = 1'b1;
               CacheHit = 1'b1;
               next_state = IDLE;
            end

            DONE : begin
               Stall = 1'b0;
               Done = 1'b1;
               comp = 1'b1;
               write = 1'b1;
               data_out_cache = DataIn;
               next_state = IDLE;
            end

            READ_DONE : begin
               Stall = 1'b0;
               Done = 1'b1;
               next_state = IDLE;
            end

            default : begin
               err_controller = 1'b1;
            end
      endcase
    end
   
endmodule
`default_nettype wire