/**
 * @file parser.c
 * IFJ20 compiler
 * @authors xvosci00
 * @brief the parser
 */

#include "parser.h"
#include "errors.h"
#include "scanner.h"
#include "semgen.h"
#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////

/**
 * Private function prototypes
 */

static bool kw_cmp(tToken *tkn, tKeyWord kw);
static bool token_cmp(tToken *tkn, tTokenType type);
static bool Prog(tFileString *inputFile, tToken *tkn);
static bool Func_list(tFileString *inputFile, tToken *tkn);
static bool Func_next(tFileString *inputFile, tToken *tkn);
static bool Func(tFileString *inputFile, tToken *tkn);
static bool Param_list(tFileString *inputFile, tToken *tkn);
static bool Param_next(tFileString *inputFile, tToken *tkn);
static bool Ret_types(tFileString *inputFile, tToken *tkn);
static bool Type_list(tFileString *inputFile, tToken *tkn);
static bool Type_next(tFileString *inputFile, tToken *tkn);
static bool Type(tFileString *inputFile, tToken *tkn);
static bool St_list(tFileString *inputFile, tToken *tkn);
static bool Stmt(tFileString *inputFile, tToken *tkn);
static bool If(tFileString *inputFile, tToken *tkn);
static bool For(tFileString *inputFile, tToken *tkn);
static bool For_def(tFileString *inputFile, tToken *tkn);
static bool For_assign(tFileString *inputFile, tToken *tkn);
static bool Call_or_var(tFileString *inputFile, tToken *tkn);
static bool Var_next(tFileString *inputFile, tToken *tkn);
static bool Var_def(tFileString *inputFile, tToken *tkn);
static bool Var_assign(tFileString *inputFile, tToken *tkn);
static bool Expr_or_call(tFileString *inputFile, tToken *tkn);
static bool Expr_list(tFileString *inputFile, tToken *tkn);
static bool Expr_next(tFileString *inputFile, tToken *tkn);

////////////////////////////////////////////////////////////////////////////////

/**
 * Private function definitions
 */

static bool kw_cmp(tToken *tkn, tKeyWord kw) {

    if (tkn->type == T_ERR) {
        lex_err();
    }

    return (tkn->type == T_KEYWORD && tkn->attribute.keyword == kw);
}

static bool token_cmp(tToken *tkn, tTokenType type) {

    if (tkn->type == T_ERR) {
        lex_err();
    }

    return (tkn->type == type);
}

bool parse(tFileString *inputFile) {

    tToken tkn = get_token(inputFile, EOL_IGNR);

    return Prog(inputFile, &tkn);
}

static bool Prog(tFileString *inputFile, tToken *tkn) {

    // Prog->package id eol Func_list eof
    if (!kw_cmp(tkn, KW_PACKAGE)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    package_ID_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_EOL)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);

    if (!Func_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_EOF)) {
        return false;
    }

    prog_end_sem(tkn);

    return true;
}

static bool Func_list(tFileString *inputFile, tToken *tkn) {

    // Func_list->eps
    if (token_cmp(tkn, T_EOF)) {
        return true;
    }

    // Func_list->Func Func_next
    if (!kw_cmp(tkn, KW_FUNC)) {
        return false;
    }

    return (Func(inputFile, tkn) && Func_next(inputFile, tkn));
}
static bool Func_next(tFileString *inputFile, tToken *tkn) {

    if (!token_cmp(tkn, T_EOL) && !token_cmp(tkn, T_EOF)) {
        return false;
    }

    // Func_next->eps
    if (token_cmp(tkn, T_EOF)) {
        return true;
    }

    *tkn = get_token(inputFile, EOL_IGNR); // finds the first token after EOLs

    // Func_next->eps
    if (token_cmp(tkn, T_EOF)) {
        return true;
    }

    // Func_next->eol Func Func_next
    if (!Func(inputFile, tkn)) {
        return false;
    }

    return Func_next(inputFile, tkn); // Func_next->eol Func Func_next
}

