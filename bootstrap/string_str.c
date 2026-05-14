#include "redc.h"

#include <stdlib.h>
#include <string.h>

str strmake_static(char *s, int len) {
  str _s;
  _s.s = malloc(sizeof(char) * (len + 1));
  memcpy(_s.s, s, len);
  _s.s[len] = 0;
  _s.len = len;
  return _s;
}

str *strmake(char *s, int len) {
  str *_s = malloc(sizeof(str));
  _s->s = malloc(sizeof(char) * (len + 1));
  memcpy(_s->s, s, len);
  _s->s[len] = 0;
  _s->len = len;
  return _s;
}

void strdel(str *s) {
  free(s->s);
  free(s);
}

void strdel_static(str s) { free(s.s); }

str strdp_static(str s) { return strmake_static(s.s, s.len); }

str *strdp(str *s) { return strmake(s->s, s->len); }