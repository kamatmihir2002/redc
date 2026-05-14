#include "redc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

var varstack[1024];
int top = 0;

int tempv = 0;

var redc_s_vtemp() {
  char tempn[4];
  sprintf(tempn, "v%d", tempv);
  tempv++;
  var v;
  v.name = strmake_static(tempn, 4);
  v.type = VTEMP;
  return v;
}

var redc_s_vconst() {
  long long sval = atoll(tstr());
  var v;
  v.type = VCONST;
  v.constval = sval;
  return v;
}

var redc_s_vvar() {
  var v;
  v.name = strmake_static(tstr(), len_tstr());
  v.type = VVAR;
  return v;
}

var redc_s_vcallresult() {
  var v;
  v.type = VCALLRES;
  return v;
}

void redc_s_var_push(var v) {
  varstack[top] = v;
  top++;
}

var redc_s_var_pop() {
  top--;
  return varstack[top];
}

int redc_s_var_gettempvarnum() { return tempv; }

void redc_s_var_savetempvarnum(int top) { tempv = top; }
/** ssa rep */
