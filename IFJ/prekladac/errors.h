/**
 * @file error.h
 * IFJ20 compiler
 * @authors xvenge01
 * @brief header for the error handling
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "expressions.h"
#ifndef ERRORS_H
#define ERRORS_H



typedef enum errorCodes {
    NO_ERR = 0,
    LEX_ERR, // Lexical analysis error
    SYN_ERR, // Syntax analysis error
    SEM_DEF_ERR, // Semantic error due to no or multiple func/var definition
    SEM_T_DEF_ERR, // Semantic error due to wrong variable definition
    SEM_T_EXPR_ERR, // Semantic error due to incompatible types in expressions
    SEM_FUNC_ERR, // Semantic error due to wrong func arguments or return values
    SEM_OTHER_ERR, // Other semantic errors
    SEM_Z_DIV_ERR = 9, // Semantic error due to division by zero
    INTERNAL_ERR = 99
} tErrorCodes; 

void lex_err();

void clean_semgen();

void syntax_err();

void sem_def_err();

void sem_t_def_err();

void sem_t_expr_err();

void sem_func_err();

void sem_other_err();

void sem_z_div_err();

void internal_err();

#endif //ERRORS_H