/**
 * @file expressions.c
 * IVS20 compiler
 * @authors xvenge01
 * @brief the expressions
 */

#include "scanner.h"
#include "expressions.h"
#include "semgen.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * Private function prototypes
 */

static void SInit(TStack *s);
static void Push(TStack *s, precSymbol symbol);
static void Push_after_current(TStack *s, precSymbol symbol);
static void Pop(TStack *s);
static precSymbol get_token_symbol(tToken *tmpToken);
static precSymbol get_top_stack_symbol(TStack *s);
static precSymbol get_cur_stack_symbol(TStack *s);
static precRules reduce(TStack *s);
static precTableSigns get_operation(precSymbol stlpec, precSymbol riadok);
static precRules reduce_rule_3op(precSymbol op1, precSymbol op2, precSymbol op3);

////////////////////////////////////////////////////////////////////////////////
TStack **s_ptr = NULL;


/**
 * Private function definitions
 */
/**
 * @brief deletes and frees whole stack
 * 
 */
void DeleteStack(TStack *s) {
    while (s->topPtr != NULL) {
        Pop(s);
    }
    s->curPtr = NULL;
    free(s);
    s_ptr = NULL;
}

void delete_rest_stack(){
    if(s_ptr!=NULL){
        DeleteStack(*s_ptr);
    }
}
/**
 * @brief Initializes stack
 */
static void SInit(TStack *s) {
    s->topPtr = NULL;
    s->curPtr = NULL;
    Push(s, S_DOLLAR);
}

/**
 * @brief pushes on top of the stack item with parameters
 */
static void Push(TStack *s, precSymbol symbol) {
    StackItem *newItemPtr = (StackItem *)malloc(sizeof(StackItem));
    if (newItemPtr != NULL) {
        newItemPtr->symbol = symbol;
        if (s->topPtr != NULL) {
            newItemPtr->nextPtr = s->topPtr->nextPtr;
            s->topPtr->nextPtr = newItemPtr;

        } else {
            newItemPtr->nextPtr = NULL;
        }

        newItemPtr->prevPtr = s->topPtr;
        s->curPtr = newItemPtr;
        s->topPtr = newItemPtr;
    }
}

/**
 * @brief pushes item with parameter on top of the current item in stack
 */
static void Push_after_current(TStack *s, precSymbol symbol) {
    StackItem *newItemPtr = (StackItem *)malloc(sizeof(StackItem));
    if (newItemPtr != NULL) {

        newItemPtr->symbol = symbol;
        if (s->topPtr != NULL && s->curPtr != NULL) {

            newItemPtr->nextPtr = s->curPtr->nextPtr;
            s->curPtr->nextPtr->prevPtr = newItemPtr;
            s->curPtr->nextPtr = newItemPtr;

        } else {
            newItemPtr->nextPtr = NULL;
        }

        newItemPtr->prevPtr = s->curPtr;
        s->curPtr = newItemPtr;
    }
}

/**
 * @brief pops itm on top of the stack
 */
static void Pop(TStack *s) {
    StackItem *itemPtr;
    if (s->topPtr != NULL) {
        itemPtr = s->topPtr;
        if (s->curPtr == s->topPtr) {
            s->curPtr = s->curPtr->prevPtr;
        }
        s->topPtr = s->topPtr->prevPtr;
        free(itemPtr);
    }
}

/**
 * @brief gets symbol from the item on top of the stack
 * @returns top stack item symbol
 */
static precSymbol get_top_stack_symbol(TStack *s) {
    if (s->topPtr != NULL) {
        return (s->topPtr->symbol);
    }
    return S_DOLLAR;
}

/**
 * @brief gets symbol from the current stack item
 * @returns current stack item symbol
 */
static precSymbol get_cur_stack_symbol(TStack *s) {
    if (s->curPtr != NULL) {
        return (s->curPtr->symbol);
    }
    return S_DOLLAR;
}

/**
 * @brief reduces the stack according to rules
 * @returns rule that was used to reduce the stack
 * @details if no rule found the stack can be reduced with -> R_ERROR
 */
