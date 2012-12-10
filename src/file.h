#ifndef _DOPE_FILE_H_
#define _DOPE_FILE_H_
#include "vm.h"

program *load_program_from_file(char* file);
int write_program_to_file(int instrlen,    // program length
			  int *ip,         // pointer to opcodes
			  int datalen,     // length of data segment
			  char **data,  // data segment
			  char *file);     // file to write to

#endif
