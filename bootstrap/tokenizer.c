#include "redc.h"
#include <string.h>

#undef kw
#define kw(k) #k

char **__global_stream;

char *kw_strings[num_keywords] = {
#include "tokenlist"
};

char alpha(char ch) {
  return range_incl('A', ch, 'Z') || range_incl('a', ch, 'z');
}

char num(char ch) { return range_incl('0', ch, '9'); }

char alphanum(char ch) { return alpha(ch) || num(ch); }

char token_string[128];
int token_string_len;

char *tstr() { return token_string; }

int len_tstr() { return token_string_len; }

int tokenize(char **stream) {
  while (**stream == ' ' || **stream == '\t' || **stream == '\n') {
    (*stream)++;
  }

  if (**stream == 0) {
    return 0;
  }
  int l = 0;
  if (alpha(**stream) || **stream == '_') {
    while (alphanum(**stream) || **stream == '_') {
      token_string[l] = **stream;
      (*stream)++;
      l += 1;
    }
    token_string[l] = 0;
    token_string_len = l;
    int kwi = 0;
    while (kwi < num_keywords) {
      if (strcmp(kw_strings[kwi], token_string) == 0) {
        return start + 1 + kwi;
      }
      kwi++;
    }
    return identifier;
  }

  if (num(**stream)) {
    while (num(**stream)) {
      token_string[l] = **stream;
      (*stream)++;
      l += 1;
    }
    token_string[l] = 0;
    token_string_len = l;
    return numeric;
  }

  int tk = **stream;
  (*stream)++;
  return tk;
}

int peektok(char **stream) {
  char *s = *stream;
  int tk = tokenize(stream);
  *stream = s;
  return tk;
}
