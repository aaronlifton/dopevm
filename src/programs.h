#ifndef _PROGRAMS_H
#define _PROGRAMS_H

char* staticdata[] = { "hello world!", "goodbye world!" };

// addition
int program1[] = { 0,  0, 10, 0,   // reg 0 = 10
                   0,  1, 20, 0,   // reg 1 = 20
                   1,  1, 1,  0,   // reg 1 = reg1 + reg0
                   6,  1, 0,  0,   // show reg 1
                   12, 0, 0,  0 }; // exit

// loop
int program2[] = { 0, 0,    10,   0,   // reg 0 = 10
                   0, 1,    1,    0,   // reg 1 = 1
                   6, 0,    0,    0,   // show reg 0
                   2, 0,    0,    1,   // reg 0 = reg 0 - reg 1
                   7, 0,    0,    1,   // compare reg 0 to 0
                   8, (-3), 0,    2,   // jump back 3 instrs if r0 != 0
                   12, 0,    0,   0 }; // exit

// hello world
int program3[] = { 9,  0, 0, 0,   // print "hello world!"
                   9,  1, 0, 0,   // print "goodbye world!"
                   12, 0, 0, 0 }; // exit

// stack ops, push & pop from stack
int program4[] = { 10, 3, 0, 1, // push value 3 to stack
           10, 2, 0, 1, // push value 2 to stack
           10, 1, 0, 1, // push value 1 to stack
           11, 1, 0, 0, // pop to register 1
           11, 2, 0, 0, // pop to register 2
           11, 3, 0, 0, // pop to register 3
           6,  1, 0, 0, // print reg 1
           6,  2, 0, 0, // print reg 2
           6,  3, 0, 0, // print reg 3
           12, 0, 0, 0 } ;

int program5[] = {0,0,0,0,   // reg 0 = 0
                  6,0,0,0,   // show reg 0
                  12,0,0,0}; // exit

#endif
