// look in pins.pcf for all the pin names on the TinyFPGA BX board
module top (
    input CLK,    // 16MHz clock
    output LED,   // User/boot LED next to power LED
    output USBPU  // USB pull-up resistor
    ,output PIN_14
);
    // drive USB pull-up resistor to '0' to disable USB
    assign USBPU = 0;
	 wire clk_out;
	 reg reset = 0;
	 reg on = 0;

	 clk_div t1(CLK,reset,clk_out);

    // increment the blink_counter every clock
    always @(posedge clk_out) begin
	 	if(on==0) 
			begin
				on = 1;
			end
		else
			begin
				on = 0;
			end
		LED = on;
		PIN_14 = on;
    end

endmodule
