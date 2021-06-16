#Full Adder

We will continue to learn more examples with Combinational Circuit - this time a full adder. A combinational circuit is one in which the present output is a function of only the present inputs - there is no memory. This is different from the sequential circuits that we will learn later where the present output is a function of not only the present input but also of past inputs/outputs as well.


Truth Table: 

 |Carry in|Input y|Input x|Carry out|Output A|  
 |--------|-------|-------|---------|--------|
 |   0    |  0		|  0	  |   0	   |   0    |
 |   0	 |  0	   |  1	  |   0	   |   1    |
 |   0	 |  1	   |  0	  |   0	   |   1    |
 |   0	 |  1	   |  1	  |   1	   |   0    |
 |   1	 |  0	   |  0	  |   0	   |   1    |
 |   1	 |  0    |	1	  |   1	   |   0    |
 |   1	 |  1	   |  0	  |   1	   |   0    |
 |   1	 |  1	   |  1	  |   1	   |   1    |


get output
iverilog -o output fulladder.v fulladdertb.v
vvp output

