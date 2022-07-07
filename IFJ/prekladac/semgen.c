/**
 * @file semgen.c
 * IFJ20 compiler
 * @authors xkucma00 xvisku01 xvenge01
 * @brief the semantic analyzer and code generator
 * @details code is generated and checked sequentially, stack structures and
 *  auxiliary variables used in places where simple sequential approach wouldn't 
 *  work
 */

//Regex to search error calls, ignoring the internal one:
//^(?!.+internal_err\(\).*\n).+err\(\)


#define _GNU_SOURCE
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "parser.h"
#include "scanner.h"
#include "semgen.h"

#include "expressions.h"
#include "symtable.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * Private type definitions, global variables, function parameters
 */

//
//typedefs

//token stack
typedef struct tokenItem {
    tToken tkn;
    struct tokenItem *ptr;
} tTokenItem;

typedef tTokenItem *tStackToken;

//type (actually attrType, not type) stack
typedef struct typeItem {
    tAttrType type;
    struct typeItem *ptr;
} tTypeItem;

typedef tTypeItem *tStackType;

//
//global variables
//!

static tStack *symStk = NULL; //symtable stack
static int IDScp = 0;         //ID of current scope

//auxiliary ptr to current function's entry in the symtable
static tSymItem *auxFn;
//ptr to entry in symtable of function that's currently being called
static tSymItem *calFn;

static tSymItem *auxVar; //auxiliary ptr to a variable's entry in symtable
static tToken curVar;    //token of currently processed ID
//stack to save ID tokens, of variables that are used in definition/assignment
static tStackToken tknStk = NULL;

static tAttrType curType;
//a stack to remember types of processed expressions which haven't been assigned
//yet
static tStackType typeStk = NULL;

//iterator through list of parameters/return types of function
static tAttrList *argIter;

//determines if current function has already included a return statemenet
static bool returned = false;
// true on first pass, at the end changes to false
static bool firstPass = true;
//determines whether last token in an expression was zero literal
static bool zeroLast = false;

//how deep in for cycles is the compiler
//(0 = not currently in for cycle, 1 = for cycle, 2 = nested 2nd for cycle, etc)
static size_t forCnt = 0;
//index that may only increase, used to enable generating unique jumps
//in for cycles, to avoid multi-definition of a variable in generated code
static size_t forDefIdx = 0;
//used to remember value of forDefIdx when needed
static size_t forReturnDefLoc = 0;

//for output string
static char **pointer_to_output = NULL;
static char **pointer_to_instrucution = NULL;
static char *instrucution = NULL;
static char *output = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////
//
//functions

//stks
static void StkTknInit(tStackToken *stk);
static void StkTknDest(tStackToken *stk);
static int StkTknPush(tStackToken *stk, tToken tkn);
static int StkTknPop(tStackToken *stk, tToken *tkn);
static int StkTknTop(tStackToken *stk, tToken *tkn);
static void StkTknDashify(tStackToken stk, char *name);
static void StkTypeInit(tStackType *stk);
static void StkTypeDest(tStackType *stk);
static int StkTypePush(tStackType *stk, tAttrType type);
static int StkTypePop(tStackType *stk, tAttrType *type);
static int StkTypeTop(tStackType *stk, tAttrType *type);

//auxiliary
static int str_output(const char *in, ...);
static void define_var(tSymItem *var);
static bool in_for();
static int cmp_builtin(char *string);
static int add_built_in_fun(tStack *symStk);

////////////////////////////////////////////////////////////////////////////////

/**
 * Private function definitions
 */

//
//Token Stack
static void StkTknInit(tStackToken *stk) {
    *stk = NULL;
}

static void StkTknDest(tStackToken *stk) {
    tTokenItem *tmpItem = *stk;
    while (tmpItem != NULL) {
        *stk = tmpItem->ptr;
        free(tmpItem);
        tmpItem = *stk;
    }
}

static int StkTknPush(tStackToken *stk, tToken tkn) {
    tTokenItem *newItem = malloc(sizeof *newItem);
    if (newItem == NULL)
        internal_err();
    newItem->tkn = tkn;
    newItem->ptr = *stk;
    *stk = newItem;
    return 0;
}

/**
 * @returns returns 0 on success, returns 1 if the stack is empty
 */
static int StkTknPop(tStackToken *stk, tToken *tkn) {
    if (*stk == NULL)
        return 1; //empty stack
    *tkn = (*stk)->tkn;

    tTokenItem *tmp = *stk;
    *stk = (*stk)->ptr;
    free(tmp);

    return 0;
}

/**
 * @returns returns 0 on success, returns 1 if the stack is empty
 */
static int StkTknTop(tStackToken *stk, tToken *tkn) {
    if (*stk == NULL)
        return 1; //empty stack
    *tkn = (*stk)->tkn;
    return 0;
}

/**
 * @brief goes through the whole stack, and renames every ID with given name
 *  to "_"
 */
static void StkTknDashify(tStackToken stk, char *name) {
    tTokenItem *tmp = stk;
    while (tmp != NULL) {
        if (strcmp(tmp->tkn.attribute.string, name) == 0) {
            tmp->tkn.attribute.string = "_";
        }
        tmp = tmp->ptr;
    }
}

//
//Type Stack
static void StkTypeInit(tStackType *stk) {
    *stk = NULL;
}

static void StkTypeDest(tStackType *stk) {
    tTypeItem *tmpItem = *stk;
    while (tmpItem != NULL) {
        *stk = tmpItem->ptr;
        free(tmpItem);
        tmpItem = *stk;
    }
}

static int StkTypePush(tStackType *stk, tAttrType type) {
    tTypeItem *newItem = malloc(sizeof *newItem);
    if (newItem == NULL)
        internal_err();
    newItem->type = type;
    newItem->ptr = *stk;
    *stk = newItem;
    //str_output("\nPushing %d\n", type); //DEBUG
    return 0;
}

/**
 * @returns returns 0 on success, returns 1 if the stack is empty
 */
