#include "redc.h"

#include <stdio.h>
#include <stdlib.h>

extern char **GS;

int redc_f_statement_block() {
  if (!peek('{'))
    return 0;
  next();
  int go_on = 1;
  while (go_on) {
    int is_valid = redc_f_statement();
    if (peek('}')) {
      next();
      go_on = 0;
      continue;
    }
    if (!is_valid)
      exit(1);

    if (peek(0)) {
      exit(1);
    }
  }
  return 1;
} /** parse statement block */
