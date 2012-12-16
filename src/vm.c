#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <gc/gc.h>

#include <math.h>
#include "file.h"
#include "util.h"
#include "programs.h"

/* RISC-like approach; all opcodes are of the same length
   and we use relative offsets for jumps. all opcodes are 32-bits
   in length (4 seperate bytes)

   there is also one special register, cmpR, which is set only by the
   'compare' instruction which is op 7. it is 0 by default. if a compare
   via op 7 succeeds, then it is set to 1 and 0 after any instruction which
   depends on its value is executed successfully (because they all essentially
   perform their operation based on the fact cmpR == 1.)

   program bytecodes:
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

    see programs.h for all programs
  */

// special compare register
#define SPECIAL_REG_NUM 1
typedef enum { CMPR = 0 } SpecialRegs;

// runs the programs above with their static data section
int run_program_static(int program) {
  int *ip;
  int ret = 0;

  if (program == 1)
    ip = program1;
  else if(program == 2)
    ip = program2;
  else if(program == 3)
    ip = program3;
  else if(program == 4)
    ip = program4;
  else if(program == 5)
    ip = program5;
  else {
    printf("Incorrect program\n");
    exit(-1);
  }

  ret &= vm_engine(ip, staticdata);

  return ret;
}


// executes a program from a file
int run_program_from_file(char* file) {
  program *prog = (program*)load_program_from_file(file);
  int ret = 0;

  if(prog == NULL) {
    printf("Error loading from file.\n"); ret = -1;
  } else {

    ret = vm_engine(prog->ip,prog->dataseg->datastrs);
  }

  return ret;
}

typedef struct _p_stack {
  int val;
  struct _p_stack *prev;
} p_stack;

// main vm engine, runs a program with a specified data
// segment
int vm_engine(int *ip, char **datastrs) { 
  void *func_table[] = { &&set,      // byte 0
			 &&add,      // byte 1
			 &&sub,      // byte 2
			 &&mul,      // byte 3
			 &&div,      // byte 4
			 &&mod,      // byte 5
			 &&show,     // byte 6
			 &&compare,  // byte 7
			 &&jump,     // byte 8
			 &&printstr, // byte 9
			 &&push,     // byte 10
			 &&pop,      // byte 11
			 &&exit };   // byte 12

  int  regs[32]                  = { 0, };     // 32 registers
  int  specregs[SPECIAL_REG_NUM] = { 0, };     // special registers
  p_stack *stack, *stack_tmp;

  printf("Initializing stack...\n");
  stack = GC_malloc(sizeof(*stack));
  stack->prev = NULL; stack->val = 0;

  goto *func_table[ip[0]];

 set:
  if (ip[3] == 0) {
    printf("[op 0] setting reg %d to val %d\n",ip[1],ip[2]);
    regs[ip[1]] = ip[2];
  } else if (ip[3] == 1) {
    printf("[op 0] setting reg %d to reg%d(%d)\n",ip[1],ip[2],regs[ip[2]]);
    regs[ip[1]] = regs[ip[2]];
  }
  
  ip += 4; 
  printf("[op 0] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

 add:
  printf("[op 1] setting reg%d = reg%d + reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] + regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] + regs[ip[3]];
  ip += 4;

  printf("[op 1] next op is %d\n",ip[0]);
  
  goto *func_table[ip[0]];
  
 sub:
  printf("[op 2] setting reg%d = reg%d - reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] - regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] - regs[ip[3]];
  ip += 4;
  
  printf("[op 2] next op is %d\n",ip[0]);
  
  goto *func_table[ip[0]];
  
 mul:
  printf("[op 3] setting reg%d = reg%d * reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] * regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] * regs[ip[3]];
  ip += 4;
  
  printf("[op 3] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];
  
 div:
  printf("[op 4] setting reg%d = reg%d / reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] / regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] / regs[ip[3]];
  ip += 4;
  
  printf("[op 4] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];
  
 mod:
  printf("[op 5] setting reg%d = reg%d %% reg%d (%d)\n",
		ip[1],ip[2],ip[3], regs[ip[2]] % regs[ip[3]]);
  
  regs[ip[1]] = regs[ip[2]] % regs[ip[3]];
  ip += 4;
  
  printf("[op 5] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];
  
 show:
  printf("[op 6] reg %d ==> %d\n",ip[1],regs[ip[1]]);
  ip += 4; 

  printf("[op 6] next op is %d\n",ip[0]);

  goto *func_table[ip[0]];

 compare:
  
  if(ip[3] == 0) {
	  printf("[op 7] comparing reg%d to reg%d\n",
	 		  ip[1], ip[2]);

    if(regs[ip[1]] == regs[ip[2]])
      specregs[CMPR] = 1;
    else
      specregs[CMPR] = 0;
  } else if (ip[3] == 1) {
	  printf("[op 7] comparing reg%d to '%d'\n",
		  ip[1], ip[2]);

    if(regs[ip[1]] == ip[2])
      specregs[CMPR] = 1;
    else
      specregs[CMPR] = 0;
  }

  ip += 4;

  printf("[op 7] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];

 jump:

  if(ip[3] == 0) {
    printf("[op 8] jumping %s %d opcodes\n",
		  ip[1] > 0 ? "forward" : "back",abs(ip[1]));

    ip += (ip[1]*4);
  } else if(ip[3] == 1) {
    printf("[op 8] jumping %s %d opcodes if cmpR == 1\n",
		  ip[1] > 0 ? "forward" : "back",abs(ip[1]));

    if(specregs[CMPR] == 1)
      ip += (ip[1]*4);
    else
      ip += 4;
  } else if(ip[3] == 2) {
    printf("[op 8] jumping %s %d opcodes if cmpR == 0\n",
		  ip[1] > 0 ? "forward" : "back",abs(ip[1]));

    if(specregs[CMPR] == 0)
      ip += (ip[1]*4);
    else
      ip += 4;
  }

  printf("[op 8] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];

 printstr:
  printf("[op 9] printing string, data index %d\n",ip[1]);

  printf("%s\n",(char*)datastrs[ip[1]]);
  ip += 4;
  
  printf("[op 9] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];

 push:
  stack_tmp = GC_malloc(sizeof(*stack_tmp));
  if(ip[4] == 0)      stack_tmp->val = regs[ip[1]];
  else if(ip[3] == 1) stack_tmp->val = ip[1];

  stack_tmp->prev = stack;
  stack = stack_tmp; stack_tmp = NULL;

  ip += 4;
  printf("[op 10] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];

 pop:
  if(stack == NULL) panic("Stack underflow");
  regs[ip[1]] = stack->val;
  stack = stack->prev;

  ip += 4;
  printf("[op 11] next op is %d\n",ip[0]);
  goto *func_table[ip[0]];

 exit:
  printf("[op 12] exiting with value %d\n",ip[1]);
  return ip[1];
}
