.PHONY: all

all:
	iverilog-vpi userdata.cpp
	iverilog -ouserdata.vvp top.sv
	vvp -M. -muserdata userdata.vvp
