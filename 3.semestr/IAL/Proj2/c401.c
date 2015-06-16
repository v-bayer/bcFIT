
/* c401.c: **********************************************************}
{* Téma: Rekurzivní implementace operací nad BVS
**                                         Vytvoøil: Petr Pøikryl, listopad 1994
**                                         Úpravy: Andrea Nìmcová, prosinec 1995
**                                                      Petr Pøikryl, duben 1996
**                                                   Petr Pøikryl, listopad 1997
**                                  Pøevod do jazyka C: Martin Tuèek, øíjen 2005
**                                         Úpravy: Bohuslav Køena, listopad 2009
**                                         Úpravy: Karel Masaøík, øíjen 2013
**                                         Úpravy: Radek Hranický, øíjen 2014
**
** Implementujte rekurzivním zpùsobem operace nad binárním vyhledávacím
** stromem (BVS; v angliètinì BST - Binary Search Tree).
**
** Klíèem uzlu stromu je jeden znak (obecnì jím mù¾e být cokoliv, podle
** èeho se vyhledává). U¾iteèným (vyhledávaným) obsahem je zde integer.
** Uzly s men¹ím klíèem le¾í vlevo, uzly s vìt¹ím klíèem le¾í ve stromu
** vpravo. Vyu¾ijte dynamického pøidìlování pamìti.
** Rekurzivním zpùsobem implementujte následující funkce:
**
**   BSTInit ...... inicializace vyhledávacího stromu
**   BSTSearch .... vyhledávání hodnoty uzlu zadaného klíèem
**   BSTInsert .... vkládání nové hodnoty
**   BSTDelete .... zru¹ení uzlu se zadaným klíèem
**   BSTDispose ... zru¹ení celého stromu
**
** ADT BVS je reprezentován koøenovým ukazatelem stromu (typ tBSTNodePtr).
** Uzel stromu (struktura typu tBSTNode) obsahuje klíè (typu char), podle
** kterého se ve stromu vyhledává, vlastní obsah uzlu (pro jednoduchost
** typu int) a ukazatel na levý a pravý podstrom (LPtr a RPtr). Pøesnou definici typù 
** naleznete v souboru c401.h.
**
** Pozor! Je tøeba správnì rozli¹ovat, kdy pou¾ít dereferenèní operátor *
** (typicky pøi modifikaci) a kdy budeme pracovat pouze se samotným ukazatelem 
** (napø. pøi vyhledávání). V tomto pøíkladu vám napoví prototypy funkcí.
** Pokud pracujeme s ukazatelem na ukazatel, pou¾ijeme dereferenci.
**/

#include "c401.h"
int solved;

void BSTInit (tBSTNodePtr *RootPtr) {
/*   -------
** Funkce provede poèáteèní inicializaci stromu pøed jeho prvním pou¾itím.
**
** Ovìøit, zda byl ji¾ strom pøedaný pøes RootPtr inicializován, nelze,
** proto¾e pøed první inicializací má ukazatel nedefinovanou (tedy libovolnou)
** hodnotu. Programátor vyu¾ívající ADT BVS tedy musí zajistit, aby inicializace
** byla volána pouze jednou, a to pøed vlastní prací s BVS. Provedení
** inicializace nad neprázdným stromem by toti¾ mohlo vést ke ztrátì pøístupu
** k dynamicky alokované pamìti (tzv. "memory leak").
**	
** V¹imnìte si, ¾e se v hlavièce objevuje typ ukazatel na ukazatel.	
** Proto je tøeba pøi pøiøazení pøes RootPtr pou¾ít dereferenèní operátor *.
** Ten bude pou¾it i ve funkcích BSTDelete, BSTInsert a BSTDispose.
**/

	*RootPtr = NULL;
}	

int BSTSearch (tBSTNodePtr RootPtr, char K, int *Content)	{
/*  ---------
** Funkce vyhledá uzel v BVS s klíèem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v promìnné Content se
** vrací obsah pøíslu¹ného uzlu.´Pokud pøíslu¹ný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah promìnné Content není definován (nic do ní proto
** nepøiøazujte).
**
** Pøi vyhledávání v binárním stromu bychom typicky pou¾ili cyklus ukonèený
** testem dosa¾ení listu nebo nalezení uzlu s klíèem K. V tomto pøípadì ale
** problém øe¹te rekurzivním volání této funkce, pøièem¾ nedeklarujte ¾ádnou
** pomocnou funkci.
**/
							   
	if ( RootPtr == NULL )	// strom je prazdny
		return FALSE;
	else{	// jestlize neni prazdny
		if ( RootPtr->Key == K){	// nasel
			*Content = RootPtr->BSTNodeCont;  //vracime obsah uzlu
			return TRUE;
		}else{
			if ( RootPtr->Key > K )	// jestli je klic vetsi
				return BSTSearch(RootPtr->LPtr, K, Content);	// hledej v levem podstromu
			else
				return BSTSearch(RootPtr->RPtr, K, Content); // hledej v pravem podstromu
		}
	}
} 