static bool Func(tFileString *inputFile, tToken *tkn) {

    // Func->func id ( Param_list ) Ret_types { eol St_list }
    if (!kw_cmp(tkn, KW_FUNC)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    func_ID_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_LBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!Param_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_RBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!Ret_types(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_LCBRACKET)) {
        return false;
    }

    func_head_end_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_EOL)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR); // finds the first token after EOLs
    if (!St_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_RCBRACKET)) {
        return false;
    }

    func_end_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return true; // Func->func id ( Param_list ) Ret_types { eol St_list }
}

static bool Param_list(tFileString *inputFile, tToken *tkn) {

    // Param_list->eps
    if (token_cmp(tkn, T_RBRACKET)) {
        return true;
    }

    // Param_list->id Type Param_next
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    param_ID_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!Type(inputFile, tkn)) {
        return false;
    }

    param_type_sem(tkn);

    return Param_next(inputFile, tkn); // Param_list->id Type Param_next
}

static bool Param_next(tFileString *inputFile, tToken *tkn) {

    // Param_next->eps
    if (token_cmp(tkn, T_RBRACKET)) {
        return true;
    }

    // Param_next->, id Type Param_next
    if (!token_cmp(tkn, T_COMMA)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    param_ID_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    if (!Type(inputFile, tkn)) {
        return false;
    }

    param_type_sem(tkn);

    return Param_next(inputFile, tkn);
}

static bool Ret_types(tFileString *inputFile, tToken *tkn) {

    // Ret_types->eps
    if (token_cmp(tkn, T_LCBRACKET)) {
        return true;
    }

    // Ret_types->( Type_list )
    if (!token_cmp(tkn, T_LBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!Type_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_RBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);

    return true; // Ret_types->( Type_list )
}

static bool Type_list(tFileString *inputFile, tToken *tkn) {

    // Type_list-> eps
    if (token_cmp(tkn, T_RBRACKET)) {
        return true;
    }

    // Type_list->Type Type_next
    if (!token_cmp(tkn, T_KEYWORD)) {
        return false;
    }

    switch (tkn->attribute.keyword) {
        case KW_INT:
        case KW_FLOAT64:
        case KW_STRING:
            break;
        default:
            return false;
    }

    if (!Type(inputFile, tkn)) {
        return false;
    }

    return_type_sem(tkn);

    if (!Type_next(inputFile, tkn)) {
        return false;
    }

    return true;
}

static bool Type_next(tFileString *inputFile, tToken *tkn) {

    // Type_next-> eps
    if (token_cmp(tkn, T_RBRACKET)) {
        return true;
    }

    // Type_list->, Type Type_next
    if (!token_cmp(tkn, T_COMMA)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!Type(inputFile, tkn)) {
        return false;
    }

    return_type_sem(tkn);

    return Type_next(inputFile, tkn); // Type_list->, Type Type_next
}

static bool Type(tFileString *inputFile, tToken *tkn) {

    // Type->int | float64 | string
    if (!token_cmp(tkn, T_KEYWORD)) {
        return false;
    }

    switch (tkn->attribute.keyword) {
        case KW_INT:
        case KW_FLOAT64:
        case KW_STRING:
            break;
        default:
            return false;
    }

    type_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return true; // Type->int | float64 | string
}

static bool St_list(tFileString *inputFile, tToken *tkn) {

    // St_list->eps
    if (token_cmp(tkn, T_RCBRACKET)) {
        return true;
    }

    // St_list->Stmt eol St_list
    if (!token_cmp(tkn, T_ID) && !token_cmp(tkn, T_KEYWORD)) {
        return false;
    }

    if (token_cmp(tkn, T_KEYWORD)) {
        switch (tkn->attribute.keyword) {
            case KW_RETURN:
            case KW_IF:
            case KW_FOR:
                break;
            default:
                return false;
        }
    }

    if (!Stmt(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_EOL)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);

    return St_list(inputFile, tkn); // St_list->Stmt eol St_list
}

static bool Stmt(tFileString *inputFile, tToken *tkn) {

    if (!token_cmp(tkn, T_KEYWORD) && !token_cmp(tkn, T_ID)) {
        return false;
    }

    if (token_cmp(tkn, T_KEYWORD)) {
        switch (tkn->attribute.keyword) {
            case KW_RETURN:
            case KW_IF:
            case KW_FOR:
                break;
            default:
                return false;
        }
    }

    // Stmt->id Call_or_var
    if (token_cmp(tkn, T_ID)) {
        ID_unknown_sem(tkn); //checks if fun
        *tkn = get_token(inputFile, EOL_RET);
        return Call_or_var(inputFile, tkn); // Stmt->id Call_or_var
    }

    // Stmt->If
    if (kw_cmp(tkn, KW_IF)) {
        return If(inputFile, tkn);
    }

    // Stmt->For
    if (kw_cmp(tkn, KW_FOR)) {
        return For(inputFile, tkn);
    }

    // Stmt->return Expr_list
    if (kw_cmp(tkn, KW_RETURN)) {
        *tkn = get_token(inputFile, EOL_RET);
        if (Expr_list(inputFile, tkn) == false)
            return false;
        return_sem(tkn);
        return true;
    }

    return false; // should not happen
}

static bool If(tFileString *inputFile, tToken *tkn) {

    // If->if Expr { eol St_list } else { eol St_list }
    if (!kw_cmp(tkn, KW_IF)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    token_cmp(tkn, T_ERR);
    if (!Expr(inputFile, tkn, NULL)) {
        return false;
    }

    if_post_exp_sem(tkn);

    if (!token_cmp(tkn, T_LCBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_EOL)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!St_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_RCBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!kw_cmp(tkn, KW_ELSE)) {
        return false;
    }

    else_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_LCBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_EOL)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!St_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_RCBRACKET)) {
        return false;
    }

    if_end_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return true;
}

