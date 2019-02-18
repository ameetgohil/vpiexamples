.PHONY: all

all:
	iverilog-vpi endoftime.cpp
	iverilog -oendoftime.vvp top.sv
	vvp -M. -mendoftime endoftime.vvp
