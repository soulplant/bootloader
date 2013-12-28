#include "kutil.h"

void* memset(void* s, int c, size_t n) {
  int i;
  char* m = s;
  for (i = 0; i < n; i++)
    m[i] = c;
}
