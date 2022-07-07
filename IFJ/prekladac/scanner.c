/**
 * @file scanner.c
 * IFJ20 compiler
 * @authors xvenge01 xkucma00
 * @brief the scanner
 */

#include "scanner.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

////////////////////////////////////////////////////////////////////////////////

#define BUFF_DEFAULT 128         //chunk size for a buffer string
#define STRINGS_DEFAULT 64       //chunk amount for a array of pointers to strings
#define STRING_FILE_DEFAULT 1024 //chunk amount for a string loading file

////////////////////////////////////////////////////////////////////////////////

/**
 * Private type definitions, global variables, function parameters
 */

//Global variables
static char *toFree;

//Functions
static char *append_buff(int c, bool reset);
static char *append_buff_dec(int c);
static char *save_string(char *string);
static int digit_to_hex(int c);
static tKeyWord kw_check(char *id);

////////////////////////////////////////////////////////////////////////////////

/**
 * Private function definitions
 */

/**
 * @brief appends character c to buffer string; if EOF, frees memory instead
 *      if reseting, doesn't append
 * @returns ptr to buffer string on success; NULL on error and when freeing
 */
static char *append_buff(int c, bool reset) {
    static char *buff = NULL;
    static size_t max = 0;
    static size_t i = 0; //index

    //freeing
    if (c == EOF) {
        free(buff);
        return NULL;
    }

    //reseting
    if (reset) {
        i = 0;
        return NULL;
    }

    //if size exceeded, reallocing
    if (i + 1 > max) {
        char *tmp = realloc(buff, max + BUFF_DEFAULT);
        if (tmp == NULL) {
            return NULL;
        }
        buff = tmp;
        max += BUFF_DEFAULT;
    }
    //appending
    buff[i] = c;
    i++;
    return buff;
}

/**
 * @brief appends char to buff as decadic escape sequence,
 *  using append_buff function
 * @pre c is char with ascii value 0 <= c <= 255
 * @returns ptr to buffer string on success; NULL on error
 */
static char *append_buff_dec(int c) {
    if (append_buff('\\', false) == NULL) {
        return NULL;
    }
    if (append_buff((c / 100) % 10 + '0', false) == NULL) {
        return NULL;
    }
    if (append_buff((c / 10) % 10 + '0', false) == NULL) {
        return NULL;
    }
    return append_buff(c % 10 + '0', false);
}

/**
 * @brief saves string in memory; if NULL, frees memory instead
 * @pre string is NULL, or valid '\0' terminated string
 * @returns ptr to saved string on success; NULL on error and when freeing
 */
static char *save_string(char *string) {
    static char **strings = NULL; //array of strings
    static size_t max = 0;
    static size_t i = 0; //index

    //freeing
    if (string == NULL) {
        for (size_t j = 0; j < i; j++) {
            free(strings[j]);
        }
        free(strings);
        return NULL;
    }

    //if size exceeded, reallocing
    if (i + 1 > max) {
        char **tmp = (char **)realloc(strings,
                                      (max + STRINGS_DEFAULT) * sizeof(char *));
        if (tmp == NULL) {
            return NULL;
        }
        strings = tmp;
        max += STRINGS_DEFAULT;
    }

    //asprintf(strings + i, "%s", string);
    strings[i] = malloc(strlen(string) + 1);
    if (strings[i] == NULL) {
        return NULL;
    }
    strcpy(strings[i], string);
    i++;
    return strings[i - 1];
}

/**
 * @brief hex char to digit
 * @pre number is a hex char (0-9 or A-F or a-f)
 * @returns value of hex char c
 */
static int digit_to_hex(int c) {
    if ('A' <= c && c <= 'F') {
        return c - 'A' + 10;
    }
    if ('a' <= c && c <= 'f') {
        return c - 'a' + 10;
    }
    if ('0' <= c && c <= '9') {
        return c - '0';
    }
    return -1; //should never happen
}

/**
 * @brief determines if id matches Keyword and which
 * @pre ID is a valid string
 * @returns valid keyword if match found, else err keyword
 */
static tKeyWord kw_check(char *id) {
    //must be in same order as enum
    static char *keywords[KW_NONE] = {
        "else",
        "float64",
        "for",
        "func",
        "if",
        "int",
        "package",
        "return",
        "string",
    };
    //i must be defined with the first keyword (KW_ELSE currently)
    for (tKeyWord i = KW_ELSE; i < KW_NONE; i++) {
        if (strcmp(keywords[i], id) == 0) {
            return i; //i equals enum value of given keyword
        }
    }
    return KW_NONE; //if no match found (and i == KW_NONE)
}