static bool For(tFileString *inputFile, tToken *tkn) {

    // For->for For_def ; Expr ; For_assign { eol St_list }
    if (!kw_cmp(tkn, KW_FOR)) {
        return false;
    }

    for_intro_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!For_def(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_SEMIC)) {
        return false;
    }

    for_before_cond_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    token_cmp(tkn, T_ERR);
    if (!Expr(inputFile, tkn, NULL)) {
        return false;
    }

    for_condition_sem(tkn);

    if (!token_cmp(tkn, T_SEMIC)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);
    if (!For_assign(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_LCBRACKET)) {
        return false;
    }

    for_enter_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(tkn, T_EOL)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!St_list(inputFile, tkn)) {
        return false;
    }

    if (!token_cmp(tkn, T_RCBRACKET)) {
        return false;
    }

    for_leave_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return true;
}

static bool For_def(tFileString *inputFile, tToken *tkn) {

    // For_def->eps
    if (token_cmp(tkn, T_SEMIC)) {
        return true;
    }

    // For_def->id Var_def
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    ID_var_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return Var_def(inputFile, tkn); // For_def->id Var_def
}

static bool For_assign(tFileString *inputFile, tToken *tkn) {

    // For_assign->eps
    if (token_cmp(tkn, T_LCBRACKET)) {
        return true;
    }

    // For_assign->id Var_next Var_assign
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    ID_var_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);
    if (!Var_next(inputFile, tkn)) {
        return false;
    }

    if (!Var_assign(inputFile, tkn)) // For_assign->id Var_next Var_assign
        return false;

    assignment_sem(tkn);

    return true;
}

