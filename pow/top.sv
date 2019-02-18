module top;
   reg               clk;

   wire [31:0]       val;
   
   reg [31:0]        val1,val2,val3;
   reg [31:0]        i;
   

   //assign val = $pow_test(2, val2);
   
   always @(posedge clk) begin
      val1 <= $pow_test(2,val2);
      val2 <= val2 + 1;
   //   $display(val1);
      
   end

   always #5 clk = ~clk;
   
   initial begin
      $dumpfile("test.vcd");
      $dumpvars(0,top);
      clk=0;
      val2=0;
      //val3=$pow_test(2,val2);
      #10;
      
      #100;
      $finish;
      
   end
endmodule // top
