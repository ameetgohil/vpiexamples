.PHONY: all

all:
	iverilog-vpi hello.cpp
	iverilog -ohello.vvp top.sv
	vvp -M. -mhello hello.vvp
