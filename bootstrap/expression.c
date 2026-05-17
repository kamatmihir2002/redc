#include "redc.h"

#include <stdio.h>
#include <stdlib.h>

extern char **GS;

char *ops = "||&&==!=>\0<\0+\0-\0*\0/";

short binding_power(short op) {
  char maxp[2] = "ab";
  char decr[10] = {5, 4, 3, 3, 2, 2, 1, 1, 0, 0};

  short incrs = (10) << 8 | (10);
  for (int i = 0; i < 10; i++) {
    if (op == (short)ops[2 * i]) {
      return *(short *)maxp - decr[i] * incrs;
    }
  }

  return -1;
}

short do_op(short op, var *a, var *b) {

  for (int i = 0; i < 10; i++) {
    if (op == (short)ops[2 * i]) {
      switch (i) {
      case 0:
        a->constval = a->constval || b->constval;
        return 1;
      case 1:
        a->constval = a->constval && b->constval;
        return 1;
      case 2:
        a->constval = a->constval == b->constval;
        return 1;
      case 3:
        a->constval = a->constval != b->constval;
        return 1;
      case 4:
        a->constval = a->constval > b->constval;
        return 1;
      case 5:
        a->constval = a->constval < b->constval;
        return 1;
      case 6:
        a->constval = a->constval + b->constval;
        return 1;
      case 7:
        a->constval = a->constval - b->constval;
        return 1;
      case 8:
        a->constval = a->constval * b->constval;
        return 1;
      case 9:
        a->constval = a->constval / b->constval;
        return 1;
      }
    }
  }

  return -1;
}

int redc_f_operation() {
  int tk = 0;
  if (peek('+') || peek('-') || peek('*') || peek('/') || peek('>') ||
      peek('<')) {
    int rt = tk | ((char)next());
    return rt;
  } else if (peek('&') || peek('|') || peek('!') || peek('=')) {
    tk = ((char)next()) << 8;
    if (peek('&') || peek('|') || peek('=')) {
      tk = tk | ((char)next());
      return tk;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

int redc_f_expression_atom() {
  int tk = peek_nocheck();
  if (tk != identifier && tk != numeric && tk != '(')
    return 0;
  if (tk == identifier) {
    /** todo add function call */
    /** todo add arr index */

    /** normal variable */
    var k = redc_s_vvar();
    redc_s_var_push(k);
    next();
    return 1;
  } else if (tk == numeric) {
    /** normal numeric */
    redc_s_var_push(redc_s_vconst());
    next();
    return 1;
  } else if (tk == '(') {
    next();
    int e = redc_f_expression(0);
    if (!match(')'))
      exit(1);
    return 1;
  }
}

int redc_f_unary_expression_atom() {
  int tk = peek_nocheck();
  if (tk != '*' && tk != '&') {
    return redc_f_expression_atom();
  }
  if (tk == '*') {
    next();
    return redc_f_expression_atom();
  } else if (tk == '&') {
    next();
    return redc_f_expression_atom();
  }
}

/** main expression parse rule.
        make sure you save the temp variables before calling this and reset them
   after, to avoid register overuse.
*/

int redc_f_expression(char prio) {
  int at = redc_f_unary_expression_atom();

  char *s = *__global_stream;
  int op = redc_f_operation();
  *__global_stream = s;

  while (op) {

    s = *__global_stream;
    op = redc_f_operation();
    char *post_op = *__global_stream;
    *__global_stream = s;

    short bpow = binding_power(((short *)&op)[0]);

    if (((char *)&bpow)[0] < 0) {
      return 1;
    }

    if (prio > ((char *)&bpow)[0]) {
      return 1;
    }

    var res = redc_s_vtemp();
    *__global_stream = post_op;
    redc_f_expression(((char *)&bpow)[1]);
    var b = redc_s_var_pop();
    var a = redc_s_var_pop();

    /** If both are constants, do the thing on the spot.*/

    if (a.type == VCONST && b.type == VCONST) {
      do_op(op, &a, &b);
      redc_s_var_push(a);
    } else {

      /** load any var into registers and do the expression */

      var tb = b, ta = a;
      if (tb.type == VVAR) {
        tb = redc_s_vtemp();
        redc_b_state_add_load(tb, b);
        redc_b_state_advance();
      }
      if (ta.type == VVAR) {
        ta = redc_s_vtemp();
        redc_b_state_add_load(ta, a);
        redc_b_state_advance();
      }
      redc_b_state_add_binop(op, res, ta, tb);
      redc_b_state_advance();
      redc_s_var_push(res);
    }
  }
  return 1;
}
