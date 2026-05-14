#pragma once

#define range(lt, v, gt) (((lt) < (v)) && ((v) < (gt)))
#define range_incl(lt, v, gt) (((lt) <= (v)) && ((v) <= (gt)))

#define kw(k) kw_##k

typedef enum {
  start = 255,
#include "tokenlist"
  ,
  end,
  identifier,
  numeric,
  num_keywords = end - start - 1,
} keywords;

char *tstr();
int len_tstr();

int tokenize(char **stream);
int peektok(char **stream);

#define GS __global_stream

#define next() tokenize(GS)
#define peek(ch) (peektok(GS) == (ch))
#define match(ch) (tokenize(GS) == (ch))
#define peek_nocheck(ch) (peektok(GS))

/** strings */

typedef struct str {
  char *s;
  int len;
} str;

str strmake_static(char *s, int len);
str *strmake(char *s, int len);
void strdel(str *s);
void strdel_static(str s);
str strdp_static(str s);
str *strdp(str *s);

/** variable evaluation stack */

/** REDC STACK NAMESPACE */

typedef enum vartype { VNONE = 0, VCONST, VVAR, VTEMP } vartype;

typedef struct var {
  vartype type;
  str name;
  long long constval;
} var;

var redc_s_vtemp();
var redc_s_vconst();
var redc_s_vvar();
void redc_s_var_push(var v);
var redc_s_var_pop();
int redc_s_var_gettempvarnum();
void redc_s_var_savetempvarnum(int top);

/** REDC BACKEND NAMESPACE */
/** ssa rep */

typedef enum statement_type {

  SNONE = 0,
  SBINOP,
  SLOAD,
  SSTORE,
  SCALL,
  SPARAM,
  SCMP,
  SJUMP,
  SRET,
  SFUNCDEF

} statement_type;

typedef struct statement {
  statement_type stype;
  short op;
  var lhs;
  var rhs1;
  var rhs2;
  str label;
  struct statement *iftrue;
  struct statement *iffalse;
  struct statement *iftrue_prev;
  struct statement *iffalse_prev;
} statement;

typedef struct parse_state {

  statement *all;
  statement *tail;
  statement *current_conditional_block_s;

} parse_state;

statement *new_statement(int type);

void redc_b_state_init();
void redc_b_state_add_binop(short op, var lhs, var a, var b);
void redc_b_state_add_load(var dest, var src);
void redc_b_state_add_store(var dest, var src);
void redc_b_state_add_params(var param);
void redc_b_state_add_call(var funcname);
void redc_b_state_advance();
void redc_b_state_advance_false();


/** REDC FRONTEND NAMESPACE */
/** parse rules */

#define FUNC_DECL_DEF 1024
#define FUNC_CALL 1025

int redc_f_operation();
int redc_f_expression_atom();
int redc_f_unary_expression_atom();
int redc_f_expression(char prio);
int redc_f_statement();
int redc_f_statement_block();
int redc_f_function(int type);
int redc_f_program();
