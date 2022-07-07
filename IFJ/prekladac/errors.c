/**
 * @file errors.c
 * IFJ20 compiler
 * @authors xvenge01
 * @brief error handling
 */


#include "expressions.h"
#include "parser.h"
#include "errors.h"
#include "scanner.h"
#include "semgen.h"
#include <stdio.h>

void clean_semgen(){
    semgen_free();
    scanner_end();
    delete_rest_stack();
    
}
void lex_err(){
    fprintf(stderr, "Error in lexical analysis.\n");
    clean_semgen();
    exit(LEX_ERR);
}

void syntax_err(){
    fprintf(stderr, "Error in syntax.\n");
    clean_semgen();
    exit(SYN_ERR);
}

void sem_def_err(){
    fprintf(stderr, "Error redefinition/not defined function/variable.\n");
    clean_semgen();
    exit( SEM_DEF_ERR);
}

void sem_t_def_err(){
    fprintf(stderr, "Error data type.\n");
    clean_semgen();
    exit(SEM_T_DEF_ERR);
}

void sem_t_expr_err(){
    fprintf(stderr, "Error type incompatible.\n");
    clean_semgen();
    exit(SEM_T_EXPR_ERR);
}

void sem_func_err(){
    fprintf(stderr, "Error function call.\n");
    clean_semgen();
    exit(SEM_FUNC_ERR);
}

void sem_other_err(){
    fprintf(stderr, "Error other semantic error.\n");
    clean_semgen();
    exit(SEM_OTHER_ERR);
}

void sem_z_div_err(){
    fprintf(stderr, "Error zero division.\n");
    clean_semgen();
    exit(SEM_Z_DIV_ERR);
} 

void internal_err(){
    fprintf(stderr, "Error Internal fault.\n");
    clean_semgen();
    exit(INTERNAL_ERR);
} 

