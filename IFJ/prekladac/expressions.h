/**
 * @file expressions.h
 * IVS20 compiler
 * @authors xvenge01
 * @brief the expressions header
 */

#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

#include "scanner.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>


//collumns/lines in precedence table
#define MAX_NUMBER 17

//chooses what to do from precedence table
typedef enum precTableSigns {
   S=0,//<
   R,  //>
   E,  //=
   H   //#
} precTableSigns;


/**
 * elements of precedence table
 * indexes of table + E from rules and shiftsign
*/
typedef enum precSymbol {
    S_PLUS=0,    
    S_MINUS, //1
    S_DIV,
    S_MULTI,
    S_INT,
    S_DOUBLE,//5
    S_ID,
    S_GT,
    S_LT,//8
    S_GE,
    S_LE,//10
    S_NE,
    S_EQ,
    S_LBRACKET,//13
    S_RBRACKET,
    S_DOLLAR,//15
    S_STRING,
    S_SHIFT,
    S_E   //17    //non term = E
} precSymbol;


/**
 * All rules in gramatic
 */
typedef enum precRules {
    R_E =0,      //E->id or E->string or E->int or E->double
    R_PLUS_E ,   //E->E+E
    R_MINUS_E,   //E->E-E
    R_LB_E_RB,   //E->(E)
    R_DIV_E,     //E->E/E
    R_MUL_E,     //E->E*E
    R_GT_E,      //E->E>E
    R_LT_E,      //E->E<E
    R_GE_E,      //E->E>=E
    R_LE_E,      //E->E<=E
    R_NE_E,      //E->E!=E
    R_EQ_E,      //E->E==E
    R_ERROR,     //rule not define
} precRules;


////////////////////Stack//////////////////////
/**
 * Item of stack
 */
typedef struct stackItem{
    precSymbol symbol;
    struct stackItem *nextPtr;
    struct stackItem *prevPtr;
}StackItem;

typedef struct tstack{
    StackItem *topPtr;
    StackItem *curPtr;
}TStack;

///////////////////////////////////////////////

// extern TStack **s_ptr;


/**
 * Precedence table
 */
static const int precTable [MAX_NUMBER] [MAX_NUMBER] = {
//          |  + | -| /| *| int|dou|id| >| <|>=|<=|!=|==| (| )| $|str
/* +        */ {R, R, S, S, S,  S,  S,  R, R, R, R, R, R, S, R, R, S  },
/* -        */ {R, R, S, S, S,  S,  S,  R, R, R, R, R, R, S, R, R, S  },
/* /        */ {R, R, R, R, S,  S,  S,  R, R, R, R, R, R, S, R, R, S },
/* *        */ {R, R, R, R, S,  S,  S,  R, R, R, R, R, R, S, R, R, S  },
/* int      */ {R, R, R, R, H,  H,  H,  R, R, R, R, R, R, H, R, R, H  },
/* double   */ {R, R, R, R, H,  H,  H,  R, R, R, R, R, R, H, R, R, H  },
/* id       */ {R, R, R, R, H,  H,  H,  R, R, R, R, R, R, H, R, R, H  },
/* >        */ {S, S, S, S, S,  S,  S,  H, H, H, H, H, H, S, R, R, S  },
/* <        */ {S, S, S, S, S,  S,  S,  H, H, H, H, H, H, S, R, R, S  },
/* >=       */ {S, S, S, S, S,  S,  S,  H, H, H, H, H, H, S, R, R, S  },
/* <=       */ {S, S, S, S, S,  S,  S,  H, H, H, H, H, H, S, R, R, S  },
/* !=       */ {S, S, S, S, S,  S,  S,  H, H, H, H, H, H, S, R, R, S  },
/* ==       */ {S, S, S, S, S,  S,  S,  H, H, H, H, H, H, S, R, R, S  },
/* (        */ {S, S, S, S, S,  S,  S,  S, S, S, S, S, S, S, E, H, S  },
/* )        */ {R, R, R, R, H,  H,  H,  R, R, R, R, R, R, H, R, R, H  },
/* $        */ {S, S, S, S, S,  S,  S,  S, S, S, S, S, S, S, H, H, S  },
/* string   */ {R, R, R, R, H,  H,  H,  R, R, R, R, R, R, H, R, R, H  }
};

/**
 * Public function prototypes
 */
bool Expr(tFileString *inputFile, tToken *tmpToken, tToken *optToken);
void delete_rest_stack();

#endif //EXPRESSIONS_H
