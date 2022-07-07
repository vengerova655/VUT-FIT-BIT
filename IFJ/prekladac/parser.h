/**
 * @file parser.h
 * IFJ20 compiler
 * @authors xvosci00
 * @brief header for the parser
 */

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdbool.h>
#include "scanner.h"
#include "symtable.h"
#include "expressions.h"

bool parse(tFileString *inputFile);

#endif //PARSER_H
