#include "redc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
parse_state *global_state;

statement *new_statement(int type) {
  statement *ss = malloc(sizeof(statement));
  memset(ss, 0, sizeof(statement));
  ss->stype = type;
  return ss;
}

void redc_b_state_init() {
  global_state = malloc(sizeof(parse_state));
  memset(global_state, 0, sizeof(parse_state));
  global_state->all = new_statement(SNONE);
  global_state->tail = global_state->all;
}

void redc_b_state_add_binop(short op, var lhs, var a, var b) {
  statement *sp = new_statement(SBINOP);
  sp->op = op;
  sp->lhs = lhs;
  sp->rhs1 = a;
  sp->rhs2 = b;
  if (!global_state->tail) {
    global_state->tail = sp;
  } else {
    global_state->tail->iftrue = sp;
    global_state->tail->iftrue->iftrue_prev = global_state->tail->iftrue;
  }
}

void redc_b_state_add_params(var param) {
  statement *sp = new_statement(SPARAM);
  sp->lhs = param;
  if (!global_state->tail) {
    global_state->tail = sp;
  } else {
    global_state->tail->iftrue = sp;
    global_state->tail->iftrue->iftrue_prev = global_state->tail->iftrue;
  }
}

void redc_b_state_add_call(var funcname) {
  statement *sp = new_statement(SCALL);
  sp->lhs = funcname;
  if (!global_state->tail) {
    global_state->tail = sp;
  } else {
    global_state->tail->iftrue = sp;
    global_state->tail->iftrue->iftrue_prev = global_state->tail->iftrue;
  }
}

void redc_b_state_add_load(var dest, var src) {
  statement *sp = new_statement(SLOAD);
  sp->lhs = dest;
  sp->rhs1 = src;
  if (!global_state->tail)
    global_state->tail = sp;
  else {
    global_state->tail->iftrue = sp;
    global_state->tail->iftrue->iftrue_prev = global_state->tail->iftrue;
  }
}

void redc_b_state_add_store(var dest, var src) {
  statement *sp = new_statement(SSTORE);
  sp->lhs = dest;
  sp->rhs1 = src;
  if (!global_state->tail)
    global_state->tail = sp;
  else {
    global_state->tail->iftrue = sp;
    global_state->tail->iftrue->iftrue_prev = global_state->tail->iftrue;
  }
}

void redc_b_state_advance() { global_state->tail = global_state->tail->iftrue; }

void redc_b_state_advance_false() {
  global_state->tail = global_state->tail->iffalse;
}

void redc_b_state_add_definition(var funcname) {
  statement *sp = new_statement(SFUNCDEF);
  sp->lhs = funcname;
  if (!global_state->tail) {
    global_state->tail = sp;
  } else {
    global_state->tail->iftrue = sp;
    global_state->tail->iftrue->iftrue_prev = global_state->tail->iftrue;
  }
}
