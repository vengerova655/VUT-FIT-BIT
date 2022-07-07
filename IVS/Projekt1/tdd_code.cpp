//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - priority queue code
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     VERONIKA VENGEROVA <xvenge01@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author VERONIKA VENGEROVA
 * 
 * @brief Implementace metod tridy prioritni fronty.
 */

#include <stdlib.h>
#include <stdio.h>

#include "tdd_code.h"

//============================================================================//
// ** ZDE DOPLNTE IMPLEMENTACI **
//
// Zde doplnte implementaci verejneho rozhrani prioritni fronty (Priority Queue)
// 1. Verejne rozhrani fronty specifikovane v: tdd_code.h (sekce "public:")
//    - Konstruktor (PriorityQueue()), Destruktor (~PriorityQueue())
//    - Metody Insert/Remove/Find a GetHead
//    - Pripadne vase metody definovane v tdd_code.h (sekce "protected:")
//
// Cilem je dosahnout plne funkcni implementace prioritni fronty implementovane
// pomoci tzv. "double-linked list", ktera bude splnovat dodane testy 
// (tdd_tests.cpp).
//============================================================================//

PriorityQueue::PriorityQueue()
{
    Element_t *e = GetHead();
    e = NULL;
}

PriorityQueue::~PriorityQueue()
{
    Element_t *e = GetHead();
    while(e!=NULL){
        Element_t *tmp = e;
        e = e->pNext;
        delete tmp;
    }
}

void PriorityQueue::Insert(int value) {
    Element_t *e = GetHead();
    Element_t *tmp;
    while (e != NULL) {
            if (e->pNext == NULL && e->pPrev == NULL) {

                if (e->value < value) {

                    e->pNext = tmp;
                    tmp->value = value;
                    tmp->pPrev = e;
                    tmp->pNext = NULL;
                } else {
                    tmp->value = value;
                    tmp->pNext = e;
                    tmp->pPrev = NULL;
                    e->pPrev = tmp;
                }
            } else if (e->pNext != NULL && e->pPrev == NULL) {

                if (e->value > value) {
                    tmp->pNext = e;
                    tmp->value = value;
                    tmp->pPrev = NULL;
                    e->pPrev = tmp;
                } else {
                    e->pNext = tmp;
                    tmp->value = value;
                    tmp->pPrev = e;
                }
            } else if (e->pNext == NULL && e->pPrev != NULL) {

                if (e->value < value) {
                    e->pNext = tmp;
                    tmp->pPrev = e;
                    tmp->value = value;
                    tmp->pNext = NULL;
                } else {
                    tmp->pPrev = e->pPrev;
                    e->pPrev = tmp;
                    tmp->value = value;
                    tmp->pNext = e;
                }
            }
        else if(e->value >= value && e->pPrev->value <= value){

            tmp->pPrev = e->pPrev;
            tmp->pNext = e;
            tmp->value = value;
        }
        e = e->pNext;
    }
}

bool PriorityQueue::Remove(int value)
{
    return false;
}

PriorityQueue::Element_t *PriorityQueue::Find(int value)
{
    Element_t *e = GetHead();
    while(e!=NULL){
        if(e->value==value)
            break;
        e = e->pNext;
    }
    return e;
}

PriorityQueue::Element_t *PriorityQueue::GetHead()
{
    Element_t* e;
    return e;
}

/*** Konec souboru tdd_code.cpp ***/