static int StkTypePop(tStackType *stk, tAttrType *type) {
    if (*stk == NULL)
        return 1; //empty stack
    *type = (*stk)->type;

    tTypeItem *tmp = *stk;
    *stk = (*stk)->ptr;
    free(tmp);

    //str_output("\nPopping %d\n", *type); //DEBUG
    return 0;
}

/**
 * @returns returns 0 on success, returns 1 if the stack is empty
 */
static int StkTypeTop(tStackType *stk, tAttrType *type) {
    if (*stk == NULL)
        return 1; //empty stack
    *type = (*stk)->type;
    return 0;
}

//
//auxiliary

//concatenates to final output string
static int str_output(const char *in, ...) {
    int number_of_chars_printed = 0;
    va_list ar;
    va_start(ar, in);
    char *ptr_to_prev = NULL;

    if (pointer_to_instrucution == NULL) {
        if ((pointer_to_instrucution = (char **)malloc(sizeof(pointer_to_instrucution))) == NULL) {
            internal_err();
        }
    }

    *pointer_to_instrucution = "\0";
    //gets string with variable outputs as needed -. like printf but to string
    if ((number_of_chars_printed = vasprintf(pointer_to_instrucution, in, ar)) < 0) {
        free(pointer_to_instrucution);
        internal_err();
    }

    instrucution = *pointer_to_instrucution;
    va_end(ar);
    free(pointer_to_instrucution);
    pointer_to_instrucution = NULL;

    if (pointer_to_output == NULL) {
        if ((pointer_to_output = (char **)malloc(sizeof(pointer_to_output))) == NULL) {
            internal_err();
        }
    }

    *pointer_to_output = "\0";
    //first initialization->empty string
    if (output == NULL) {
        output = "\0";
    } else {
        ptr_to_prev = output;
    }

    // Concats strings through asprintf()
    if ((number_of_chars_printed = asprintf(pointer_to_output, "%s%s", output, instrucution) < 0)) {
        free(pointer_to_output);
        internal_err();
    }
    output = *pointer_to_output;
    if (ptr_to_prev != NULL) {
        free(ptr_to_prev);
    }
    free(instrucution);
    instrucution = NULL;
    free(pointer_to_output);
    pointer_to_output = NULL;
    return 0;
}

/**
 * @pre tSysItem is var
 * @brief generates code that defines variable
 * @details if in for-cycle, generates outer jump-label pair, to skip this 
 *  definition, and inner label-jump pair that allows jumping to this definition
 *  and then jumping to the next definition
 */
static void define_var(tSymItem *var) {
    //generated code tests if var was already defined, if in for-cycle
    if (in_for()) {
        str_output("JUMP $skipdef%s$%d$%s\n", var->key, IDScp, auxFn->key);
        str_output("LABEL $in-for-defs$$%d\n", forDefIdx);
        forDefIdx++;
    }
    //if not def, define
    str_output("DEFVAR LF@%s$%d\n", var->key, IDScp);

    if (in_for()) {
        str_output("JUMP $in-for-defs$$%d\n", forDefIdx);
        //label for skipping definition
        str_output("LABEL $skipdef%s$%d$%s\n", var->key, IDScp, auxFn->key);
    }
}

static bool in_for() {
    return forCnt > 0;
}

/**
 * @brief compares string with the names of builting functions
 */
static int cmp_builtin(char *string) {
    if (strcmp(string, "inputs") == 0 ||
        strcmp(string, "inputi") == 0 ||
        strcmp(string, "inputf") == 0 ||
        strcmp(string, "print") == 0 ||
        strcmp(string, "int2float") == 0 ||
        strcmp(string, "float2int") == 0 ||
        strcmp(string, "len") == 0 ||
        strcmp(string, "substr") == 0 ||
        strcmp(string, "ord") == 0 ||
        strcmp(string, "chr") == 0)
        return 0;
    return 1;
}

/**
 * @brief generates code for built in functions and adds them to the symtable
 */