static precRules reduce(TStack *s) {

    //number of symbols while reducing rule
    int count = 0;

    s->curPtr = s->topPtr;
    precSymbol symbol = get_top_stack_symbol(s);

    while (symbol != S_DOLLAR) {
        if (symbol == S_SHIFT) {
            break;
        }

        count++;
        s->curPtr = s->curPtr->prevPtr;
        symbol = get_cur_stack_symbol(s);
    }

    if (count == 1) {
        symbol = get_top_stack_symbol(s);
        if (symbol == S_INT || symbol == S_DOUBLE || symbol == S_ID || symbol == S_STRING) {
            Pop(s);
            // print_Stack(s);
            if (s->curPtr->symbol == s->topPtr->symbol) {
                if (s->curPtr->symbol == S_SHIFT) {
                    Pop(s);
                }
            }
            return R_E;
        } else {
            return R_ERROR;
        }
    }
    // max number of symbols in rule = 3 -> more = ERROR
    if (count == 3) {
        precSymbol symbol2;
        precSymbol symbol3;

        s->curPtr = s->curPtr->nextPtr;
        symbol = get_cur_stack_symbol(s);

        s->curPtr = s->curPtr->nextPtr;
        symbol2 = get_cur_stack_symbol(s);

        s->curPtr = s->curPtr->nextPtr;
        symbol3 = get_cur_stack_symbol(s);

        Pop(s); //remove symbol on stack top
        Pop(s);
        Pop(s); 

        //deleting the < sign if on stack
        if (s->curPtr->symbol == s->topPtr->symbol) {
            if (s->curPtr->symbol == S_SHIFT) {
                Pop(s);
            }
        }
        return reduce_rule_3op(symbol, symbol2, symbol3);
    } else {
        return R_ERROR;
    }

    return R_ERROR; // should never happen
}

/**
 * @brief gets rule that was used with 3 symbols
 * @returns returns rule that was used with 3 symbols
 */
static precRules reduce_rule_3op(precSymbol s1, precSymbol s2, precSymbol s3) {
    if (s1 != S_DOLLAR && s2 != S_DOLLAR && s3 != S_DOLLAR) {
        if (s1 == S_LBRACKET && s2 == S_E && s3 == S_RBRACKET) {
            return R_LB_E_RB;
        }
        if (s1 == S_E && s3 == S_E) {
            switch (s2) {
                case S_PLUS:
                    PA_plus_sem(); //good place?
                    return R_PLUS_E;
                case S_MINUS:
                    PA_minus_sem();
                    return R_MINUS_E;
                case S_DIV:
                    PA_div_sem();
                    return R_DIV_E;
                case S_MULTI:
                    PA_mul_sem();
                    return R_MUL_E;
                case S_GT:
                    PA_GT_sem();
                    return R_GT_E;
                case S_LT:
                    PA_LT_sem();
                    return R_LT_E;
                case S_GE:
                    PA_GE_sem();
                    return R_GE_E;
                case S_LE:
                    PA_LE_sem();
                    return R_LE_E;
                case S_NE:
                    PA_NE_sem();
                    return R_NE_E;
                case S_EQ:
                    PA_EQ_sem();
                    return R_EQ_E;
                default:
                    return R_ERROR;
            }
        }
        return R_ERROR;
    }
    return R_ERROR;
}

/**
 * @brief gets the index of line/column of PT
 * @returns returns the index of line/column of PT
 */
static precSymbol get_token_symbol(tToken *tmpToken) {
    switch (tmpToken->type) {
        case T_PLUS:
            return S_PLUS;
        case T_MINUS:
            return S_MINUS;
        case T_DIV:
            return S_DIV;
        case T_MULTI:
            return S_MULTI;
        case T_ID:
            PA_ID_sem(tmpToken);
            return S_ID;
        case T_INT:
            PA_INT_sem(tmpToken);
            return S_INT;
        case T_STRING:
            PA_STRING_sem(tmpToken);
            return S_STRING;
        case T_DOUBLE:
            PA_FLOAT_sem(tmpToken);
            return S_DOUBLE;
        case T_EQUALS:
            return S_EQ;
        case T_INEQUALS:
            return S_NE;
        case T_LE:
            return S_LE;
        case T_LT:
            return S_LT;
        case T_GE:
            return S_GE;
        case T_GT:
            return S_GT;
        case T_LBRACKET:
            return S_LBRACKET;
        case T_RBRACKET:
            return S_RBRACKET;
        default:
            return S_DOLLAR;
    }
}

/**
 * @brief gets from the table what needs to be done with expression
 * @returns returns the sign in PT
 */