static int stringGetC(tFileString *input) {
    if (input->i == input->EOFidx) {
        return EOF;
    }
    input->i++;
    return input->string[input->i - 1];
}

static void stringUngetC(int c, tFileString *input) {
    if (c == EOF) {
        return;
    }
    if (c != input->string[input->i - 1]) {
        fprintf(stderr, "INVALID stringUngetC\n"); //debug
    }
    input->i--;
    return;
}

////////////////////////////////////////////////////////////////////////////////

/**
 * Public function definitions
 */

int getFileString(FILE *in, tFileString *strIn) {
    strIn->string = NULL;
    strIn->i = 0;
    strIn->EOFidx = 0;
    int c = fgetc(in);
    size_t max = 0;
    while (c != EOF) {
        if (strIn->i + 1 > max) {
            char *tmp = realloc(strIn->string, max + STRING_FILE_DEFAULT);
            if (tmp == NULL) {
                return 1;
            }
            strIn->string = tmp;
            max += STRING_FILE_DEFAULT;
        }
        strIn->string[strIn->i] = c;
        strIn->i++;
        c = fgetc(in);
    }
    strIn->EOFidx = strIn->i;
    strIn->i = 0;
    toFree = strIn->string;
    return 0;
}

/**
 * @brief frees scanner memory
 */
void scanner_end() {
    append_buff(EOF, false);
    save_string(NULL);
    free(toFree);
}

/**
 * @brief reads from input file to generate tokens
 * @returns valid token on success, error token on failure
 * @details strings are returned in format required by IFJ20code
 */
