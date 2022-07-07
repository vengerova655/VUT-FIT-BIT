/* c201.c *********************************************************************}
{* Téma: Jednosměrný lineární seznam
**
**                     Návrh a referenční implementace: Petr Přikryl, říjen 1994
**                                          Úpravy: Andrea Němcová listopad 1996
**                                                   Petr Přikryl, listopad 1997
**                                Přepracované zadání: Petr Přikryl, březen 1998
**                                  Přepis do jazyka C: Martin Tuček, říjen 2004
**                                              Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ jednosměrný lineární seznam.
** Užitečným obsahem prvku seznamu je celé číslo typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou typu tList.
** Definici konstant a typů naleznete v hlavičkovém souboru c201.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ tList:
**
**      DisposeList ... zrušení všech prvků seznamu,
**      InitList ...... inicializace seznamu před prvním použitím,
**      InsertFirst ... vložení prvku na začátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zruší první prvek seznamu,
**      PostDelete .... ruší prvek za aktivním prvkem,
**      PostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na další prvek seznamu,
**      Active ........ zjišťuje aktivitu seznamu.
**
** Při implementaci funkcí nevolejte žádnou z funkcí implementovaných v rámci
** tohoto příkladu, není-li u dané funkce explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam předá
** někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/


/**
 * c201 IAL domaca uloha 1
 * @author Veronika Vengerova (xvenge01)
**/

#include "c201.h"

int errflg;
int solved;

void Error() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální proměnná -- příznak chyby */
}

void InitList ( tList *L ) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    //nastavenie na neaktivny
	L->Act = NULL;
    //nastavenie ze zoznam je prazdny
    L->First = NULL;
}

void DisposeList ( tList *L ) {
/*
** Zruší všechny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Veškerá paměť používaná prvky seznamu L bude korektně
** uvolněna voláním operace free.
***/

    // osetrenie volania pri prazdnom zozname
     if ( L->First != NULL ) {
        // alokovanie pamate pre pomocny prvok
        tElemPtr tmp = (tElemPtr) malloc(sizeof(struct tElem));
        
        //pri uspesnej alokacii
        if ( tmp != NULL ) {
            //cyklus na prejdenie celeho zoznamu
            do{
                //nastavenie pomocneho ukazatela na prvy prvok
                tmp->ptr = L->First;
                //obnovenie ukazatela prveho prvku na nasledujuci (v pripade, ze iba 1 prvok = NULL)
                L->First = tmp->ptr->ptr;
                //uvolnenie prveho prvku
                free(tmp->ptr);
            }while( L->First != NULL );
            //uvolnenie alokovanej pamate pomocneho ukazatela
            free(tmp);
        }
    }

    //obnovenie aktivity -> prazdny zoznam = neaktivny zoznam
    L->Act = NULL; 
}

void InsertFirst ( tList *L, int val ) {
/*
** Vloží prvek s hodnotou val na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci Error().
**/

    //alokovanie pamate pre novy prvok
    tElemPtr newElemP = (tElemPtr) malloc(sizeof(struct tElem));
	
    //osetrenie neuspesnej alokacie pamate
    if ( newElemP == NULL ) {
        Error();
    }
    else {
        //novy prvy prvok musi ukazovat na predchadzajuci prvy prvok
        newElemP->ptr = L->First;
        //nastavenie hodnoty prvku
        newElemP->data = val;
        //aktualizacia ukazatela na prvy prvok na novy prvy prvok
        L->First = newElemP; 
    }
}

void First ( tList *L ) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný příkaz, aniž byste testovali,
** zda je seznam L prázdný.
**/
    //aktivny prvok = prvy prvok
	L->Act = L->First;
}

void CopyFirst ( tList *L, int *val ) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/

    // osetrenie volania nad prazdnym zoznamom
	if ( L->First == NULL ) {
        Error();
    }
    else {
        //vracanie hodnoty
        *val = L->First->data;
    }
}

void DeleteFirst ( tList *L ) {
/*
** Zruší první prvek seznamu L a uvolní jím používanou paměť.
** Pokud byl rušený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/
    //vykonavanie akcie iba pri neprazdnom prvku
	if( L->First != NULL) {

        //pri odstraneni aktivneho prvku -> zoznam neaktivny
        if ( L->Act == L->First ) {
            L->Act = NULL;
        }

        //alokovnaie pamate pre pomocny prvok 
        tElemPtr tmp = (tElemPtr) malloc(sizeof(struct tElem));
        
        //pri uspesna alokacia
        if ( tmp != NULL ) {
            //nastavenie ukazatela na odstranovany prvok
            tmp->ptr = L->First;
            //posunutie ukazatela naprvy prvok na nasledujuci/NULL pri 1 prvku
            L->First = tmp->ptr->ptr;
            
            //uvolnenie ostraneneho prvku
            free(tmp->ptr);
            //uvolnenie pomocneho prvku
            free(tmp);
        }

    }
}	

void PostDelete ( tList *L ) {
/* 
** Zruší prvek seznamu L za aktivním prvkem a uvolní jím používanou paměť.
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se neděje.
**/

    //vykonavanie iba ak zoznam je aktivny
    if ( L->Act != NULL ){
        
        //vykonavanie iba ak aktivny prvok nie je poslendym prvkom zoznamu
        if ( L->Act->ptr != NULL ){

            //alokovanie pamate na pomocny prvok
            tElemPtr tmp = (tElemPtr) malloc(sizeof(struct tElem));
            
            //pri uspesnej alokacii
            if ( tmp != NULL ) {
                //nastavenie pomocneho ukazovatela na nasledujuci prvok
                tmp->ptr = L->Act->ptr;
                //aktualizacia na dalsi prvok
                L->Act->ptr = tmp->ptr->ptr;

                //uvolnenie odstranovaneho prvku
                free(tmp->ptr);
                free(tmp);
            }
        }
    }   
}

void PostInsert ( tList *L, int val ) {
/*
** Vloží prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje!
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** zavolá funkci Error().
**/
    //vykonanie iba ak zoznam aktivny
	if ( L->Act != NULL ){
        
        //uvolnenie pamate
        tElemPtr newElement = (tElemPtr) malloc(sizeof(struct tElem));
        
        //pri uspesnej alokacii
        if ( newElement == NULL ) {
            Error();
        }
        else {
            newElement->ptr = L->Act->ptr;
            L->Act->ptr = newElement;
            newElement->data = val;
        }
        
    }   
}

void Copy ( tList *L, int *val ) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/
    //osetrenie pri volani neaktivneho zoznamu
	if ( L->Act == NULL ){
        Error();
    }
    else
    {
        *val = L->Act->data;
    }
}

void Actualize ( tList *L, int val ) {
/*
** Přepíše data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedělá nic!
**/
	if ( L->Act != NULL ){
        L->Act->data = val;
    }
}

void Succ ( tList *L ) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Všimněte si, že touto operací se může aktivní seznam stát neaktivním.
** Pokud není předaný seznam L aktivní, nedělá funkce nic.
**/
	if ( L->Act != NULL ){
        L->Act = L->Act->ptr;
    }
}

int Active ( tList *L ) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Tuto funkci je vhodné implementovat jedním příkazem return. 
**/
    //True = 0 -> nenulova hdonota
	return ( L->Act != NULL );
}

/* Konec c201.c */
