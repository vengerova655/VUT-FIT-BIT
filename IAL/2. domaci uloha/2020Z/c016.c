
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Položkami
**                      První implementace: Petr Přikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masařík, říjen 2014
**                              Radek Hranický, 2014-2018
**
** Vytvořete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Položkami = Hash table)
** s explicitně řetězenými synonymy. Tabulka je implementována polem
** lineárních seznamů synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku před prvním použitím
**  HTInsert ..... vložení prvku
**  HTSearch ..... zjištění přítomnosti prvku v tabulce
**  HTDelete ..... zrušení prvku
**  HTRead ....... přečtení hodnoty prvku
**  HTClearAll ... zrušení obsahu celé tabulky (inicializace tabulky
**                 poté, co již byla použita)
**
** Definici typů naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelů na položky, jež obsahují složky
** klíče 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na další synonymum 'ptrnext'. Při implementaci funkcí
** uvažujte maximální rozměr pole HTSIZE.
**
** U všech procedur využívejte rozptylovou funkci hashCode.  Povšimněte si
** způsobu předávání parametrů a zamyslete se nad tím, zda je možné parametry
** předávat jiným způsobem (hodnotou/odkazem) a v případě, že jsou obě
** možnosti funkčně přípustné, jaké jsou výhody či nevýhody toho či onoho
** způsobu.
**
** V příkladech jsou použity položky, kde klíčem je řetězec, ke kterému
** je přidán obsah - reálné číslo.
*/
//vypracovala Veronika Vengerova -> xvenge01
#include "c016.h"
#include <stdio.h>
int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode ( tKey key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {
	for (int i = 0; i < HTSIZE; i++)
	{
		(*ptrht)[i]=NULL;
	}
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	//vygeneruje index
	int index = hashCode(key);
	//prvok na danom indexi v tabulke
	tHTItem *prvok = (*ptrht)[index];
	while(prvok){
		//najdeny prvok ktory hladame
		if(strcmp(prvok->key,key)==0){
			return prvok;
		}
		//posun v liste
		prvok = prvok->ptrnext;
	}

	return NULL;//not found
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {
	//vrati najdeny item
	tHTItem *found = htSearch(ptrht,key);
	
	//uz sa nachadza v tabulke-> iba zmenime hodnotu
	if(found!=NULL){
		found->data = data;
		return;
	}

	//nebol najdeny, musi sa vytvorit druuhy
	tHTItem *new_item = (tHTItem *)malloc(sizeof(struct tHTItem));
	//ci spravna alokacia
	if(new_item == NULL){
		return;
	}

	//index
	int index = hashCode(key);

	//nastavenie udajov
	new_item->key = key;
	new_item->data = data;
	//nadvezovanie
	new_item->ptrnext = (*ptrht)[index];
	(*ptrht)[index] = new_item;

}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	tHTItem *found = htSearch(ptrht,key);
	//nenajdeny prvok
	if(found == NULL){
		return NULL;
	}
	//vracia data najdeneho
	return &(found)->data;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {
	//index/hash
	int index = hashCode(key);
	tHTItem *tmp = (*ptrht)[index];
	tHTItem *prev = NULL;
	
	//nie je v vtabulke
	if(tmp == NULL) return;

	//najde predchadzajuci ktoreho treba vymazat 
	while(tmp!= NULL && strcmp(tmp->key,key)){
		prev = tmp;
		tmp = tmp->ptrnext;
	}

	//nie je v vtabulke
	if(tmp == NULL) return;
	//mazeme korenoveho
	if(prev == NULL){
		(*ptrht)[index] = tmp->ptrnext;
		free(tmp);
	}
	else
	{
		//nastavenie nasledujuceho
		prev->ptrnext = tmp->ptrnext;
		free(tmp);
	}
	
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	//od zaciatku
	int index =0;
	
	while(index < HTSIZE){
		
		tHTItem *tmp = (*ptrht)[index];
		tHTItem *prev = NULL;
		//uvolnuje vsetky prvky listu
		while(tmp){
			prev = tmp;
			tmp = tmp->ptrnext;
			free(prev);
		}
		//nastavuje na null
		(*ptrht)[index]=NULL;

		//posun v tabulke
		index++;
	}
}
