
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvořil: Petr Přikryl, listopad 1994
**                                         Úpravy: Andrea Němcová, prosinec 1995
**                                                      Petr Přikryl, duben 1996
**                                                   Petr Přikryl, listopad 1997
**                                  Převod do jazyka C: Martin Tuček, říjen 2005
**                                         Úpravy: Bohuslav Křena, listopad 2009
**                                                 Karel Masařík, říjen 2013
**                                                 Radek Hranický 2014-2018
**
** Implementujte rekurzivním způsobem operace nad binárním vyhledávacím
** stromem (BVS; v angličtině BST - Binary Search Tree).
**
** Klíčem uzlu stromu je jeden znak (obecně jím může být cokoliv, podle
** čeho se vyhledává). Užitečným (vyhledávaným) obsahem je zde integer.
** Uzly s menším klíčem leží vlevo, uzly s větším klíčem leží ve stromu
** vpravo. Využijte dynamického přidělování paměti.
** Rekurzivním způsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíčem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zrušení uzlu se zadaným klíčem
**   BSTDispose ... zrušení celého stromu
**
** ADT BVS je reprezentován kořenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíč (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Přesnou definici typů
** naleznete v souboru c401.h.
**
** Pozor! Je třeba správně rozlišovat, kdy použít dereferenční operátor *
** (typicky při modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem
** (např. při vyhledávání). V tomto příkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, použijeme dereferenci.
**/

#include "c401.h"
int solved;
//vypracovala VEronika Vengerova -> xvenge01
void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**
** Ověřit, zda byl již strom předaný přes RootPtr inicializován, nelze,
** protože před první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor využívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to před vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by totiž mohlo vést ke ztrátě přístupu
** k dynamicky alokované paměti (tzv. "memory leak").
**
** Všimněte si, že se v hlavičce objevuje typ ukazatel na ukazatel.
** Proto je třeba při přiřazení přes RootPtr použít dereferenční operátor *.
** Ten bude použit i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/
	*RootPtr = NULL;
}

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**
** Při vyhledávání v binárním stromu bychom typicky použili cyklus ukončený
** testem dosažení listu nebo nalezení uzlu s klíčem K. V tomto případě ale
** problém řešte rekurzivním volání této funkce, přičemž nedeklarujte žádnou
** pomocnou funkci.
**/
	//korenovy nie je prazdny
	if(RootPtr!=NULL){
		//najdeny
		if(RootPtr->Key == K){
			//nastavenie hodnoty najdeneho
			*Content = RootPtr->BSTNodeCont;
			return TRUE;
		}
		//hladame v lavom podstrome
		else if(RootPtr->Key > K)
		{
			//zavolame s novym korenom(lavym uzlom sucasneho)
			return(BSTSearch(RootPtr->LPtr,K,Content));
		}
		//hladame v pravom podstrome
		else
		{
			//hladame s novym korenom (pravym uzlom sucasneho)
			return(BSTSearch(RootPtr->RPtr,K,Content));
		}
	}
	//nenajdeny
	return FALSE;
}


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{
/*   ---------
** Vloží do stromu RootPtr hodnotu Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**
** Funkci implementujte rekurzivně. Nedeklarujte žádnou pomocnou funkci.
**
** Rekurzivní implementace je méně efektivní, protože se při každém
** rekurzivním zanoření ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto případě byla efektivnější jak z hlediska
** rychlosti, tak z hlediska paměťových nároků. Zde jde ale o školní
** příklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
	if (*RootPtr!=NULL)
	{
		//posunieme sa do laveho podstromu
		if((*RootPtr)->Key > K)
		{
			BSTInsert(&(*RootPtr)->LPtr,K,Content);
		}
		//posunieme sa do pravheo podstormu
		else if(K>(*RootPtr)->Key)
		{
			BSTInsert(&(*RootPtr)->RPtr,K,Content);
		}
		//uz sa nachadza uzol, iba prepiseme stare data na nove
		else
		{
			(*RootPtr)->BSTNodeCont = Content;
		}
	}
	else
	{
		
		//alokovanie noveho uzla
		tBSTNodePtr new_Node = (tBSTNodePtr)malloc(sizeof(struct tBSTNode));
		//ci uspesna alokacia
		if (new_Node == NULL){
			return;
		}
		//nastavenie hodnot noveho uzlu
		new_Node->Key=K;
		new_Node->BSTNodeCont = Content;
		//vzdy vkladame list -> LPTR = RPTR = NULL
		new_Node->LPtr=NULL;
		new_Node->RPtr=NULL;
		//napojenie na uzol
		(*RootPtr) = new_Node;
	}
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce). Tuto funkci implementujte rekurzivně.
**
** Tato pomocná funkce bude použita dále. Než ji začnete implementovat,
** přečtěte si komentář k funkci BSTDelete().
**/

	//najviac pravy uzol
	tBSTNodePtr right_Node = (*RootPtr);
	//najpravejsi uzol
	if (right_Node->RPtr == NULL)
	{
		//nastavenie hodnot njpravejsieho
		PtrReplaced->Key = right_Node->Key;
		PtrReplaced->BSTNodeCont = right_Node->BSTNodeCont;
		return;	
	}
	//este sa nachadza na pravej strane uzlu nejaky
	else{	
		right_Node = right_Node->RPtr;
		//posunieme doprava
		ReplaceByRightmost(PtrReplaced,&right_Node);
	}	
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivně s využitím dříve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	//uzol, ktory potrebujeme vymazat
	tBSTNodePtr node_to_Delete;

	//prazdny strom -> nic sa nedeje
	if((*RootPtr)==NULL){
		return;
	}
	
	//hladany kluc vacsi ako zucasneho uzlu
	if((*RootPtr)->Key < K ){
		//vieme sa dostat doprava
		if((*RootPtr)->RPtr!=NULL) {
			//kluc praveho uzlu = hladany kluc -> ten potrebujeme vymazat
			if((*RootPtr)->RPtr->Key == K){
				node_to_Delete = (*RootPtr)->RPtr;
				
				
				if(node_to_Delete->LPtr!=NULL){
					//uzol ma 2 podstormy
					if(node_to_Delete->RPtr!=NULL){
						ReplaceByRightmost(node_to_Delete,&(node_to_Delete->LPtr));
						//vymazanie premiestnenho prvku
						BSTDelete (&(node_to_Delete->LPtr), node_to_Delete->Key);
					}
					//uzol ma podstrom iba na lavej strane
					if (node_to_Delete->RPtr==NULL)
					{
						//prenastavenie nasledovatela otca
						(*RootPtr)->RPtr = node_to_Delete->LPtr;
						//uvolnenie
						free(node_to_Delete);
						return;
					}
				}

				if(node_to_Delete->LPtr==NULL){
					//ma iba podstrom na pravej strane
					if (node_to_Delete->RPtr!=NULL)
					{
						//prenastavenie nasledovatela otca
						(*RootPtr)->RPtr = node_to_Delete->RPtr;
						//uvolnenie
						free(node_to_Delete);
						return;
					}
				}

				//nema podstrom ani na jednej strane
				if(node_to_Delete->LPtr==NULL){
					if (node_to_Delete->RPtr==NULL)
					{
						(*RootPtr)->RPtr = NULL;
						free(node_to_Delete);
						return;
					}
				}
			}
		}
		//lavy podstrom
		BSTDelete(&((*RootPtr)->RPtr),K);
	}


	//hladany kluc mensi ako aktualneho uzla

	//solved = FALSE;		  /* V případě řešení smažte tento řádek! */
	if((*RootPtr)->Key > K ){
		//ma lavy podstrom
		if((*RootPtr)->LPtr != NULL) {
			//kluc laveho uzlu = hladany, tne potrebujeme vymazat
			if((*RootPtr)->LPtr->Key == K){
				node_to_Delete = (*RootPtr)->LPtr;
				
				
				if(node_to_Delete->LPtr!=NULL){
					//ma 2 podstromy
					if(node_to_Delete->RPtr!=NULL){
						ReplaceByRightmost(node_to_Delete,&(node_to_Delete->LPtr));
						//vymazanie premiestneneho prvku
						BSTDelete (&(node_to_Delete->LPtr), node_to_Delete->Key);
						return;
					}
					//ma podstrom iba na lavej strane
					if (node_to_Delete->RPtr==NULL)
					{
						//prenastavenie nasledovatela otca na lavej strane
						(*RootPtr)->LPtr = node_to_Delete->LPtr;
						//uvolnenie
						free(node_to_Delete);
						return;
					}
				}

				if(node_to_Delete->LPtr==NULL){
					//ma podstrom iba na pravej strane
					if (node_to_Delete->RPtr!=NULL)
					{
						//prenastavenie nasledovatela otca na lavej strane
						(*RootPtr)->LPtr = node_to_Delete->RPtr;
						//uvolnenie
						free(node_to_Delete);
						return;
					}
				}

				//nema podstromy
				if(node_to_Delete->LPtr==NULL){
					if (node_to_Delete->RPtr==NULL)
					{
						(*RootPtr)->LPtr = NULL;
						free(node_to_Delete);
						return;
					}
				}
			}
		}
		BSTDelete(&((*RootPtr)->LPtr),K);
	}

	//rovna sa kluc
	if((*RootPtr)->Key == K ){
		node_to_Delete = (*RootPtr);

		if(node_to_Delete->LPtr != NULL){
			//ma 2 podstromy
			if(node_to_Delete->RPtr!=NULL){
				ReplaceByRightmost(node_to_Delete,&(node_to_Delete->LPtr));
				//mazanie premiestneneho prvku
				BSTDelete (&(node_to_Delete->LPtr), node_to_Delete->Key);
			}
			//ma podstrom iba na lavej strane
			if (node_to_Delete->RPtr==NULL)
			{
				//prenastavenie nasledovatela otca
				(*RootPtr) = node_to_Delete->LPtr;
				//uvolnenie
				free(node_to_Delete);
				return;
			}
		}

		if(node_to_Delete->LPtr==NULL){
			//ma podstrom iba na pravej strane
			if (node_to_Delete->RPtr!=NULL)
			{
				//prenastavenie nasledovatela otca
				(*RootPtr)= node_to_Delete->RPtr;
				//uvolnenie
				free(node_to_Delete);
				return;
			}
		}

		//nema ziadny podstrom
		if(node_to_Delete->LPtr==NULL){
			if (node_to_Delete->RPtr==NULL)
			{
				(*RootPtr) = NULL;
				//uvolnenie
				free(node_to_Delete);
				return;
			}
		}
	}

	//ak neobsahuje kluc nic sa nedeje
}

void BSTDispose (tBSTNodePtr *RootPtr) {
/*   ----------
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**
** Po zrušení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivně bez deklarování pomocné
** funkce.
**/

	if((*RootPtr)!= NULL){
		//rusenie lavych prvkov
		BSTDispose(&((*RootPtr)->LPtr));
		//rusenie pravych prvkov
		BSTDispose(&((*RootPtr)->RPtr));
		//uvolnovanie prvkov
		free(*RootPtr);
		//nastavovanie uzlov na NULL
		*RootPtr =NULL;
	}
}

/* konec c401.c */

