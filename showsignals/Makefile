.PHONY: all

all:
	iverilog-vpi showsignals.cpp
	iverilog -oshowsignals.vvp top.sv
	vvp -M. -mshowsignals showsignals.vvp
