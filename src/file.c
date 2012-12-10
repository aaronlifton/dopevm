#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gc/gc.h>
#include "util.h"
#include "file.h"
#include "vm.h"

program *load_program_from_file(char* file) {
  program      *prog       = NULL;
  FILE         *fd         = NULL;
  long          dataoffset = 0;
  int           j          = 0;
  int           dseg_size  = 0;
  int           tmp        = 0;
  data_segment  *dataseg   = NULL;
  char          *ptr       = NULL;
  char          ch         = 0;

#define __fileread(el, size, nmel, fd, file, lineno) \
  if (fread(el, size, nmel, fd) != nmel) { \
    fprintf(stderr, "* Error: Invalid file format, not a valid program at line %d of %s!\n", lineno, file); \
    goto error; \
  }

#define fileread(el, size, nmel, fd) __fileread(el, size, nmel, fd, __FILE__, __LINE__)

  fd = fopen(file,"rb");
  if (!fd)
    goto error;

  prog = GC_malloc(sizeof(*prog));
  if (!prog)
    goto error;

  prog->hdr = GC_malloc(sizeof(*prog->hdr));
  if (!prog->hdr)
    goto error;
 
  // load and check magic number
  fileread(&prog->hdr->magic, sizeof(int), 1, fd);

  if(prog->hdr->magic != MAGIC_CONST) {
    printf("load_program_from_file: err, magic number invalid. should be 0x%x, is instead 0x%x\n",
	   MAGIC_CONST,prog->hdr->magic);
    goto error;
  }

  // read length and version
  fileread(&prog->hdr->ver, sizeof(int),1,fd);
  fileread(&prog->hdr->instrlen, sizeof(int), 1,fd);

  // allocate memory for ip and read data
  // note: we have to allocate instrlen*sizeof(int));
  // data here, since instrlen does not take the
  // size of the actual type it is into account; you'll
  // notice the writer program does divides the size of
  // the program from the size of the type (int)
  prog->ip = GC_malloc(prog->hdr->instrlen*sizeof(int));
  fileread(prog->ip,sizeof(int),prog->hdr->instrlen,fd);

  dataoffset = ftell(fd);
  for(;;dseg_size++)
    if (fread(&ch,sizeof(char),1,fd) != 1)
      break;

  if(dseg_size == 0) goto end;

  fseek(fd,dataoffset,SEEK_SET);
  // allocate memory and copy data over
  dataseg             = GC_malloc(sizeof(data_segment));
  dataseg->datastrs   = GC_malloc(dseg_size * sizeof(char**));
  dataseg->datastrptr = GC_malloc(dseg_size+2);
  fileread(dataseg->datastrptr,sizeof(char),dseg_size,fd);

  ptr = dataseg->datastrptr;
  dataseg->datastrs[j++] = ptr; // first is obvious
  for(tmp = 0; tmp < dseg_size; tmp++,ptr++) {
    if(tmp+1 == dseg_size) break;
    if(*ptr == L'\00') {
      dataseg->datastrs[j] = ptr+1;
      j++;
    }
  }

 end:
  prog->dataseg = dataseg;
  fclose(fd);
  return prog;
error:
  if (fd)
    fclose(fd);

  return NULL;
#undef fileread
#undef __fileread
}

int write_program_to_file(int instrlen, int *ip,int datalen, char **data, char *file) {
  FILE *fd = NULL;
  int magic    = MAGIC_CONST;  // we have to provide a ptr to fwrite
  int ver      = FILE_VER_NUM; // ditto
  int i        = 0;
  char nul  = L'\00';

  fd = fopen(file,"wb");
  if(fd == NULL) {
    printf("write_program_to_file: err, couldn't open file\n");
    return -1;
  }

  // write header and instrs
  fwrite(&magic,sizeof(int),1,fd);
  fwrite(&ver,sizeof(int),1,fd);
  fwrite(&instrlen,sizeof(int),1,fd);
  fwrite(ip,sizeof(int),instrlen,fd);

  printf("write_program_to_file: wrote %d ints to '%s'\n",instrlen,file);

  // write data 
  for(; i < datalen; i++) {
    fwrite(data[i],sizeof(char),strlen(data[i]),fd);
    fwrite(&nul,sizeof(char),1,fd);
  }

  for(; i < datalen; i++)
    fputs(data[i],fd);

  fclose(fd);
  return 0;
}