static int add_built_in_fun(tStack *symStk) {

    //inputs() (string, int)
    auxFn = InsertNewItem(symStk, "inputs", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListRet(&auxFn, STR, "ret1")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret2")) {
        return 1;
    }
    str_output("# buit-in FUN inputsa\n"
               "LABEL inputs\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval1\n"
               "MOVE LF@%%retval1 nil@nil\n"

               "DEFVAR LF@%%retval2\n"
               "MOVE LF@%%retval2 nil@nil\n"

               "READ LF@%%retval1 string\n"

               "DEFVAR LF@type_result\n"
               "TYPE LF@type_result LF@%%retval1\n"
               "JUMPIFEQ $no_error$inputs LF@type_result string@string\n"
               "MOVE LF@%%retval2 int@1\n"
               "PUSHS string@\n"
               "JUMP $end$inputs\n"

               "LABEL $no_error$inputs\n"
               "MOVE LF@%%retval2 int@0\n"
               "PUSHS LF@%%retval1\n"

               "LABEL $end$inputs\n"
               "PUSHS LF@%%retval2\n"
               "POPFRAME\n"
               "RETURN\n\n");
    //inputi() (int, int)
    auxFn = InsertNewItem(symStk, "inputi", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListRet(&auxFn, INT, "ret2")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret1")) {
        return 1;
    }
    str_output("# buit-in FUN inputi\n"
               "LABEL inputi\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval1\n"
               "MOVE LF@%%retval1 nil@nil\n"

               "DEFVAR LF@%%retval2\n"
               "MOVE LF@%%retval2 nil@nil\n"

               "READ LF@%%retval1 int\n"

               "DEFVAR LF@type_result\n"
               "TYPE LF@type_result LF@%%retval1\n"
               "JUMPIFEQ $no_error$inputi LF@type_result string@int\n"
               "MOVE LF@%%retval2 int@1\n"
               "PUSHS int@0\n"
               "JUMP $end$inputi\n"

               "LABEL $no_error$inputi\n"
               "MOVE LF@%%retval2 int@0\n"
               "PUSHS LF@%%retval1\n"

               "LABEL $end$inputi\n"
               "PUSHS LF@%%retval2\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //inputf() (float64, int)
    auxFn = InsertNewItem(symStk, "inputf", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListRet(&auxFn, FLT, "ret1")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret2")) {
        return 1;
    }
    str_output("# buit-in FUN inputf\n"
               "LABEL inputf\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval1\n"
               "MOVE LF@%%retval1 nil@nil\n"

               "DEFVAR LF@%%retval2\n"
               "MOVE LF@%%retval2 nil@nil\n"

               "READ LF@%%retval1 float\n"

               "DEFVAR LF@type_result\n"
               "TYPE LF@type_result LF@%%retval1\n"
               "JUMPIFEQ $no_error$inputf LF@type_result string@float\n"
               "MOVE LF@%%retval2 int@1\n"
               "PUSHS float@0x0p+0\n"
               "JUMP $end$inputf\n"

               "LABEL $no_error$inputf\n"
               "MOVE LF@%%retval2 int@0\n"
               "PUSHS LF@%%retval1\n"

               "LABEL $end$inputf\n"
               "PUSHS LF@%%retval2\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //print (term1, term2, ...)
    auxFn = InsertNewItem(symStk, "print", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    str_output("# buit-in FUN print\n"
               "LABEL print\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "POPS GF@num_of_print1\n"
               "MOVE GF@num_of_print2 GF@num_of_print1\n"

               "LABEL $before_while1$print\n"
               "JUMPIFEQ $before_while2$print GF@num_of_print1 int@0\n"

               "CREATEFRAME\n"
               "PUSHFRAME\n"
               "DEFVAR LF@print_hodnota\n"
               "POPS LF@print_hodnota\n"

               "SUB GF@num_of_print1 GF@num_of_print1 int@1\n"
               "JUMP $before_while1$print\n"

               "LABEL $before_while2$print\n"
               "JUMPIFEQ $end$print GF@num_of_print2 int@0\n"

               "WRITE LF@print_hodnota\n"
               "POPFRAME\n"
               "SUB GF@num_of_print2 GF@num_of_print2 int@1\n"
               "JUMP $before_while2$print\n"

               "LABEL $end$print\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //int2float (i int) (float64)
    auxFn = InsertNewItem(symStk, "int2float", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListArg(&auxFn, INT, "i")) {
        return 1;
    }
    if (AddListRet(&auxFn, FLT, "ret")) {
        return 1;
    }
    str_output("# buit-in FUN int2float\n"
               "LABEL int2float\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval\n"
               "MOVE LF@%%retval nil@nil\n"

               "DEFVAR LF@i$1\n"
               "POPS LF@i$1\n"

               "INT2FLOAT LF@%%retval LF@i$1\n"
               "PUSHS LF@%%retval\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //float2int (f float64) (int)
    auxFn = InsertNewItem(symStk, "float2int", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListArg(&auxFn, FLT, "f")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret")) {
        return 1;
    }
    str_output("# buit-in FUN float2int\n"
               "LABEL float2int\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval\n"
               "MOVE LF@%%retval nil@nil\n"

               "DEFVAR LF@f$1\n"
               "POPS LF@f$1\n"

               "FLOAT2INT LF@%%retval LF@f$1\n"
               "PUSHS LF@%%retval\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //len (s string) (int)
    auxFn = InsertNewItem(symStk, "len", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListArg(&auxFn, STR, "s")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret")) {
        return 1;
    }
    str_output("# buit-in FUN len\n"
               "LABEL len\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval\n"
               "MOVE LF@%%retval nil@nil\n"

               "DEFVAR LF@s$1\n"
               "POPS LF@s$1\n"

               "STRLEN LF@%%retval LF@s$1\n"
               "PUSHS LF@%%retval\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //substr (S string, i int, n int) (string, int)
    auxFn = InsertNewItem(symStk, "substr", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListArg(&auxFn, STR, "S")) {
        return 1;
    }
    if (AddListArg(&auxFn, INT, "i")) {
        return 1;
    }
    if (AddListArg(&auxFn, INT, "n")) {
        return 1;
    }
    if (AddListRet(&auxFn, STR, "ret1")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret2")) {
        return 1;
    }
    str_output("# buit-in FUN substr\n"
               "LABEL substr\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval1\n"
               "MOVE LF@%%retval1 nil@nil\n"

               "DEFVAR LF@%%retval2\n"
               "MOVE LF@%%retval2 nil@nil\n"

               "DEFVAR LF@n$1\n"
               "POPS LF@n$1\n"
               "DEFVAR LF@i$1\n"
               "POPS LF@i$1\n"
               "DEFVAR LF@S$1\n"
               "POPS LF@S$1\n"

               "DEFVAR LF@error_bool\n"     //START error checking
               "DEFVAR LF@lenght_minus_i\n" //version2
               "DEFVAR LF@str_len\n"
               "STRLEN LF@str_len LF@S$1\n"
               "SUB LF@lenght_minus_i LF@str_len LF@i$1\n" //version2

               "LT LF@error_bool LF@i$1 int@0\n"
               "JUMPIFEQ $start_error$substr LF@error_bool bool@true\n"
               "GT LF@error_bool LF@i$1 LF@str_len\n"
               "JUMPIFEQ $start_error$substr LF@error_bool bool@true\n"
               "LT LF@error_bool LF@n$1 int@0\n"
               "JUMPIFEQ $start_error$substr LF@error_bool bool@true\n"

               //VERSION2
               "GT LF@error_bool LF@n$1 LF@lenght_minus_i\n"
               "JUMPIFNEQ $after_error_handling$substr LF@error_bool bool@true\n"
               "MOVE LF@n$1 LF@lenght_minus_i\n"

               "LABEL $after_error_handling$substr\n" //END error Checking
               "MOVE GF@str1 string@\n"
               "MOVE GF@str2 string@\n"

               "DEFVAR LF@while_bool_eq\n"
               "DEFVAR LF@while_bool\n"

               "LABEL $while_loop$substr\n"

               "EQ LF@while_bool_eq LF@n$1 int@0\n"
               "JUMPIFEQ $end_while_loop$substr LF@while_bool_eq bool@true\n" //while (n != 0)

               "GETCHAR GF@str1 LF@S$1 LF@i$1\n"
               "CONCAT GF@str2 GF@str2 GF@str1\n"
               "ADD LF@i$1 LF@i$1 int@1\n"
               "SUB LF@n$1 LF@n$1 int@1\n"
               "JUMP $while_loop$substr\n"

               "LABEL $end_while_loop$substr\n"

               //END VERSION2

               "MOVE LF@%%retval1 GF@str2\n" //return
               "MOVE LF@%%retval2 int@0\n"
               "JUMP $end_error$substr\n"

               "LABEL $start_error$substr\n"
               "MOVE LF@%%retval2 int@1\n"

               "LABEL $end_error$substr\n"
               "PUSHS LF@%%retval1\n"
               "PUSHS LF@%%retval2\n"

               "POPFRAME\n"
               "RETURN\n\n");

    //ord (S string, i int) (int, int)
    auxFn = InsertNewItem(symStk, "ord", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListArg(&auxFn, STR, "S")) {
        return 1;
    }
    if (AddListArg(&auxFn, INT, "i")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret1")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret2")) {
        return 1;
    }
    str_output("# buit-in FUN ord\n"
               "LABEL ord\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval1\n"
               "MOVE LF@%%retval1 nil@nil\n"

               "DEFVAR LF@%%retval2\n"
               "MOVE LF@%%retval2 nil@nil\n"

               "DEFVAR LF@i$1\n"
               "POPS LF@i$1\n"

               "DEFVAR LF@S$1\n"
               "POPS LF@S$1\n"

               "DEFVAR LF@error_bool\n"
               "DEFVAR LF@str_len\n"
               "STRLEN LF@str_len LF@S$1\n"
               "SUB LF@str_len LF@str_len int@1\n"

               "LT LF@error_bool LF@i$1 int@0\n"
               "JUMPIFEQ $start_error$ord LF@error_bool bool@true\n"
               "GT LF@error_bool LF@i$1 LF@str_len\n"
               "JUMPIFEQ $start_error$ord LF@error_bool bool@true\n"

               "STRI2INT LF@%%retval1 LF@S$1 LF@i$1\n"
               "MOVE LF@%%retval2 int@0\n"
               "JUMP $end_error$ord\n"

               "LABEL $start_error$ord\n"
               "MOVE LF@%%retval2 int@1\n"

               "LABEL $end_error$ord\n"
               "PUSHS LF@%%retval1\n"
               "PUSHS LF@%%retval2\n"
               "POPFRAME\n"
               "RETURN\n\n");

    //chr (i int) (string, int)
    auxFn = InsertNewItem(symStk, "chr", &IDScp);
    if (!auxFn) {
        return 1;
    }
    auxFn->type = FUN;
    if (AddListArg(&auxFn, INT, "i")) {
        return 1;
    }
    if (AddListRet(&auxFn, STR, "ret1")) {
        return 1;
    }
    if (AddListRet(&auxFn, INT, "ret2")) {
        return 1;
    }
    str_output("# buit-in FUN chr\n"
               "LABEL chr\n"
               "CREATEFRAME\n"
               "PUSHFRAME\n"

               "DEFVAR LF@%%retval1\n"
               "MOVE LF@%%retval1 nil@nil\n"

               "DEFVAR LF@%%retval2\n"
               "MOVE LF@%%retval2 nil@nil\n"

               "DEFVAR LF@i$1\n"
               "POPS LF@i$1\n"

               "DEFVAR LF@error_bool\n"

               "LT LF@error_bool LF@i$1 int@0\n"
               "JUMPIFEQ $start_error$chr LF@error_bool bool@true\n"
               "GT LF@error_bool LF@i$1 int@255\n"
               "JUMPIFEQ $start_error$chr LF@error_bool bool@true\n"

               "INT2CHAR LF@%%retval1 LF@i$1\n"
               "MOVE LF@%%retval2 int@0\n"
               "JUMP $end_error$chr\n"

               "LABEL $start_error$chr\n"
               "MOVE LF@%%retval2 int@1\n"

               "LABEL $end_error$chr\n"
               "PUSHS LF@%%retval1\n"
               "PUSHS LF@%%retval2\n"
               "POPFRAME\n"
               "RETURN\n\n");
    return 0;
}
////////////////////////////////////////////////////////////////////////////////

/**
 * Public function definitions
 */

/**
 * @brief frees all resources used by semgen
 */
void semgen_free() {
    DisposeStack(&symStk);
    StkTknDest(&tknStk);
    StkTypeDest(&typeStk);
    free(output);
    symStk = NULL;
    tknStk = NULL;
    typeStk = NULL;
}

/**
 * First pass: initializes dynamic structures, generates some necessary parts
 * of the code that are used in every source file
 * Second pass: checks if the package is main, and if main was properly defined
 */
void package_ID_sem(tToken *tkn) {
    if (!firstPass) {
        if (strcmp(tkn->attribute.string, "main") != 0) {
            //Error: package is not main
            syntax_err();
        }

        auxFn = SearchTop(symStk, "main", &IDScp);
        //missing main
        if (auxFn == NULL)
            //Error: main function Wasn't defined
            sem_def_err();
        //wrong params/arguments
        if (auxFn->argList != NULL || auxFn->returnList != NULL)
            //Error: main function doesn't have emtpy parameter/return list
            sem_func_err();

        curType = NONE;
    } else {
        //FIRST PASS
        //initialize scope stack
        if (InitStack(&symStk) != 0)
            internal_err();

        //initialize stacks
        StkTknInit(&tknStk);
        StkTypeInit(&typeStk);

        //G: define global variables
        str_output(".IFJcode20\nDEFVAR GF@str1\nDEFVAR GF@num_of_print1\nDEFVAR GF@num_of_print2\n"
                   "DEFVAR GF@str2\nDEFVAR GF@str3\nDEFVAR GF@$$$IDtrash\n");
        str_output("CREATEFRAME\nPUSHFRAME\nCALL main\nJUMP $$$$end\n\n\n");
        if (add_built_in_fun(symStk) != 0)
            internal_err();
    }
}

/**
 * First pass: saves function to symtable
 * Second pass: creates new scope, generates beginning of the function related
 *  code
 */
void func_ID_sem(tToken *tkn) {
    if (!firstPass) {
        //no need to test if found, must've been added in the first pass
        auxFn = SearchAll(symStk, tkn->attribute.string, &IDScp);
        if (InsertNewScope(&symStk) == 1)
            internal_err();
        str_output("\nLABEL %s\n", auxFn->key);
        str_output("CREATEFRAME\n"
                   "PUSHFRAME\n");
        str_output("#defining and assigning values to args\n"); //G comment
    } else {
        //FIRST PASS
        if (strcmp(tkn->attribute.string, "_") == 0)
            //Error: defining _ as a function
            sem_def_err(); //idk? maybe 7
        auxFn = SearchAll(symStk, tkn->attribute.string, &IDScp);
        //if duplicate function
        if (auxFn != NULL)
            //Error: multiple function definitions for the same ID
            sem_def_err();
        auxFn = InsertNewItem(symStk, tkn->attribute.string, &IDScp);
        if (auxFn == NULL)
            internal_err();
        auxFn->type = FUN;
    }
}

/**
 * First pass: adds parameter to function's symtable entry's parameter list
 * Second pass: adds paramater to scope, saves variable pointer to the parameter
 */
void param_ID_sem(tToken *tkn) {
    if (!firstPass) {
        //check if exists, create var in symtable
        if (SearchTop(symStk, tkn->attribute.string, &IDScp) != NULL)
            //Error: multiple parameters with the same name
            sem_def_err();
        //insert it into the scope
        auxVar = InsertNewItem(symStk, tkn->attribute.string, &IDScp);
        auxVar->type = VAR;
    } else {
        //FIRST PASS
        if (strcmp(tkn->attribute.string, "_") == 0)
            //Error: defining _ as a parameter
            sem_def_err(); //idk? maybe 7
        if (cmp_builtin(tkn->attribute.string) == 0)
            //Error: defining param with the same name as a built in function
            sem_def_err();
        if (AddListArg(&auxFn, NONE, tkn->attribute.string) != 0)
            internal_err();
        //take the added arg item
        argIter = auxFn->argList;
    }
}

/**
 * First pass: sets current type
 * Second pass: sets current type
 */
void type_sem(tToken *tkn) {
    if (!firstPass) {
        switch (tkn->attribute.keyword) {
            case KW_FLOAT64:
                curType = FLT;
                break;
            case KW_INT:
                curType = INT;
                break;
            case KW_STRING:
                curType = STR;
                break;
            default:
                //should never happen, syntax checks this
                ;
        }
    } else {
        switch (tkn->attribute.keyword) {
            case KW_FLOAT64:
                curType = FLT;
                break;
            case KW_INT:
                curType = INT;
                break;
            case KW_STRING:
                curType = STR;
                break;
            default:
                //should never happen, syntax checks this
                ;
        }
    }
}

/**
 * First pass: sets variable type, defines parameter, resets current type
 * Second pass: sets parameter type, resets current type
 */
void param_type_sem(tToken *tkn) {
    if (!firstPass) {
        auxVar->attrType = curType;
        define_var(auxVar);
        curType = NONE;
    } else {
        argIter->type = curType;
        curType = NONE;
    }
}

/**
 * First pass: resets current type
 * Second pass: adds return type to function's symtable entry's return list,
 * resets current type
 */
void return_type_sem(tToken *tkn) {
    if (!firstPass) {
        curType = NONE;
    } else {
        if (AddListRet(&auxFn, curType, NULL) != 0)
            internal_err();
        curType = NONE;
    }
}

/**
 * Second pass: pop stack values into parameter variables
 */
void func_head_end_sem(tToken *tkn) {
    if (!firstPass) {
        argIter = auxFn->argList;
        while (argIter != NULL) {
            str_output("POPS LF@%s$%d\n", argIter->name, IDScp);
            argIter = argIter->ptrnext;
        }
        str_output("\n");
    } else {
        ;
    }
}

/**
 * Second pass: removes scope, does end of function related checks and 
 *  generates popframe, return, in case function didn't contain return
 */
void func_end_sem(tToken *tkn) {
    if (!firstPass) {
        DeleteTopScope(&symStk);
        if (auxFn->returnList == NULL) {
            str_output("POPFRAME\n");
            str_output("RETURN\n\n");
        } else {
            str_output("EXIT int@6\n\n"); //runtime sem_func_err
        }
        if (!returned && auxFn->returnList != NULL)
            //Error: missing return in a function with non-empty return list
            sem_func_err();
        auxFn = NULL;
        returned = false;
    } else {
        ;
    }
}

/**
 * First pass: change to second pass
 * Second pass: generates code for the end of the program, prints the generated
 *  code, frees resources
 */
void prog_end_sem(tToken *tkn) {
    if (!firstPass) {
        str_output("LABEL $$$$end\nPOPFRAME\n");
        printf("%s", output);
        semgen_free();
    } else {
        firstPass = false;
    }
}
////////////////////////////////////////////////////////////////////////////////
//Statement related semantic functions
//no first pass actions here

//ID at the beginning of statement (unsure whether func call or var def/assign)
void ID_unknown_sem(tToken *tkn) {
    if (!firstPass) {
        calFn = SearchAll(symStk, tkn->attribute.string, &IDScp);
        if (calFn != NULL && calFn->type == FUN) {
            //still may be ID that's currently being defined, therefore
            //it needs to be saved
            ID_var_sem(tkn);
        } else {
            calFn = NULL;
            ID_var_sem(tkn);
            //whether it exists in the scope gets checked happens after assign
        }
    } else {
        ;
    }
}

//checks if function called in no-return context has return types
void func_noret_sem(tToken *tkn) {
    if (!firstPass) {
        if (calFn->returnList != NULL)
            sem_func_err();
        // exit(6);
    } else {
        ;
    }
}

//performs checks related to calling function in assignment context
void func_ret_sem(tToken *tkn) {
    if (!firstPass) {
        calFn = SearchAll(symStk, tkn->attribute.string, &IDScp);
        if (calFn == NULL || calFn->type != FUN)
            //Error: calling undefined function
            sem_def_err();
        if (strcmp(calFn->key, "print") == 0)
            //Error: assigning result of a function with no return values
            //(more specifically, print)
            sem_other_err();
    } else {
        ;
    }
}

//checks validity of expressions given as arguments, generates func call
//note: print is a special case
void func_args_sem(tToken *tkn) {
    if (!firstPass) {
        if (calFn == NULL)
            //Error: calling undefined function
            sem_def_err();
        if (strcmp(calFn->key, "print") == 0) {
            int i = 0; //number of arguments
            while (StkTypePop(&typeStk, &curType) != 1) {
                if (curType == BOL)
                    //Error: printing bool (that's error, right?)
                    sem_func_err();
                i++;
            }
            str_output("PUSHS int@%d\n", i); //pushes amount of args
            str_output("CALL print\n");
            //accepts print with 0 arguments
            curType = NONE;
        } else {
            argIter = calFn->argList;
            while (argIter != NULL) {
                //check if there are still some arguments
                if (StkTypePop(&typeStk, &curType) == 1)
                    //Error: fewer arguments than function parameters
                    sem_func_err();
                //check if arg types match
                if (curType != argIter->type)
                    //Error: argument type is different than parameter type
                    sem_func_err();
                //the args are already in stack, no need to do anything here
                argIter = argIter->ptrnext;
            }
            //if there are too many args
            if (StkTypeTop(&typeStk, &curType) != 1)
                //Error: more arguments than function parameters
                sem_func_err();
            str_output("CALL %s\n", calFn->key);
            curType = NONE;
        }
    } else {
        ;
    }
}

//gets rid of the saved ID
void func_noret_end(tToken *tkn) {
    if (!firstPass) {
        //turned out to not be ID that's being defined after all, but
        //genuine function
        StkTknPop(&tknStk, &curVar);
        //no longer calling function
        calFn = NULL;
    } else {
        ;
    }
}

//saves the ID to the stack
void ID_var_sem(tToken *tkn) {
    if (!firstPass) {
        curVar = *tkn;
        StkTknPush(&tknStk, curVar);
    } else {
        ;
    }
}

//does definition related checks and generates definition, assigns value
void definition_sem(tToken *tkn) {
    if (!firstPass) {
        StkTknPop(&tknStk, &curVar); //no need to test, syntax guarantees

        if (strcmp(curVar.attribute.string, "_") == 0)
            //Error: defining _ as a variable
            sem_def_err(); //idk? maybe 7
        if (cmp_builtin(curVar.attribute.string) == 0)
            //Error: defining variable with same name as a built in function
            sem_def_err(); //idk?; maybe (re)definition error?
        auxVar = SearchTop(symStk, curVar.attribute.string, &IDScp);
        //if exists
        if (auxVar != NULL)
            //Error: redefining variable (in the same scope)
            sem_def_err();
        auxVar = InsertNewItem(symStk, curVar.attribute.string, &IDScp);
        auxVar->type = VAR;
        if (auxVar == NULL)
            internal_err();

        str_output("#definition\n");
        define_var(auxVar);
        if (StkTypePop(&typeStk, &curType) == 1 || curType == BOL)
            //Error: the result of an expression is missing (probs impossible)
            //or bool
            sem_t_def_err(); //idk?
        auxVar->attrType = curType;
        curType = NONE;
        str_output("POPS LF@%s$%d\n\n", auxVar->key, IDScp);
    } else {
        ;
    }
}

//does assignment related checks and generates the code that assigns values
//different handling for func call assignment and expression assignment
void assignment_sem(tToken *tkn) {
    if (!firstPass) {
        if (calFn != NULL) {
            //if just called a function, get iterator
            argIter = calFn->returnList;
        }
        //while having id's to assign values to
        while (StkTknPop(&tknStk, &curVar) != 1) {
            auxVar = SearchAll(symStk, curVar.attribute.string, &IDScp);
            //and the IDs were defined
            if ((strcmp(curVar.attribute.string, "_") != 0) && auxVar == NULL)
                //Error: assigning to undefined ID, that's also not _
                sem_def_err();
            //and it's not function
            if ((strcmp(curVar.attribute.string, "_") != 0) && auxVar->type == FUN)
                //Error: assigning to function
                sem_other_err(); // idk?; maybe sem_def_err?
            if (calFn == NULL) {
                //and there are still results on the stack
                if (StkTypePop(&typeStk, &curType) == 1)
                    //Error: fewer expression than IDs to assign to
                    sem_other_err(); //idk?
            } else {
                //and the function has still unused return values
                if (argIter == NULL)
                    //Error: fewer return values than IDs to assign to
                    sem_func_err();
                curType = argIter->type;
                argIter = argIter->ptrnext;
            }
            //and types match (if _, type just can't be BOOL)
            if (((strcmp(curVar.attribute.string, "_") != 0) &&
                 curType != auxVar->attrType) ||
                curType == BOL) {
                //Error: assignemnt types don't match
                if (calFn == NULL)
                    sem_t_expr_err(); //according to forum post (id=57605), 4)
                else
                    sem_other_err();
            }
            //assign the value from the stack
            str_output("#assignment\n");
            if (strcmp(curVar.attribute.string, "_") != 0) {
                //prevents from assigning to same variable, in this assignment
                StkTknDashify(tknStk, auxVar->key);
                str_output("POPS LF@%s$%d\n", auxVar->key, IDScp);
            } else {
                str_output("POPS GF@$$$IDtrash\n");
            }
        }
        //if there are still results on the stack, even if there's no variable
        if ((calFn == NULL && StkTypeTop(&typeStk, &curType) != 1) ||
            (calFn != NULL && argIter != NULL))
            //Error: fewer expression/return values than IDs to assign to
            sem_other_err(); // idk?

        curType = NONE;
    } else {
        ;
    }
}

//checks return list with expresion results, remembers that current function
//called return, generates return code
void return_sem(tToken *tkn) {
    if (!firstPass) {
        argIter = auxFn->returnList;
        while (argIter != NULL) {
            //if not enough values for return
            if (StkTypePop(&typeStk, &curType) == 1)
                //Error: fewer expressions than function's return types
                sem_func_err();
            //if wrong types
            if (curType != argIter->type)
                //Error: expression result doesn't match return type
                sem_func_err();
            argIter = argIter->ptrnext;
        }
        //if too many values for return
        if (StkTypeTop(&typeStk, &curType) != 1)
            //Error: more expression than function's return types
            sem_func_err();
        returned = true;
        curType = NONE;
        str_output("POPFRAME\n");
        str_output("RETURN\n");
    } else {
        ;
    }
}

//creates if-else statement scope (for the scope ID that's used as tag),
//then if scope, checks the condition expression type, generates conditional
//jump code
void if_post_exp_sem(tToken *tkn) {
    if (!firstPass) {
        StkTypePop(&typeStk, &curType); //no need to test, syntax guarantees
        if (curType != BOL)
            //Error: if-condition is not bool
            sem_t_expr_err();             //idk?
        if (InsertNewScope(&symStk) == 1) //scope purely to remember the ID
            internal_err();
        str_output("PUSHS bool@false\n");
        str_output("JUMPIFEQS $else$%d$%s\n", GetTopID(symStk), auxFn->key);
        if (InsertNewScope(&symStk) == 1) //actual scope
            internal_err();
        curType = NONE;
    } else {
        ;
    }
}

//generates hard jump to skip the else part, generates label for else cond jump,
//removes if scope and creates the else scope
void else_sem(tToken *tkn) {
    if (!firstPass) {
        DeleteTopScope(&symStk);
        str_output("JUMP $else-skip$%d$%s\n", GetTopID(symStk), auxFn->key);
        str_output("LABEL $else$%d$%s\n", GetTopID(symStk), auxFn->key);
        if (InsertNewScope(&symStk) == 1) //actual scope
            internal_err();
    } else {
        ;
    }
}

//generates label for the else skip jump, removes scopes
void if_end_sem(tToken *tkn) {
    if (!firstPass) {
        DeleteTopScope(&symStk); //delete actual scope
        str_output("LABEL $else-skip$%d$%s\n", GetTopID(symStk), auxFn->key);
        DeleteTopScope(&symStk); //delete ID scope
    } else {
        ;
    }
}

//creates a new scope, if not already in a for - also generate code for handling
//definitions within the cycle
void for_intro_sem(tToken *tkn) {
    if (!firstPass) {
        //if not yet in for, jump to next definition within this
        //for or this for's sub-fors, also create label to return here later
        //and remember this label's index
        if (!in_for()) {
            forReturnDefLoc = forDefIdx;
            forDefIdx++;
            str_output("JUMP $in-for-defs$$%d\n", forDefIdx);
            str_output("LABEL $in-for-defs$$%d\n", forReturnDefLoc);
        }
        //scope for the definition, assignment
        if (InsertNewScope(&symStk) == 1)
            internal_err(); //exit(99)
    } else {
        ;
    }
}

//for-def happens here

//generate label for the for-cycle
void for_before_cond_sem(tToken *tkn) {
    if (!firstPass) {
        str_output("#for incoming (def^)\n"
                   "LABEL $for-start$%d$%s\n",
                   GetTopID(symStk), auxFn->key);
    } else {
        ;
    }
}

//for-cond expression happens here

//check the expression type, generate a conditional jump code, generate jump
//to skip the assignment, and label to the assignment
void for_condition_sem(tToken *tkn) {
    if (!firstPass) {
        StkTypePop(&typeStk, &curType); //no need to test, syntax guarantees
        if (curType != BOL)
            //Error: for-condition is not bool
            sem_t_expr_err();
        str_output("PUSHS bool@false\n");
        str_output("JUMPIFEQS $for-skip$%d$%s\n", GetTopID(symStk), auxFn->key);
        curType = NONE;
        str_output("JUMP $for-assign-skip$%d$%s\n", GetTopID(symStk), auxFn->key);
        str_output("LABEL $for-assign$%d$%s\n", GetTopID(symStk), auxFn->key);
    } else {
        ;
    }
}

//for-assignment happens here

//generates jump after the assignment to the beginning of the cycle, then label
//for skipping the assignment, creates second (internal) scope
void for_enter_sem(tToken *tkn) {
    if (!firstPass) {
        str_output("JUMP $for-start$%d$%s\n", GetTopID(symStk), auxFn->key);
        str_output("LABEL $for-assign-skip$%d$%s\n",
                   GetTopID(symStk), auxFn->key);
        forCnt++;
        if (InsertNewScope(&symStk) == 1)
            internal_err();
    } else {
        ;
    }
}

//for-cycle statements are here

//generate hard jump to for-conditional-expression, and label for skipping for
//cycle;
//if leaving for-cycle that isn't nested - generate additional code and
//  a jump-label pair to skip it; the code - generate a label to jump to from
//  the last definition in this for-cycle, then immediatelly jump back to the
//  beginning of the for-cycle (right after the point where the definition jumps
//  began) using the saved for index;
void for_leave_sem(tToken *tkn) {
    if (!firstPass) {
        DeleteTopScope(&symStk);
        forCnt--;
        //if this is the top for, here's stop for the jumping definitions that
        //return the program to the beginning of the for
        if (!in_for()) {
            str_output("JUMP $for-just-skipping-this-part$%d$%s\n",
                       GetTopID(symStk), auxFn->key);
            str_output("LABEL $in-for-defs$$%d\n", forDefIdx);
            str_output("JUMP $in-for-defs$$%d\n", forReturnDefLoc);
            forDefIdx++;
            str_output("LABEL $for-just-skipping-this-part$%d$%s\n",
                       GetTopID(symStk), auxFn->key);
        }
        str_output("JUMP $for-assign$%d$%s\n", GetTopID(symStk), auxFn->key);
        str_output("LABEL $for-skip$%d$%s\n", GetTopID(symStk), auxFn->key);
        DeleteTopScope(&symStk);
    } else {
        ;
    }
}

////////////////////////////////////////////////////////////////////////////////
//Expression related semantic functions

//General information
//At the beginning, current type is not defined. The first literal/ID sets the
//type, the following literals'/IDs' types are checked with that one
//operations check validity of types (relation operations change type to bool);
//preforms other various checks;
//literals/IDs generate code that pushes them to the IFJ20code stack;
//operations generate stack instructions that executes said operation;
//(concat requires special handling, due to lack of stack version of concat);
//at the end of the expression, the type is saved to the stack and reset

void after_expr_sem(tToken *tkn) {
    if (!firstPass) {
        StkTypePush(&typeStk, curType);
        curType = NONE;
        str_output("#EXPR ^\n");
        zeroLast = false;
    } else {
        ;
    }
}

void PA_ID_sem(tToken *tkn) {
    if (!firstPass) {
        zeroLast = false;
        if (strcmp(tkn->attribute.string, "_") == 0)
            //Error: using _ in expression
            sem_def_err(); //idk? maybe 7
        auxVar = SearchAll(symStk, tkn->attribute.string, &IDScp);
        if (auxVar == NULL) {
            //Error: undefined ID used in an expression
            sem_def_err();
        }
        if (auxVar->type != VAR)
            //Error: function ID used in an expression
            sem_other_err();   //idk? maybe 5
        if (curType == NONE) { //get type, if have none
            curType = auxVar->attrType;
        } //check type compatibility
        else if (curType != auxVar->attrType) {
            //Error: uncompatible types in an expression
            sem_t_expr_err();
        }
        str_output("PUSHS LF@%s$%d\n", auxVar->key, IDScp);
    } else {
        ;
    }
}

void PA_INT_sem(tToken *tkn) {
    if (!firstPass) {
        if (tkn->attribute.integer == 0) {
            zeroLast = true;
        } else {
            zeroLast = false;
        }
        if (curType == NONE) {
            curType = INT;
        } else if (curType != INT) {
            //Error: uncompatible types in an expression
            sem_t_expr_err();
        }
        str_output("PUSHS int@%ld\n", tkn->attribute.integer);
    } else {
        ;
    }
}

void PA_FLOAT_sem(tToken *tkn) {
    if (!firstPass) {
        if (tkn->attribute.real == 0) {
            zeroLast = true;
        } else {
            zeroLast = false;
        }
        if (curType == NONE) {
            curType = FLT;
        } else if (curType != FLT) {
            //Error: uncompatible types in an expression
            sem_t_expr_err();
        }
        str_output("PUSHS float@%a\n", tkn->attribute.real);
    } else {
        ;
    }
}

void PA_STRING_sem(tToken *tkn) {
    if (!firstPass) {
        zeroLast = false;
        if (curType == NONE) {
            curType = STR;
        } else if (curType != STR) {
            //Error: uncompatible types in an expression
            sem_t_expr_err();
        }
        str_output("PUSHS string@%s\n", tkn->attribute.string);
    } else {
        ;
    }
}

void PA_plus_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case INT:
            case FLT:
                str_output("ADDS\n");
                break;
            case STR:
                //take them out, concat, push back
                str_output("POPS GF@str2\n");
                str_output("POPS GF@str1\n");
                str_output("CONCAT GF@str3 GF@str1 GF@str2\n");
                str_output("PUSHS GF@str3\n");
                break;
            default:
                break; //shouldn't happen
        }
    } else {
        ;
    }
}

void PA_minus_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case STR:
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case INT:
            case FLT:
                str_output("SUBS\n");
                break;
            default:
                break; //shouldn't happen
        }
    } else {
        ;
    }
}

