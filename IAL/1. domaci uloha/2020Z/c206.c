
/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2020
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

/**
 * c206 IAL domaca uloha 1
 * @author Veronika Vengerova (xvenge01)
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;            
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->Act = NULL;
    L->First = NULL;
    L->Last = NULL;
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/
    //zrusenie aktivity
    L->Act = NULL;
    //pomocny ukazovatel
	tDLElemPtr tmp = (tDLElemPtr) malloc(sizeof(struct tDLElem));
	
    //pri neuspesnej alokacii
    if (tmp == NULL) {
        DLError();
    }//pri uspesnej alokacii
    else {
        //uvolnuje vsekty prvky
        while(L->Last != NULL){
            tmp->rptr = L->First;
            L->First = L->First->rptr;
            if(tmp->rptr == L->Last) {
                L->Last = NULL;
            }
            free(tmp->rptr);
        }
        //uvolnuje pomocny
        free(tmp);
    }
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	//novy prvok
	tDLElemPtr newtDElemP = (tDLElemPtr) malloc(sizeof(struct tDLElem));
	
    if (newtDElemP == NULL) {
        DLError();
    }
    else {
        //prvy prvok
        newtDElemP->rptr = L->First;
        //priradenie dat elementu
        newtDElemP->data = val;
        newtDElemP->lptr = NULL;
        
        L->First = newtDElemP;

        //ak list predtym prazdny, novy prvok je prvy ja posledny
        //ak list mal predtym nejake prvky zaradi na zaciatok
        if (newtDElemP->rptr != NULL) {
            newtDElemP->rptr->lptr = newtDElemP;
        }
        else {
            L->Last = newtDElemP;
        }
    }
}

void DLInsertLast(tDLList *L, int val) {	
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 
	tDLElemPtr newtDElemP = (tDLElemPtr) malloc(sizeof(struct tDLElem));
	
    if (newtDElemP == NULL) {
        DLError();
    }
    else {
        newtDElemP->data = val;
        newtDElemP->rptr = NULL;
        newtDElemP->lptr = L->Last;
        L->Last = newtDElemP;
        if(L->First == NULL) {
            L->First = newtDElemP;
        }
        else
        {
            newtDElemP->lptr->rptr = newtDElemP;
        }
        

    }
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->First;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    //prazdny zoznam
    if (L->First == NULL) {
        DLError();
    }
    else {
        *val = L->First->data;
    }
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    //prazdny zoznam
	if (L->First == NULL) {
        DLError();
    }
    else {
        *val = L->Last->data;
    }
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    if (L->First != NULL){

        //nicenie aktivity
        if(L->Act == L->First){
            L->Act = NULL;
        }

        //pomocne
        tDLElemPtr tmp = (tDLElemPtr) malloc(sizeof(struct tDLElem));
        
        if (tmp == NULL) {
            DLError();
        }
        else{
            //na vymazanie prvku bez straty zvysnych prvkov
            tmp->rptr = L->First;

            //nastavenie noveho prveho prvku
            L->First = tmp->rptr->rptr;
            if (L->Last == tmp->rptr){
                L->Last = NULL;
            }
            else {
                tmp->rptr->rptr->lptr = NULL;
            }
            //uvolnenie vymazavaneho prvku
            free(tmp->rptr);
            //uvolnenie pomocneho prvku
            free(tmp);
        }
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L.
** Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se neděje.
**/ 
    if (L->Last != NULL){

        //nicenie aktivity
        if(L->Act == L->Last){
            L->Act = NULL;
        }

        //pomocne
        tDLElemPtr tmp = (tDLElemPtr) malloc(sizeof(struct tDLElem));
        
        if (tmp == NULL) {
            DLError();
        }
        else{
            //uvolnovany prvok
            tmp->rptr = L->Last;

            //prenastavenie posledneho prvku
            L->Last = tmp->rptr->lptr;
            if (L->First == tmp->rptr){
                L->First = NULL;
            }
            else
            {
                L->Last->rptr = NULL;
            }
            //uvonovanie mazaneho prvku
            free(tmp->rptr);
            //uvolnenie pomocneho
            free(tmp);
        }
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
    //pri aktivnom zozname
	if( L->Act != NULL ){
        //aktivny prvok je psolendy
        if( L->Act != L->Last ){

            //pomocne
            tDLElemPtr tmp = (tDLElemPtr) malloc(sizeof(struct tDLElem));
            
            //chyba alokovania
            if (tmp == NULL) {
                DLError();
            }
            else
            {
                //prvok za aktivnym
                tmp->rptr =  L->Act->rptr;
                //uvolnovany prvok bol poslendy treba prenastavit ukazatel na posledneho
                if (L->Act->rptr == L->Last) {
                    L->Last=L->Act;
                }
                else
                {
                    tmp->rptr->rptr->lptr = L->Act;
                }
                
                //prenastavenie praveho ukazovatela na dalsi prvok zoznamu/jeho pokracovanie
                L->Act->rptr=tmp->rptr->rptr;

                //uvolnovanie prvku
                free(tmp->rptr);
                //uvolnovanie pomocneho
                free(tmp); 
            }
            
        }
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
	if( L->Act != NULL ){
        //aktivny prvok je prvy -> nema sa aky prvok zrusit
        if( L->Act != L->First ){

            //pomocne
            tDLElemPtr tmp = (tDLElemPtr) malloc(sizeof(struct tDLElem));
            
            //chyba pri alokacii
            if (tmp == NULL) {
                DLError();
            }
            else
            {
                //predosly prvok
                tmp->lptr =  L->Act->lptr;

                //ak prvok ktory rusime bol prvy, treba zmenit ukazatel na prvy 
                if(L->Act->lptr == L->First) {
                    L->First = L->Act;
                }
                else
                {
                    //nastavenie aby zoznam na seba nadvezoval a nestratili sa prvky
                    tmp->lptr->lptr->rptr = L->Act;
                }

                //obojstranny zoznam, musi ukazovat na prvok pred nami rusenym prvkom
                L->Act->lptr = tmp->lptr->lptr;
               
                //uvolnovanie prvku
                free(tmp->lptr);
                //uvolnenie pomocneho prvku
                free(tmp); 
            }
            
        }
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL) {
         //pomocne
        tDLElemPtr newtDElemP = (tDLElemPtr) malloc(sizeof(struct tDLElem));
        
        //chyba pri alokacii
        if (newtDElemP == NULL) {
            DLError();
        }
        else{
            //vkladany sa napaja pred prvok za aktivnym
            newtDElemP->rptr = L->Act->rptr;

            //po vlozeni zap solendy prvok je novy poslendy prvok
            if(L->Act == L->Last){
                L->Last = newtDElemP;
            }
            else {
                L->Act->rptr->lptr = newtDElemP;
            }
            //spravne 'napojenie' do obojstranneho zonamu
            L->Act->rptr = newtDElemP;
            newtDElemP->lptr = L->Act;

            //nastavenie hodnoty
            newtDElemP->data = val;
        }

    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
	if (L->Act != NULL) {
         //pomocne
        tDLElemPtr newtDElemP = (tDLElemPtr) malloc(sizeof(struct tDLElem));
        
        //neuspesna alokacia
        if (newtDElemP == NULL) {
            DLError();
        }
        else{
            //novy prvok priradujeme pred akt. prvok v zozname
            newtDElemP->lptr = L->Act->lptr;
            //ak vkladame pred prvy prvok, vlozeny je novy prvy
            if(L->Act == L->First){
                L->First = newtDElemP;
            }
            else {
                L->Act->lptr->rptr = newtDElemP;
            }

            //spravne 'napojenie' do obojstranneho zonamu
            L->Act->lptr = newtDElemP;
            newtDElemP->rptr = L->Act;
            
            //pridelenie hodnoty
            newtDElemP->data = val;
        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    //volanie pri neaktivnom
    if(L->Act == NULL) {
        DLError();
    }
    else
    {
        *val = L->Act->data;
    }
}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
	if ( L->Act != NULL ) {
        L->Act->data = val;
    }
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL) {
        L->Act = L->Act->rptr;
    }
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if(L->Act != NULL) {
        L->Act = L->Act->lptr;
    }
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
	return (L->Act != NULL);
}

/* Konec c206.c*/