static precTableSigns get_operation(precSymbol collumn, precSymbol line) {
    return precTable[line][collumn];
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Public function definitions
 */

/**
 * @brief checks expression if it is correct
 * @returns returns true->no syntax error, or false-> syntax error found
 */
bool Expr(tFileString *inputFile, tToken *tmpToken, tToken *optToken) {

    int lbrCount = 0;
    int rbrCount = 0; 

    TStack *s = (TStack *)malloc(sizeof(struct tstack));
    
    SInit(s);
    s_ptr=&s;

    //line index of precedence table
    precSymbol line = 0;
    //collumn index of precedence table
    precSymbol column = 0;

    //symbol representing token
    precSymbol tokensymbol;

    precRules rule = R_ERROR;
    int reduced = 1;
    tokensymbol = get_token_symbol(tmpToken);

    if (tokensymbol == S_LBRACKET) {
        lbrCount++;
    } else if (tokensymbol == S_RBRACKET) {

        rbrCount++;
        if (lbrCount < rbrCount) {
            tokensymbol = S_DOLLAR;
        }
    }
    
    column = tokensymbol;
    while (column != S_DOLLAR) {
        //if last time reduced from rule->token not processed yet
        if (reduced == 0) {
            //if operand or S_LBRACKET-> in the next one EOL ignore
            if ((tokensymbol >= S_PLUS && tokensymbol <= S_MULTI) || (tokensymbol >= S_GT && tokensymbol <= S_LBRACKET)) {
                if (optToken == NULL || optToken->type == T_EOL) {
                    *tmpToken = get_token(inputFile, EOL_IGNR);
                    optToken = NULL;
                } else {
                    *tmpToken = *optToken;
                    optToken = NULL;
                }
            } else {
                if (optToken == NULL) {
                    *tmpToken = get_token(inputFile, EOL_RET);
                } else {
                    *tmpToken = *optToken;
                    optToken = NULL;
                }
            }
            //index column PT
            tokensymbol = get_token_symbol(tmpToken);
            if (tokensymbol == S_LBRACKET) {
                lbrCount++;

            } else if (tokensymbol == S_RBRACKET) {

                rbrCount++;
                if (lbrCount < rbrCount) {
                    tokensymbol = S_DOLLAR;
                }
            }
            
            column = tokensymbol;
        }

        reduced = 0; //next time can get token unless reducing again

        tokensymbol = get_top_stack_symbol(s);
        if (tokensymbol == S_E) {
            //get first terminal to get operation
            while (s->curPtr->symbol == S_E && tokensymbol != S_DOLLAR) {
                s->curPtr = s->curPtr->prevPtr;
                tokensymbol = get_cur_stack_symbol(s);
            }
        }

        //top terminal on stack
        line = tokensymbol;
        precTableSigns operation = get_operation(column, line);
        switch (operation) {
            case S:
                if (get_top_stack_symbol(s) == S_E) {
                    //push after current on stack
                    Push_after_current(s, S_SHIFT);
                } else {
                    //push on top
                    Push(s, S_SHIFT);
                }
                //push terminal on top of the stack
                Push(s, column);
                // print_Stack(s);
                break;
            case H:
                DeleteStack(s);
                return false;
            case E:
                Push(s, column);
                // print_Stack(s);
                break;
            case R:
                rule = reduce(s);
                if (rule != R_ERROR) {
                    Push(s, S_E);
                    s->curPtr = s->topPtr;
                    // print_Stack(s);
                } else {
                    DeleteStack(s);
                    return false;
                }

                //reduced->terminal is not processed yet
                reduced = 1;
                break;
            default:
                //should never happen
                break;
        }
        tokensymbol = column;
    }

    //rest of the stack-> if can be reduced to $E -> correct syntax
    while (s->topPtr->symbol != S_DOLLAR) {
        if (s->topPtr->symbol == S_E) {
            if (s->topPtr->prevPtr->symbol == S_DOLLAR) {
                //on top of the stack only $E -> correct end state
                break;
            }
        }
        rule = reduce(s);
        // print_Stack(s);

        if (rule != R_ERROR) {
            Push(s, S_E);
            s->curPtr = s->topPtr;
            // print_Stack(s);
        } else {
            DeleteStack(s);
            return false;
        }
    }
    DeleteStack(s);
    after_expr_sem(tmpToken);

    if (rule == R_ERROR) {
        //syntax error -> no rule we can reduce symbols with
        return false;
    } else {
        return true;
    }
}
