/**
 * @file scanner.h
 * IFJ20 compiler
 * @authors xvenge01, xkucma00
 * @brief header for the scanner
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SCANNER_H
#define SCANNER_H

/**
 * @typedef flag telling scanner whether to ignore EOLs or return them as tokens
 */
typedef enum eolFlags {
    EOL_IGNR = 0, //end of line ignored
    EOL_RET,      //end of line returned as token
} tEolFlag;

/**
 * @typedef types of tokens
 */
typedef enum tokenTypes {
    //0-4
    T_ERR = 0, //lexical error
    T_EOF,     //end of file
    T_EOL,     //end of line
    T_INT,     //integer literal
    T_DOUBLE,  //double literal

    //5-9
    T_STRING, //string literal
    T_PLUS,   //+
    T_MINUS,  //-
    T_DIV,    ///
    T_MULTI,  //*

    //10-14
    T_EQUALS,   //==
    T_INEQUALS, //!=
    T_LE,       //Lower Equal <=
    T_LT,       //Lower <
    T_GE,       //Greater Equal >=

    //15-19
    T_GT,    //Greater >
    T_COMMA, //Comma ,
    T_SEMIC, //Semicolon ;
    T_KEYWORD,
    T_LBRACKET, //Left bracket (

    //20-24
    T_RBRACKET,  //Right bracket )
    T_LCBRACKET, //Left composed bracket {
    T_RCBRACKET, //Right composed bracket }
    T_ID,        //Identifier
    T_ASSIGNM,   //Assignment =

    //25
    T_DEFIN,    //Definition of variable :=
    T_ERR_COMP, //compiler error
} tTokenType;

/**
 * @typedef possible keywords
 */
typedef enum keyWords {
    //0-4
    KW_ELSE = 0,
    KW_FLOAT64,
    KW_FOR,
    KW_FUNC,
    KW_IF,

    //5-9
    KW_INT,
    KW_PACKAGE,
    KW_RETURN,
    KW_STRING,
    KW_NONE, //not a keyword; must be last in enum
} tKeyWord;

/**
 * @typedef possible states of scanner FSM
 */
typedef enum states {
    ST_START = 0, //epsilon

    ST_EQ,    //=; F
    ST_COLON, //:
    //ST_DEFIN, //:=

    //bool operators
    //ST_EQEQ,   //==; F
    ST_EXCL, //!
    //ST_UNEQ,   //!=; F
    ST_LESS, //<; F
    //ST_LESSEQ, //<=; F
    ST_MORE, //>; F
    //ST_MOREEQ, //>=; F

    //arithmetic operands
    //ST_PLUS,  //+; F
    //ST_MINUS, //-; F
    //ST_MULTI, //*; F
    ST_DIV, //'/'; F

    //comments
    ST_COM_LN,      //"//"
    ST_COM_BLK_IN,  //"/*..."
    ST_COM_BLK_END, //"/*...*"

    //IDs and keywords
    ST_ID, //eg _as45; F

    //numbers
    ST_ZERO,     //0
    ST_INT,      //eg 15; F
    ST_FL_DOT,   //eg 15.
    ST_FL_DEC,   //eg 15.7; F
    ST_FL_E,     //eg 15.7e
    ST_FL_E_SGN, //eg 15.7e+
    ST_FL_EXP,   //eg 15.7e+20; F

    //strings ('"' in the comments is a part of the expression)
    ST_STR_IN, //"...
    //ST_STR_END,   //"..."; F
    ST_STR_ESC,   //eg ".../
    ST_STR_ESC_X, //eg ".../x
    ST_STR_HEX,   //eg ".../xA

    //punctuation
    //ST_SEMIC,     //';'; F
    //ST_COMMA,     //','; F
    //ST_LBRACKET,  //'('; F
    //ST_RBRACKET,  //')'; F
    //ST_LCBRACKET, //'{'; F
    //ST_RCBRACKET, //'}'; F
} tState;

/**
 * @struct structure of a token - type, potential attribute
 */
typedef struct token {
    tTokenType type;

    //can be string (STRING, ID), int (INT), double (DOUBLE), keyword (KEYWORD)
    union {
        int64_t integer;
        double real;
        char *string;
        tKeyWord keyword;
    } attribute;
} tToken;

typedef struct fileString {
    char *string;
    size_t i;
    size_t EOFidx;
} tFileString;

tToken get_token(tFileString *input, tEolFlag eolF);
int getFileString(FILE *in, tFileString *strIn);
void scanner_end();

#endif //SCANNER_H