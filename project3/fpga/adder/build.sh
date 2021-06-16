#!/bin/bash
iverilog -o fulladdrout fulladdr.v fulladdrtb.v
vvp fulladdrout
gtkwave test.vcd
