#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"

void panic(char *fmt, ...) {
  va_list va;
  va_start(va,fmt);
  printf("Panic: "); vprintf(fmt,va);
  va_end(va);
  exit(-1);
}
