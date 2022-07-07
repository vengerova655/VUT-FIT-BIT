/**
 * @file semgen.c
 * IFJ20 compiler
 * @authors xkucma00
 * @brief header for the semantic analyzer and code generator
 */

#include "scanner.h"
#include <stdbool.h>

#ifndef SEMGEN_H
#define SEMGEN_H


void semgen_free();

void package_ID_sem(tToken *tkn);
void func_ID_sem(tToken *tkn);
void param_ID_sem(tToken *tkn);
void type_sem(tToken *tkn);
void param_type_sem(tToken *tkn);
void return_type_sem(tToken *tkn);
void func_head_end_sem(tToken *tkn);
void func_end_sem(tToken *tkn);
void prog_end_sem(tToken *tkn);

//STMT sem
void ID_unknown_sem(tToken *tkn);

void func_noret_sem(tToken *tkn); //ok
void func_ret_sem(tToken *tkn); //ok
void func_args_sem(tToken *tkn); //ok
void func_noret_end(tToken *tkn); //ok

void ID_var_sem(tToken *tkn);
void definition_sem(tToken *tkn);
void assignment_sem(tToken *tkn);

void return_sem(tToken *tkn);

void if_post_exp_sem(tToken *tkn);
void else_sem(tToken *tkn);
void if_end_sem(tToken *tkn);

void for_intro_sem(tToken *tkn);
void for_before_cond_sem(tToken *tkn);
void for_condition_sem(tToken *tkn);
void for_enter_sem(tToken *tkn);
void for_leave_sem(tToken *tkn);


//EXPR sem
void after_expr_sem(tToken *tkn);
void PA_ID_sem(tToken *tkn);
void PA_INT_sem(tToken *tkn);
void PA_FLOAT_sem(tToken *tkn);
void PA_STRING_sem(tToken *tkn);
void PA_plus_sem();
void PA_minus_sem();
void PA_div_sem();
void PA_mul_sem();
void PA_LT_sem();
void PA_EQ_sem();
void PA_GT_sem();
void PA_LE_sem();
void PA_NE_sem();
void PA_GE_sem();

#endif //SEMGEN_H
