module top
  (input wire[7:0] data,
   output logic[7:0] odata
//   input clk
   );
   reg               clk;
   
   reg [7:0]   val1,val2,val3;
   

   always @(posedge clk) begin
     odata <= data << 2;
      /*$display(val1);
      $display(val2);
      $display(val3);*/
   end

   always #5 clk = ~clk;
   
initial begin
   $dumpfile("test.vcd");
   $dumpvars(0,top);
   clk=0;
   $delay(clk);
   //$delay;
   
   
   //$hello;
   $show_value(val1);
   $task_test(2, val1, val2, val3);
   $show_value(val2);
   
   /*$display(val1);
   $display(val2);
   $display(val3);*/
   #10;
   //$hello;
   #100;
   $finish;
   
end
endmodule // top
