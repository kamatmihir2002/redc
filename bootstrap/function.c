#include "redc.h"

#include <stdio.h>
#include <stdlib.h>

extern char **GS;

int redc_f_paren_expr() {
  if (!peek('('))
    return 0;
  
 
  next();
  
  int breakloop = 0;
  
  while (!breakloop) {
  
    int e = redc_f_expression(0);
    if (peek(',')) {
      next();
      continue;
    }
  
    if (peek(')')) {
      next();
      breakloop = 1;
      continue;
    }
    _err(1);
 
  }
 
  return 1;
}

int redc_f_function(int type) {
  int tok = peek_nocheck();

  if (tok != identifier)
    return 0;

  if (type == FUNC_DECL_DEF) {
    next();
    return 1;
  } else if (type == FUNC_CALL) {
    next();
    redc_f_paren_expr();
  }

  return 1;
}