tToken get_token(tFileString *inputfile, tEolFlag eolF) {

    tFileString *input = inputfile;
    ///*
    tState state = ST_START;
    tToken tkn;
    int c;
    int tmpChar;
    char *tmpStr;
    tkn.type = T_ERR; //initializing (so that it can be used in if condition)
    while (1) {
        c = stringGetC(input);

        //if ending, break (to return)
        //if moving to a new state, continue
        switch (state) {
        //can return: EOF, {, }, (, ), ',', ';', +, -, *, err, err comp
        //next possible states: EQ, DIV, EXCL, MORE, LESS, COLON, STR_IN, ID,
        //INT
        case ST_START:
            switch (c) {
            case '\n':
                if (eolF == EOL_RET) {
                    tkn.type = T_EOL;
                    break;
                } else {
                    continue;
                }
            case EOF:
                tkn.type = T_EOF;
                break;
            case '{':
                tkn.type = T_LCBRACKET;
                break;
            case '}':
                tkn.type = T_RCBRACKET;
                break;
            case '(':
                tkn.type = T_LBRACKET;
                break;
            case ')':
                tkn.type = T_RBRACKET;
                break;
            case ',':
                tkn.type = T_COMMA;
                break;
            case ';':
                tkn.type = T_SEMIC;
                break;
            case '=':
                state = ST_EQ;
                continue;
            case '+':
                tkn.type = T_PLUS;
                break;
            case '-':
                tkn.type = T_MINUS;
                break;
            case '*':
                tkn.type = T_MULTI;
                break;
            case '/':
                state = ST_DIV;
                continue;
            case '!':
                state = ST_EXCL;
                continue;
            case '>':
                state = ST_MORE;
                continue;
            case '<':
                state = ST_LESS;
                continue;
            case ':':
                state = ST_COLON;
                continue;
            case '"':
                state = ST_STR_IN;
                append_buff(0, true); //resets
                continue;
            case '0':
                state = ST_ZERO;
                continue;
            default:
                if (isalpha(c) || c == '_') {

                    state = ST_ID;
                    append_buff(0, true); //resets

                    if (append_buff(c, false) == NULL) {

                        tkn.type = T_ERR_COMP;
                        break;
                    };
                    continue;
                } else if (isdigit(c)) {
                    state = ST_INT;
                    append_buff(0, true); //resets
                    if (append_buff(c, false) == NULL) {
                        tkn.type = T_ERR_COMP;
                        break;
                    };
                    continue;
                } else if (isspace(c)) {
                    // tkn.type = T_ERR;
                    // break;
                    continue;
                } else {
                    tkn.type = T_ERR;
                    break;
                    // continue;
                }
                break;
            }
            break;

        //can return: =, ==
        //next possible states: none,
        case ST_EQ:
            if (c == '=') {
                tkn.type = T_EQUALS;
            } else {
                stringUngetC(c, input);
                tkn.type = T_ASSIGNM;
            }
            break;

        //can return: /
        //next possible states: COM_LN, COM_BLK_IN
        case ST_DIV:
            if (c == '/') {
                state = ST_COM_LN;
                continue;
            } else if (c == '*') {
                state = ST_COM_BLK_IN;
                continue;
            } else {
                stringUngetC(c, input);
                tkn.type = T_DIV;
                break;
            }

        //can return: none
        //next possible states: COM_LN, START
        case ST_COM_LN:
            if (c == '\n') {
                stringUngetC(c, input);
                state = ST_START;
            } else if (c == EOF) {
                tkn.type = T_EOF;
                break;
            } //else, stay in this state
            continue;

        //can return: err
        //next possible states: COM_BLK_IN, COM_BLK_END
        case ST_COM_BLK_IN:
            if (c == EOF) {
                tkn.type = T_ERR;
                break;
            } else if (c == '*') {
                state = ST_COM_BLK_END;
                continue;
            } else if (c == '\n' && eolF == EOL_RET) {
                tkn.type = T_EOL; //changes type, but continues (!)
                continue;
            } else {
                continue;
            }
        //can return: err
        //next possible states: COM_BLK_IN, COM_BLK_END, START
        case ST_COM_BLK_END:
            if (c == EOF) {
                tkn.type = T_ERR;
                break;
            } else if (c == '/') {
                //if found at least 1 EOL and eofF is EOL_RET, returns EOL token
                if (tkn.type == T_EOL) {
                    break;
                } else {
                    state = ST_START;
                    continue;
                }
            } else if (c == '*') {
                continue;
            } else if (c == '\n' && eolF == EOL_RET) {
                tkn.type = T_EOL; //changes type, but continues as with else (!)
                state = ST_COM_BLK_IN;
                continue;
            } else {
                state = ST_COM_BLK_IN;
                continue;
            }

        //can return: !=, err
        //next possible states: none
        case ST_EXCL:
            if (c == '=') {
                tkn.type = T_INEQUALS;
            } else {
                stringUngetC(c, input);
                tkn.type = T_ERR;
            }
            break;
        //can return: >, >=
        //next possible states: none
        case ST_MORE:
            if (c == '=') {
                tkn.type = T_GE;
            } else {
                stringUngetC(c, input);
                tkn.type = T_GT;
            }
            break;
        //can return: <, <=
        //next possible states: none
        case ST_LESS:
            if (c == '=') {
                tkn.type = T_LE;
            } else {
                stringUngetC(c, input);
                tkn.type = T_LT;
            }
            break;

        //can return: :=, err
        //next possible states: none
        case ST_COLON:
            if (c == '=') {
                tkn.type = T_DEFIN;
            } else {
                stringUngetC(c, input);
                tkn.type = T_ERR;
            }
            break;

        //can return: string, err, err comp
        //next possible states: STR_ESC
        case ST_STR_IN:
            if (c == '"') {
                //null terminate string
                tmpStr = append_buff('\0', false);
                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }

                //save it in the memory
                tmpStr = save_string(tmpStr);
                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }

                //return token
                tkn.type = T_STRING;
                tkn.attribute.string = tmpStr;
                break;

            } else if (c == '\\') {
                state = ST_STR_ESC;
                continue;
            } else if (isprint(c)) {
                if (c == '#' || isspace(c)) {
                    if (append_buff_dec(c) == NULL) {
                        tkn.type = T_ERR_COMP;
                        break;
                    }
                } else {
                    if (append_buff(c, false) == NULL) {
                        tkn.type = T_ERR_COMP;
                        break;
                    }
                }
                continue; //remain in this state
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: err
        //next possible states: STR_IN, STR_ESC_X
        case ST_STR_ESC:
            tmpChar = 0;
            switch (c) {
            case '\\':
                tmpChar = '\\';
                break;
            case 'n':
                tmpChar = '\n';
                break;
            case 't':
                tmpChar = '\t';
                break;
            case '"':
                tmpChar = '"';
                break;
            case 'x':
                state = ST_STR_ESC_X;
                continue;
            }
            //if quotes, try to append, change state
            if (tmpChar == '"' && append_buff(tmpChar, false) != NULL) {
                state = ST_STR_IN;
                continue;
            } //if other valid escape sequence, appending as dec escape sequence
            else if (tmpChar != 0 && append_buff_dec(tmpChar) != NULL) {
                state = ST_STR_IN;
                continue;
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: err
        //next possible states: STR_HEX
        case ST_STR_ESC_X:
            if (isxdigit(c)) {
                //tmp char from previous state
                tmpChar = digit_to_hex(c) * 16;
                state = ST_STR_HEX;
                continue;
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: err, err comp
        //next possible states: STR_IN
        case ST_STR_HEX:
            if (isxdigit(c)) {
                tmpChar += digit_to_hex(c);
                if (isprint(tmpChar) && tmpChar != '#' && tmpChar != '\\') {
                    if (append_buff(tmpChar, false) == NULL) {
                        tkn.type = T_ERR_COMP;
                        break;
                    }
                } else {
                    //tmpChar is confirmed to have value from 0-255 (incl.)
                    if (append_buff_dec(tmpChar) == NULL) {
                        tkn.type = T_ERR_COMP;
                        break;
                    }
                }
                state = ST_STR_IN;
                continue;
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: ID, keyword, err comp
        //next possible states: none
        case ST_ID:
            if (isalnum(c) || c == '_') {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                continue; //stay in this state
            } else {
                stringUngetC(c, input);

                tmpStr = append_buff('\0', false);
                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }

                tKeyWord kw = kw_check(tmpStr);

                if (kw != KW_NONE) {
                    tkn.type = T_KEYWORD;
                    tkn.attribute.keyword = kw;
                    break;
                }
                tmpStr = save_string(tmpStr);
                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }

                tkn.type = T_ID;
                tkn.attribute.string = tmpStr;
                break;
            }

        //can return: int, err
        case ST_ZERO:
            if (isdigit(c)) {
                //zero has to be alone
                tkn.type = T_ERR;
                break;
            } else if (c == '.') {
                append_buff(0, true);
                if (append_buff('0', false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                if (append_buff('.', false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_DOT;
                continue;
            } else {
                stringUngetC(c, input);
                tkn.type = T_INT;
                tkn.attribute.integer = 0;
                break;
            }

        //can return: int, err, err comp
        //next possible states: INT, FL_DOT, FL_E
        case ST_INT:
            if (isdigit(c)) {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                continue;
            } else if (c == '.') {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_DOT;
                continue;
            } else if (c == 'e' || c == 'E') {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_E;
                continue;
            } else {

                stringUngetC(c, input);

                tmpStr = append_buff('\0', false);

                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                //tmpStr should now be a string containting a valid number
                //strtoll guarantees 64b
                tkn.type = T_INT;
                tkn.attribute.integer = strtoll(tmpStr, NULL, 10);
                break;
            }

        //can return: err, err comp
        //next possible states: FL_DEC
        case ST_FL_DOT:
            if (isdigit(c)) {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_DEC;
                continue;
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: double, err comp
        //next possible states: FL_DEC, FL_E
        case ST_FL_DEC:
            if (isdigit(c)) {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                continue;
            } else if (c == 'e' || c == 'E') {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_E;
                continue;
            } else {

                stringUngetC(c, input);

                tmpStr = append_buff('\0', false);

                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                //tmpStr should now be a string containting a valid number
                //strtod guarantees 64b
                tkn.type = T_DOUBLE;
                tkn.attribute.real = strtod(tmpStr, NULL);
                break;
            }

        //can return: err, err comp
        //next possible states: FL_E_SGN, FL_EXP
        case ST_FL_E:
            if (isdigit(c)) {

                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_EXP;
                continue;
            } else if (c == '+' || c == '-') {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }

                state = ST_FL_E_SGN;

                continue;
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: err, err comp
        //next possible states: FL_EXP
        case ST_FL_E_SGN:
            if (isdigit(c)) {

                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_EXP;
                continue;
            } else {
                tkn.type = T_ERR;
                break;
            }

        //can return: double, err comp
        //next possible states: FL_EXP
        case ST_FL_EXP:
            if (isdigit(c)) {
                if (append_buff(c, false) == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                state = ST_FL_EXP;
                continue;
            } else {
                stringUngetC(c, input);

                tmpStr = append_buff('\0', false);
                if (tmpStr == NULL) {
                    tkn.type = T_ERR_COMP;
                    break;
                }
                //tmpStr should now be a string containting a valid number
                //strtod guarantees 64b
                tkn.type = T_DOUBLE;
                tkn.attribute.real = strtod(tmpStr, NULL);
                break;
            }
        }
        state = ST_START;
        return tkn; //if didn't continue
    }
}
