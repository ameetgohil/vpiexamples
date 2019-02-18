.PHONY: all

all:
	iverilog-vpi shownets.cpp
	iverilog -oshownets.vvp top.sv
	vvp -M. -mshownets shownets.vvp
