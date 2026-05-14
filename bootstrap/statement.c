#include "redc.h"

#include <stdio.h>
#include <stdlib.h>

extern char **GS;

int redc_f_statement() {
  int st = peek_nocheck();
  if (st != kw_if && st != identifier && st != kw_continue && st != kw_return) {
    return 0;
  }
  if (kwpeek(if)) {
    next();
    if (!match('(')) {
      exit(1);
    }
    int cond = redc_f_expression(0);
    if (!match('(')) {
      exit(1);
    }

    int sblock = redc_f_statement_block();

    return 1;
  }
  if (kwpeek(continue)) {
    next();
    if (!match(';')) {
      exit(1);
    }

    return 1;
  }
  if (kwpeek(return )) {
    next();
    int r_expr = redc_f_expression(0);

    if (!match(';')) {
      exit(1);
    }

    return 1;
  }
  if (peek(identifier)) {
    int l = redc_f_expression(0);
    if (peek('=')) {
      /** expression assign */
      next();
      int r = redc_f_expression(0);

      if (!match(';')) {
        exit(1);
      }

      return 1;
    }
    if (peek(';')) {
      /** variable declaration */
      next();
      return 1;
    }
    exit(1);
  }
  return 0;
}