void PA_div_sem() {
    if (!firstPass) {
        if (zeroLast == true)
            //Error: division by zero literal (0 or 0.0)
            sem_z_div_err();
        zeroLast = false;
        switch (curType) {
            case STR:
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case INT:
                str_output("IDIVS\n");
                break;
            case FLT:
                str_output("DIVS\n");
                break;
            default:
                break; //shouldn't happen
        }
    } else {
        ;
    }
}

void PA_mul_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case STR:
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case INT:
            case FLT:
                str_output("MULS\n");
                break;
            default:
                break; //shouldn't happen
        }
    } else {
        ;
    }
}

void PA_LT_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case STR:
            case INT:
            case FLT:
                str_output("LTS\n");
                break;
            default:
                break; //shouldn't happen
        }
        //change type to BOL
        curType = BOL;
    } else {
        ;
    }
}

void PA_EQ_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case STR:
            case INT:
            case FLT:
                str_output("EQS\n");
                break;
            default:
                break; //shouldn't happen
        }
        //change type to BOL
        curType = BOL;
    } else {
        ;
    }
}

void PA_GT_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case STR:
            case INT:
            case FLT:
                str_output("GTS\n");
                break;
            default:
                break; //shouldn't happen
        }
        //change type to BOL
        curType = BOL;
    } else {
        ;
    }
}

void PA_LE_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case STR:
            case INT:
            case FLT:
                str_output("GTS\n");
                str_output("NOTS\n");
                break;
            default:
                break; //shouldn't happen
        }
        //change type to BOL
        curType = BOL;
    } else {
        ;
    }
}

void PA_NE_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case STR:
            case INT:
            case FLT:
                str_output("EQS\n");
                str_output("NOTS\n");
                break;
            default:
                break; //shouldn't happen
        }
        //change type to BOL
        curType = BOL;
    } else {
        ;
    }
}

void PA_GE_sem() {
    if (!firstPass) {
        zeroLast = false;
        switch (curType) {
            case BOL:
                //Error: uncompatible types in an expression
                sem_t_expr_err();
                break; //just avoiding fallthrough warning
            case STR:
            case INT:
            case FLT:
                str_output("LTS\n");
                str_output("NOTS\n");
                break;
            default:
                break; //shouldn't happen
        }
        //change type to BOL
        curType = BOL;
    } else {
        ;
    }
}
