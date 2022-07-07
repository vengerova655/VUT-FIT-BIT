/**
 * @file symtable.c
 * IFJ20 compiler
 * @authors xvisku01
 * @brief the symtable
 */

#include "symtable.h"

////////////////////////////////////////////////////////////////////////////////

/**
 * Private function prototypes
 */

int hashCode(tKey key);
tSymTable *symInit(tSymTable *ptrht);
tSymItem *symSearch(tSymTable *ptrht, tKey key);
tSymItem *symInsert(tSymTable *ptrht, tKey key);
void symClearAll(tSymTable *ptrht);
void DeleteArgLists(tSymItem *item);

////////////////////////////////////////////////////////////////////////////////

/**
 * Sym(Hash)Table functions
 */

/**
 * @brief Hashes key into number to evenly distribute items across hash table
 * @returns Number coresponding to given key
 */
int hashCode(tKey key) {
    uint32_t hash = 0;
    const unsigned char *p;
    for (p = (const unsigned char *)key; *p != '\0'; p++)
        hash = 65599 * hash + *p;
    return (hash % HTSIZE);
}

/**
 * @brief Initializes SymTable
 * @returns ptr to SymTable
 */
tSymTable *symInit(tSymTable *ptrht) {

    ptrht = (tSymTable *)malloc(sizeof(tSymTable));
    if (!ptrht) {
        return NULL;
    }

    for (int i = 0; i < HTSIZE; i++) {
        (*ptrht)[i] = NULL;
    }
    return ptrht;
}

/**
 * @brief Searches hash table for item with matching key
 * @returns Ptr to item
 */
tSymItem *symSearch(tSymTable *ptrht, tKey key) {

    tSymItem *tempElem = (*ptrht)[hashCode(key)];

    while (tempElem != NULL) {
        if (!strcmp(tempElem->key, key)) {
            return tempElem;
        }

        tempElem = tempElem->ptrnext;
    }
    return NULL;
}

/**
 * @brief Inserts new item into hash table
 * @returns Ptr to newly created item
 */
tSymItem *symInsert(tSymTable *ptrht, tKey key) {

    tSymItem *tempElem = symSearch(ptrht, key);

    if (tempElem) {
        return NULL;
    }
    tSymItem *newElem = (tSymItem *)malloc(sizeof(tSymItem));
    if (!newElem) {
        return NULL;
    }
    newElem->key = key;
    newElem->ptrnext = (*ptrht)[hashCode(key)];
    newElem->argList = NULL;
    newElem->returnList = NULL;

    (*ptrht)[hashCode(key)] = newElem;

    return newElem;
}

/**
 * @brief Deletes and frees whole hash table
 */
