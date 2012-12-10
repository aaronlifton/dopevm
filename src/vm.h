#ifndef _VM_H
#define _VM_H

#define MAGIC_CONST  0xdeadbeef
#define FILE_VER_NUM 01

typedef struct {
  int          magic;
  int          ver;
  unsigned int instrlen;
} prog_header;

typedef struct {
  char  *datastrptr;
  char **datastrs;
} data_segment;

typedef struct {
  prog_header  *hdr;
  int          *ip;
  data_segment *dataseg;
} program;

int run_program_static(int program);
int run_program_from_file(char *file);
int vm_engine(int *ip, char **dataseg);

#endif /* !defined(_VM_H) */
