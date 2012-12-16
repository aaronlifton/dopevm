#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <gc/gc.h>
#include "programs.h"
#include "util.h"
#include "file.h"

void usage(char *me) {
  printf("usage: %s <n> <file>\n",me);
  printf("\tn is program to write (1-5) and file is where to write to\n");
  exit(-1);
}

int main(int ac, char **av) {
  int   progno, len;
  int  *progptr;

  if(ac < 3)
    usage(av[0]);

  GC_init();

  progno = atoi(av[1]);

  switch(progno) {
  case 1:  progptr = program1; len = sizeof(program1)/sizeof(int); break;
  case 2:  progptr = program2; len = sizeof(program2)/sizeof(int); break;
  case 3:  progptr = program3; len = sizeof(program3)/sizeof(int); break;
  case 4:  progptr = program4; len = sizeof(program4)/sizeof(int); break;
  case 5:  progptr = program4; len = sizeof(program5)/sizeof(int); break;
  default: usage(av[0]);
  }

  printf("Writing program%d[%d] to file `./%s'\n",progno,len,av[2]);
  int numstatic = sizeof(staticdata)/sizeof(char*); // num of static strings available to vm
  return write_program_to_file(len,progptr,numstatic,staticdata,av[2]);
}