void symClearAll(tSymTable *ptrht) {

    tSymItem *tempElem;
    tSymItem *ctrlElem;

    if (!ptrht) {
        return;
    }

    for (int i = 0; i < HTSIZE; i++) {
        ctrlElem = (*ptrht)[i];
        tempElem = ctrlElem;

        while (ctrlElem != NULL) {
            tempElem = ctrlElem;
            ctrlElem = ctrlElem->ptrnext;

            DeleteArgLists(tempElem);
            free(tempElem);
            tempElem = NULL;
        }

        ctrlElem = NULL;
        (*ptrht)[i] = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////

/*
 * Argument/Retrun type list functions
 */

/**
 * @brief Adds new item into list of arguments of function
 * @returns 1 if error occured otherwise 0
 */
int AddListArg(tSymItem **item, tAttrType type, char *name) {
    tAttrList *newAttr = (struct tAttrList *)malloc(sizeof(tAttrList));
    if (!newAttr) {
        return 1;
    }
    newAttr->type = type;
    newAttr->name = name;
    newAttr->ptrnext = (*item)->argList;
    (*item)->argList = newAttr;
    return 0;
}

/**
 * @brief Adds new item into list of return values of function
 * @returns 1 if error occured otherwise 0
 */
int AddListRet(tSymItem **item, tAttrType type, char *name) {
    tAttrList *newAttr = (struct tAttrList *)malloc(sizeof(tAttrList));
    if (!newAttr) {
        return 1;
    }
    newAttr->type = type;
    newAttr->name = name;
    newAttr->ptrnext = (*item)->returnList;
    (*item)->returnList = newAttr;
    return 0;
}

/**
 * @brief Deletes both the list of arguments and return values of given item
 */
void DeleteArgLists(tSymItem *item) {
    tAttrList *ctrlAttr;
    tAttrList *tempAttr;

    ctrlAttr = item->argList;
    while (ctrlAttr) {
        tempAttr = ctrlAttr;
        ctrlAttr = ctrlAttr->ptrnext;
        free(tempAttr);
    }
    ctrlAttr = item->returnList;
    while (ctrlAttr) {
        tempAttr = ctrlAttr;
        ctrlAttr = ctrlAttr->ptrnext;
        free(tempAttr);
    }
    ctrlAttr = NULL;
    tempAttr = NULL;
}

////////////////////////////////////////////////////////////////////////////////

/*
 * Stack functions
 */ 

/**
 * @brief Initiliazes stack
 * @returns 1 if error occured otherwise 0
 */
int InitStack(tStack **L) {
    *L = (tStack *)malloc(sizeof(tStack));

    if (!(*L)) {
        return 1;
    }

    //since first element was malloced ^, you need to init this element
    (*L)->hTable = symInit((*L)->hTable);
    if ((*L)->hTable == NULL) {
        return 1;
    }
    (*L)->ID = 0;
    (*L)->ptrnext = NULL;

    return 0;
}

/**
 * @brief Deletes whole stack
 */
void DisposeStack(tStack **L) {
    while ((*L)) {
        DeleteTopScope(L);
    }
    //deletes all scopes ^, so now all that's left is to invalidate pointer
    *L = NULL;
}

/**
 * @brief Inserts new scope into the stack
 * @returns 1 if error occured otherwise 0
 */
int InsertNewScope(tStack **L) {
    static int IDcnt = 1;
    tElem *newElem = malloc(sizeof(struct tElem));

    //when entering new function, the indexes reset
    if ((*L) != NULL && (*L)->ID == 0) {
        IDcnt = 1;
    }

    if (newElem) {
        newElem->ptrnext = (*L);
        tSymTable *newTable = NULL;
        newTable = symInit(newTable);
        if (!newTable) {
            return 1;
        }
        newElem->hTable = newTable;
        newElem->ID = IDcnt++;
        (*L) = newElem;
        return 0;
    } else {
        return 1;
    }
}

/**
 * @brief Deletes top scope from stack
 */
void DeleteTopScope(tStack **L) {
    if ((*L) != NULL) {
        tElem *newElem;
        newElem = (*L);
        (*L) = (*L)->ptrnext;
        symClearAll(newElem->hTable);
        free(newElem->hTable);
        free(newElem);
    }
}

/**
 * @brief Searches for item with given key in top scope
 * @returns Ptr to item if found, NULL otherwise
 * @returns ID of scope where item was found through pointer passed in argument
 */
tSymItem *SearchTop(tStack *L, tKey key, int *ID) {
    if (L) {
        tSymItem *searchItem;
        searchItem = symSearch(L->hTable, key);
        if (searchItem) {
            *ID = L->ID;
            return searchItem;
        }
    }
    *ID = -1;
    return NULL;
}

/**
 * @brief Searches for item with given key in whole stack
 * @returns Ptr to item if found, NULL otherwise
 * @returns ID of scope where item was found through pointer passed in argument
 */
tSymItem *SearchAll(tStack *L, tKey key, int *ID) {
    tElem *tempElem;
    tSymItem *searchElem;

    tempElem = L;

    while (tempElem) {
        searchElem = symSearch(tempElem->hTable, key);
        if (searchElem) {
            *ID = tempElem->ID;
            return searchElem;
        }
        tempElem = tempElem->ptrnext;
    }
    *ID = -1;
    return NULL;
}

/**
 * @brief Inserts new item into top scope
 * @returns Ptr to inserted item, NULL if error
 */
tSymItem *InsertNewItem(tStack *L, tKey key, int *ID) {
    *ID = GetTopID(L);
    return symInsert(L->hTable, key);
}

/**
 * @brief Finds ID of top scope
 * @returns ID of top scope
 */
int GetTopID(tStack *L) {
    return L != NULL ? L->ID
                     : -1;
}
