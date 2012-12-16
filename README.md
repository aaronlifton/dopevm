#dopevm

* RISC-like approach; all opcodes are of the same length and we use relative offsets for jumps.
* all opcodes are 32-bits in length (4 seperate bytes)
* there is also one special register, cmpR, which is set only by the 'compare' instruction which is op 7. it is 0 by default.
* if a compare via op 7 succeeds, then it is set to 1, and then set to 0 after any instruction which depends on its value is executed successfully (because they all essentially perform their operation based on cmpR being equal to 1.)

##program bytecodes:
   
    0,  x,  n,  0  -- set register x to value n
	
    0,  r1, r2, 1  -- set register r1 to == register r2
	
    1,  r1, r2, r3 -- add val in r2 to r3 and store in r1
	
    2,  r1, r2, r3 -- sub val in r2 by r3 and store in r1
	
    3,  r1, r2, r3 -- multiply val in r2 by r3 and store in r1
	
    4,  r1, r2, r3 -- divide val in r2 by r3 and store in r1
	
    5,  r1, r2, r3 -- mod val in r2 by r3 and store in r1
	
    6,  x,  0,  0  -- show reg x
	
    7,  r1, r2, 0  -- compare value in r1 to r2, if they are equal, set cmpR to 1
	
    7,  r,  v,  1  -- compare value in r to 'v', if they're equal, set cmpR to 1
	
    8,  n,  0,  0  -- jump n opcodes back/forwards (depends if it's positive/negative)
	
    8,  n,  0,  1  -- jump back n opcodes back/forwards iff cmpR == 1
	
    8,  n,  0,  2  -- jump back n opcodes back/forwards iff cmpR == 0
	
    9,  r,  0,  0  -- print string at data index 'r'
	
    10, r,  0,  0  -- push register 'n' to stack
	
    10, v,  0,  1  -- push value 'v' to stack
	
    11, r,  0,  0  -- pop stack value into register 'n'
	
    12, n,  0,  0  -- exit with value 'n'
	
*see programs.h for all programs*

## How to write and execute a program in the vm

1. Write an array in C with on op per line, in programs.h. This must be `program5`.

2. Write this program to a binary file, a program runnable by the vm. `./writer 5`

3. `/.dopevm file.dope`