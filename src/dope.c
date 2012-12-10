#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <gc/gc.h>
#include "util.h"
#include "vm.h"

static struct option opts[] = {
  { "help",       no_argument,       NULL, 'h' },
  { "program",     required_argument, NULL, 'p' },
  { NULL, 0, NULL, 0 }
};

void version() {
  printf("dope v0.1: simple virtual machine\n");
  exit(EXIT_SUCCESS);
}

void usage() {
  printf("usage: executable [options] [file]...\n");
  printf("'file' is a bytecode file to load.\n");
  printf("available options:\n");
  printf(" --help\t\t-h\t\tshow this information\n");
  printf(" --program=n\t-p n\t\texecute program 'n'\n");
  exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]) {
  char c;
  int ret =  0;
  int prog = -1;

  GC_init();
  
  // parse command line arguments
  while((c = getopt_long(argc, argv, "hp:i:", opts, NULL)) != -1) {
    switch(c) {
    case 'p':
      prog = atoi(optarg);
      break;
    case 'h':
    case '?':
    default:
      usage();
    }
  }

  argc -= optind;
  argv += optind;

  if(argv[0]) {
    printf("Executing file %s \n",argv[0]);
    ret = run_program_from_file(argv[0]);
  } else {
    if(prog == -1) {
      printf("Executing program1:\n"); ret &= run_program_static(1);
      printf("Executing program2:\n"); ret &= run_program_static(2);
      printf("Executing program3:\n"); ret &= run_program_static(3);
      printf("Executing program4:\n"); ret &= run_program_static(4);
    } else {
      printf("Running program.\n");
      ret = run_program_static(prog);
    }
  }
  
  return ret;
}
  