void BSTInsert (tBSTNodePtr* RootPtr, char K, int Content)	{	
/*   ---------
** Vlo¾í do stromu RootPtr hodnotu Content s klíèem K.
**
** Pokud ji¾ uzel se zadaným klíèem ve stromu existuje, bude obsah uzlu
** s klíèem K nahrazen novou hodnotou. Pokud bude do stromu vlo¾en nový
** uzel, bude vlo¾en v¾dy jako list stromu.
**
** Funkci implementujte rekurzivnì. Nedeklarujte ¾ádnou pomocnou funkci.
**
** Rekurzivní implementace je ménì efektivní, proto¾e se pøi ka¾dém
** rekurzivním zanoøení ukládá na zásobník obsah uzlu (zde integer).
** Nerekurzivní varianta by v tomto pøípadì byla efektivnìj¹í jak z hlediska
** rychlosti, tak z hlediska pamì»ových nárokù. Zde jde ale o ¹kolní
** pøíklad, na kterém si chceme ukázat eleganci rekurzivního zápisu.
**/
		
	if ( *RootPtr != NULL ){	// pokud strom neni prazdny, hledej nove misto
		if ( (*RootPtr)->Key < K )
			BSTInsert(&((*RootPtr)->RPtr), K, Content);	// hledej v levem podstromu
		else if ( (*RootPtr)->Key > K )
			BSTInsert(&((*RootPtr)->LPtr), K, Content); // hledej v pravem podstromu
		else
			(*RootPtr)->BSTNodeCont = Content;
	}else{	// strom je prazdny

		tBSTNodePtr new_ptr;	// vytvor uzel
		new_ptr = malloc(sizeof(struct tBSTNode));
		if ( new_ptr == NULL )
			return;	// ukonci

		new_ptr->Key = K;
		new_ptr->BSTNodeCont = Content;
		new_ptr->LPtr = NULL;
		new_ptr->RPtr = NULL;
		*RootPtr = new_ptr;
	}
	
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
/*   ------------------
** Pomocná funkce pro vyhledání, pøesun a uvolnìní nejpravìj¹ího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude pøesunuta hodnota
** nejpravìj¹ího uzlu v podstromu, který je urèen ukazatelem RootPtr.
** Pøedpokládá se, ¾e hodnota ukazatele RootPtr nebude NULL (zajistìte to
** testováním pøed volání této funkce). Tuto funkci implementujte rekurzivnì. 
**
** Tato pomocná funkce bude pou¾ita dále. Ne¾ ji zaènete implementovat,
** pøeètìte si komentáø k funkci BSTDelete(). 
**/
	
	tBSTNodePtr help_ptr = NULL; //	pomocny ukazatel

	if ( *RootPtr != NULL ){
		if( (*RootPtr)->RPtr != NULL )	// overeni, jestli je opravdu nejpravejsi
			ReplaceByRightmost(PtrReplaced, (&((*RootPtr)->RPtr)));	// posun doprava
		else{
			PtrReplaced->Key = (*RootPtr)->Key;  //	prepis klic
			PtrReplaced->BSTNodeCont = (*RootPtr)->BSTNodeCont; // prepis obsah
			help_ptr = *RootPtr;	// preulozeni ukazatele
			*RootPtr = (*RootPtr)->LPtr; // uvolneni uzlu

			free(help_ptr);	// odstraneni pomocneho ukazatele
		}
	}
	
}

void BSTDelete (tBSTNodePtr *RootPtr, char K) 		{
/*   ---------
** Zru¹í uzel stromu, který obsahuje klíè K.
**
** Pokud uzel se zadaným klíèem neexistuje, nedìlá funkce nic. 
** Pokud má ru¹ený uzel jen jeden podstrom, pak jej zdìdí otec ru¹eného uzlu.
** Pokud má ru¹ený uzel oba podstromy, pak je ru¹ený uzel nahrazen nejpravìj¹ím
** uzlem levého podstromu. Pozor! Nejpravìj¹í uzel nemusí být listem.
**
** Tuto funkci implementujte rekurzivnì s vyu¾itím døíve deklarované
** pomocné funkce ReplaceByRightmost.
**/
	
	tBSTNodePtr help_ptr = NULL;	// pomocny ukazatel

	if ( (*RootPtr) != NULL ){
		if ( (*RootPtr)->Key > K )
			BSTDelete(&((*RootPtr)->LPtr), K); // hledej v levem podstromu
		else if ( (*RootPtr)->Key < K )
			BSTDelete(&((*RootPtr)->RPtr), K); // hledej v pravem podstromu
		else{	// budeme rusit uzel
			help_ptr = *RootPtr; // preulozeni ukazatele
			if ( help_ptr->LPtr == NULL ){	// obsahuje jen pravy podstrom?
				*RootPtr = help_ptr->RPtr;
				free(help_ptr);
			}
			else if ( help_ptr->RPtr == NULL ){	// obsahuje jen levy podstrom?
				*RootPtr = help_ptr->LPtr;
				free(help_ptr);
			}
			else // uzel obsahuje oba podstromy, budeme hledat nejpravejsi prvek
				ReplaceByRightmost(*RootPtr, (&((*RootPtr)->LPtr)));
		}
	}
} 

void BSTDispose (tBSTNodePtr *RootPtr) {	
/*   ----------
** Zru¹í celý binární vyhledávací strom a korektnì uvolní pamì».
**
** Po zru¹ení se bude BVS nacházet ve stejném stavu, jako se nacházel po
** inicializaci. Tuto funkci implementujte rekurzivnì bez deklarování pomocné
** funkce.
**/
	
	if ( *RootPtr != NULL ){
		BSTDispose(&(*RootPtr)->RPtr);	// smer leva
		BSTDispose(&(*RootPtr)->LPtr);	// smer prava		
		free(*RootPtr);	// uvolneni uzlu
		*RootPtr = NULL;
	}
}

/* konec c401.c */

