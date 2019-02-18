.PHONY: all

all:
	iverilog-vpi pow.cpp
	iverilog -opow.vvp top.sv
	vvp -M. -mpow pow.vvp
