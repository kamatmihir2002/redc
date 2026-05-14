#include "redc.h"

#include <stdio.h>
#include <stdlib.h>

extern char **GS;

int redc_f_paren_decls() {
  if (!peek('('))
    return 0;

  next();
  int breakloop = 0;

  while (!breakloop) {

    int e = redc_f_decl();

    if (peek(',') || peek(')')) {
      char paren_or_comma = next();
      if (paren_or_comma == ')')
        breakloop = 1;
      continue;
    }

    exit(1);
  }

  return 1;
}

int redc_f_paren_expr() {
  if (!peek('('))
    return 0;

  next();

  int breakloop = 0;

  while (!breakloop) {

    int e = redc_f_expression(0);

    if (peek(',') || peek(')')) {
      char paren_or_comma = next();
      if (paren_or_comma == ')')
        breakloop = 1;
      continue;
    }

    exit(1);
  }

  return 1;
}

int redc_f_function(int type) {
  int tok = peek_nocheck();

  if (tok != identifier)
    return 0;

  next();
  var function_name = redc_s_vvar();

  if (type == FUNC_DECL_DEF) {
    /** first the definition, then the params */
    next();

    redc_b_state_add_definition(function_name);
    redc_b_state_advance();

    redc_f_paren_decls();

    if (peek(';')) {
      next();
      return 1;
    }

    if (peek('{')) {
      return redc_f_statement_block();
    }

    exit(1);

  } else if (type == FUNC_CALL) {
    /** first the params, then the call */
    next();

    redc_f_paren_expr();

    redc_b_state_add_call(function_name);
    redc_b_state_advance();

    redc_s_var_push(redc_s_vcallresult());
  }

  return 1;
}