static bool Call_or_var(tFileString *inputFile, tToken *tkn) {

    // Call_or_var->Var_def
    if (token_cmp(tkn, T_DEFIN)) {
        return Var_def(inputFile, tkn);
    }

    // Call_or_var->Var_next Var_assign
    if (token_cmp(tkn, T_ASSIGNM) || token_cmp(tkn, T_COMMA)) {
        if (!Var_next(inputFile, tkn)) {
            return false;
        }
        if (!Var_assign(inputFile, tkn)) {
            return false;
        }
        assignment_sem(tkn);
        return true;
    }

    // Call_or_var->( Expr_list )
    if (!token_cmp(tkn, T_LBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!Expr_list(inputFile, tkn)) {
        return false;
    }

    func_args_sem(tkn);

    if (!token_cmp(tkn, T_RBRACKET)) {
        return false;
    }

    func_noret_end(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return true; // Call_or_var->( Expr_list )
}

static bool Var_next(tFileString *inputFile, tToken *tkn) {

    // Var_next->eps
    if (token_cmp(tkn, T_ASSIGNM)) {
        return true;
    }

    // Var_next->, id Var_next
    if (!token_cmp(tkn, T_COMMA)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!token_cmp(tkn, T_ID)) {
        return false;
    }

    ID_var_sem(tkn);

    *tkn = get_token(inputFile, EOL_RET);

    return Var_next(inputFile, tkn); // Var_next->, id Var_next
}

static bool Var_def(tFileString *inputFile, tToken *tkn) {

    // Var_def->:= Expr
    if (!token_cmp(tkn, T_DEFIN)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    token_cmp(tkn, T_ERR);
    if (!Expr(inputFile, tkn, NULL)) { // Var_def->:= Expr
        return false;
    }

    definition_sem(tkn);

    return true;
}

static bool Var_assign(tFileString *inputFile, tToken *tkn) {

    // Var_assign->:= Expr_or_call
    if (!token_cmp(tkn, T_ASSIGNM)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);

    return Expr_or_call(inputFile, tkn); // Var_assign->:= Expr_or_call
}

static bool Expr_or_call(tFileString *inputFile, tToken *tkn) {

    // Expr_or_call->Expr Expr_next

    if (!token_cmp(tkn, T_ID)) { // also checks for T_ERR
        return (Expr(inputFile, tkn, NULL) && Expr_next(inputFile, tkn));
    }

    tToken next_tkn = get_token(inputFile, EOL_RET);
    if (!token_cmp(&next_tkn, T_LBRACKET)) { // also checks for T_ERR
        return (Expr(inputFile, tkn, &next_tkn) && Expr_next(inputFile, tkn));
    }

    // Expr_or_call->id ( Expr_list )
    func_ret_sem(tkn);

    *tkn = get_token(inputFile, EOL_IGNR);
    if (!Expr_list(inputFile, tkn)) {
        return false;
    }

    func_args_sem(tkn);

    if (!token_cmp(tkn, T_RBRACKET)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_RET);

    return true; // Expr_or_call->id ( Expr_list )
}

static bool Expr_list(tFileString *inputFile, tToken *tkn) {

    // Expr_list->eps
    if (token_cmp(tkn, T_EOL) || token_cmp(tkn, T_RBRACKET)) {
        return true;
    }

    token_cmp(tkn, T_ERR);

    // Expr_list->Expr Expr_next
    return (Expr(inputFile, tkn, NULL) && Expr_next(inputFile, tkn));
}

static bool Expr_next(tFileString *inputFile, tToken *tkn) {

    // Expr_next->eps
    switch (tkn->type) {
        case T_EOL:
        case T_RBRACKET:
        case T_LCBRACKET:
            return true;
        default:
            break;
    }

    // Expr_next->, Expr Expr_next
    if (!token_cmp(tkn, T_COMMA)) {
        return false;
    }

    *tkn = get_token(inputFile, EOL_IGNR);
    token_cmp(tkn, T_ERR);

    return (Expr(inputFile, tkn, NULL) && Expr_next(inputFile, tkn));
}
