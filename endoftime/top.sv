module add
  (input wire[31:0] a,
   input wire[31:0] b,
   input wire clk,
   output reg[31:0] out
   );

   always @(posedge clk) 
      out <= a + b;
endmodule

module top;
   reg               clk;
   integer               j;
   real              k;
   time              s;

   
   wire [31:0]       val;
   
   reg [31:0]        val1,val2;
   wire [31:0] val3;
   reg [31:0]        i;

   add add_0
     (.a(val1),
      .b(val2),
      .clk(clk),
      .out(val3)
      );
   

   //assign val = $pow_test(2, val2);
   
   always @(posedge clk) begin
      val2 <= val2 + 1;
   end

   always #5 clk = ~clk;
   
   initial begin
      $dumpfile("test.vcd");
      $dumpvars(0,top);
      clk=0;
      val1=5;
      val2=0;
      j=2;
      k=2.2;
      s=10;
      
      #10;
      $end_of_time(s);
      $display("salsdkf");
      #10;
      #20;
      s=20;
      
      #100;
      $finish;
      
   end
endmodule // top
