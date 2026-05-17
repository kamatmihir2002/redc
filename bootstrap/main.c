#include "redc.h"
#include <stdio.h>

extern char **__global_stream;
extern parse_state *global_state;

int main(int argc, char **argv) {
  char *p = argv[1];
  __global_stream = &p;
  redc_b_state_init();
  redc_f_expression(0);
  statement *s = global_state->all;
  while (s->iftrue) {
    printf("%d %s %s %c %s ( %lld )\n", s->stype, s->lhs.name.s, s->rhs1.name.s, s->op, s->rhs2.name.s, s->rhs2.constval);
    s = s->iftrue;
  }
  printf("%d %s %s %c %s ( %lld ) \n", s->stype, s->lhs.name.s, s->rhs1.name.s, s->op, s->rhs2.name.s, s->rhs2.constval);
}
