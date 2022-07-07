/**
 * @file main.c
 * IVS20 compiler
 * @authors xkucma00
 * @brief contains the main function
 */

#include "errors.h"
#include "parser.h"
#include "semgen.h"
#include <stdbool.h>
#include <stdio.h>

int main() {
    FILE *input = stdin;
    tFileString strIn;
    if (getFileString(input, &strIn) != 0) {
        internal_err();
    }

    if (!parse(&strIn)) {
        syntax_err();
    }
    strIn.i = 0; //return to the start
    if (!parse(&strIn))
        syntax_err();
    scanner_end();

    return 0;
}
