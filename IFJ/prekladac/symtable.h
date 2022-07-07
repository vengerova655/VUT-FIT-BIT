/**
 * @file symtable.h
 * IFJ20 compiler
 * @authors xvisku01
 * @brief header for the symtable
 */
#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HTSIZE 10

typedef char* tKey;

typedef enum tType {
    VAR = 0,
    FUN
} tType;

typedef enum tAttrType {
    INT = 0,
    FLT,
    STR,
    BOL,
    NONE,
} tAttrType;

typedef struct tAttrList {
    tAttrType type;
    char* name; //no longer neccessary, rework code or keep for compatibility?
    struct tAttrList* ptrnext;
} tAttrList;

typedef struct tSymItem{
	tKey key;
    tType type;
    tAttrType attrType; //only if VAR
    struct tAttrList* argList; //only if FUN
    struct tAttrList* returnList; //only if FUN  
	struct tSymItem* ptrnext; //private
} tSymItem;

typedef tSymItem* tSymTable[HTSIZE];

//private
typedef struct tElem {
    tSymTable* hTable;
    int ID;
    struct tElem* ptrnext;
} tElem;

typedef tElem tStack;

int InitStack(tStack** L);
void DisposeStack(tStack** L);
int InsertNewScope(tStack** L);
void DeleteTopScope(tStack** L);
tSymItem* SearchTop(tStack* L, tKey key, int* ID);
tSymItem* SearchAll(tStack* L, tKey key, int* ID);
tSymItem* InsertNewItem(tStack* L, tKey key, int* ID);
int AddListRet(tSymItem** item, tAttrType type, char* name);
int AddListArg(tSymItem** item, tAttrType type, char* name);
int GetTopID(tStack* L);

#endif
