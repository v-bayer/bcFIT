
/* c016.c: **********************************************************}
{* Téma:  Tabulka s Rozptýlenými Polo¾kami
**                      První implementace: Petr Pøikryl, prosinec 1994
**                      Do jazyka C prepsal a upravil: Vaclav Topinka, 2005
**                      Úpravy: Karel Masaøík, øíjen 2014
**                      Úpravy: Radek Hranický, øíjen 2014
**
** Vytvoøete abstraktní datový typ
** TRP (Tabulka s Rozptýlenými Polo¾kami = Hash table)
** s explicitnì øetìzenými synonymy. Tabulka je implementována polem
** lineárních seznamù synonym.
**
** Implementujte následující procedury a funkce.
**
**  HTInit ....... inicializuje tabulku pøed prvním pou¾itím
**  HTInsert ..... vlo¾ení prvku
**  HTSearch ..... zji¹tìní pøítomnosti prvku v tabulce
**  HTDelete ..... zru¹ení prvku
**  HTRead ....... pøeètení hodnoty prvku
**  HTClearAll ... zru¹ení obsahu celé tabulky (inicializace tabulky
**                 poté, co ji¾ byla pou¾ita)
**
** Definici typù naleznete v souboru c016.h.
**
** Tabulka je reprezentována datovou strukturou typu tHTable,
** která se skládá z ukazatelù na polo¾ky, je¾ obsahují slo¾ky
** klíèe 'key', obsahu 'data' (pro jednoduchost typu float), a
** ukazatele na dal¹í synonymum 'ptrnext'. Pøi implementaci funkcí
** uva¾ujte maximální rozmìr pole HTSIZE.
**
** U v¹ech procedur vyu¾ívejte rozptylovou funkci hashCode.  Pov¹imnìte si
** zpùsobu pøedávání parametrù a zamyslete se nad tím, zda je mo¾né parametry
** pøedávat jiným zpùsobem (hodnotou/odkazem) a v pøípadì, ¾e jsou obì
** mo¾nosti funkènì pøípustné, jaké jsou výhody èi nevýhody toho èi onoho
** zpùsobu.
**
** V pøíkladech jsou pou¾ity polo¾ky, kde klíèem je øetìzec, ke kterému
** je pøidán obsah - reálné èíslo.
*/

#include "c016.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíè a pøidìlit
** mu index v rozmezí 0..HTSize-1.  V ideálním pøípadì by mìlo dojít
** k rovnomìrnému rozptýlení tìchto klíèù po celé tabulce.  V rámci
** pokusù se mù¾ete zamyslet nad kvalitou této funkce.  (Funkce nebyla
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
** Inicializace tabulky s explicitnì zøetìzenými synonymy.  Tato procedura
** se volá pouze pøed prvním pou¾itím tabulky.
*/

void htInit ( tHTable* ptrht ) {

	if ( ptrht == NULL )
		return;

	for ( int i = 0; i < HTSIZE; i++ ) // ukazatele na indexech od 0 do HTSIZE nadstav na NULL
		(*ptrht)[i] = NULL;
}

/* TRP s explicitnì zøetìzenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíèe key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není, 
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht, tKey key ) {

	if ( ptrht == NULL )
		return NULL;

	int index = hashCode(key); // funkce hashCode vrati index

	tHTItem* item; // vytvoreni pomocne polozky
	item = (*ptrht)[index];

	if ( item == NULL )
		return NULL;

	if ( (strcmp(key, item->key)) == 0 ) // polozka byla nalezena v tabulce
		return (*ptrht)[index];

	while( item->ptrnext != NULL ){
		item = item->ptrnext;
		if ( (strcmp(key, item->key)) == 0 )
			return item;	// vrat index radku na kterem jsem nasli polozku
	}

	return NULL;
}

/* 
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vkládá do tabulky ptrht polo¾ku s klíèem key a s daty
** data.  Proto¾e jde o vyhledávací tabulku, nemù¾e být prvek se stejným
** klíèem ulo¾en v tabulce více ne¾ jedenkrát.  Pokud se vkládá prvek,
** jeho¾ klíè se ji¾ v tabulce nachází, aktualizujte jeho datovou èást.
**
** Vyu¾ijte døíve vytvoøenou funkci htSearch.  Pøi vkládání nového
** prvku do seznamu synonym pou¾ijte co nejefektivnìj¹í zpùsob,
** tedy proveïte.vlo¾ení prvku na zaèátek seznamu.
**/

void htInsert ( tHTable* ptrht, tKey key, tData data ) {

	if ( ptrht == NULL )
		return;

	tHTItem* item = htSearch(ptrht,key);

	if ( item != NULL ){	// v tabulce byla nalezena polozka 
		item->data = data;	// zapis data
		return;
	}

	int index = hashCode(key);	// funkce hashCode vrati index

	item = malloc(sizeof(tHTItem));
	if ( item == NULL )
		return;	// chybna alokace -> ukonci

	item->key = malloc(strlen(key)+1); // alokuj klic
	if ( item->key == NULL ){	
		free(item);	// uvolni polozku
		return;
	}

	item->data = data;
	strcpy(item->key, key);
	item->ptrnext = (*ptrht)[index];
	(*ptrht)[index] = item;
}

/*
** TRP s explicitnì zøetìzenými synonymy.
** Tato funkce zji¹»uje hodnotu datové èásti polo¾ky zadané klíèem.
** Pokud je polo¾ka nalezena, vrací funkce ukazatel na polo¾ku
** Pokud polo¾ka nalezena nebyla, vrací se funkèní hodnota NULL
**
** Vyu¾ijte døíve vytvoøenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht, tKey key ) {

	if ( ptrht == NULL )
		return NULL;


	tHTItem* item = htSearch(ptrht,key); // hledej v tabulce


	if ( item != NULL )	// jestlize najdes polozku v tabulce
		return &(item->data);	// vrat jeji adresu

	return NULL;
}

/*
** TRP s explicitnì zøetìzenými synonymy.
** Tato procedura vyjme polo¾ku s klíèem key z tabulky
** ptrht.  Uvolnìnou polo¾ku korektnì zru¹te.  Pokud polo¾ka s uvedeným
** klíèem neexistuje, dìlejte, jako kdyby se nic nestalo (tj. nedìlejte
** nic).
**
** V tomto pøípadì NEVYU®ÍVEJTE døíve vytvoøenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht, tKey key ) {

	if ( ptrht == NULL )
		return;

	int index = hashCode(key); // funkce hashCode vrati index

	tHTItem* last_item;	// "otec"
	tHTItem* item = (*ptrht)[index];

	if ( item == NULL )
		return;

	if ( (strcmp(key, item->key)) == 0 ){ // klic jiz existuje
		(*ptrht)[index] = item->ptrnext;	// nadstav pointer
		free(item->key);	// uvolni klic
		free(item);	// uvolni polozku
		return;
	}

	while ( item->ptrnext != NULL ){ // dokud nenarazis na NULL
		last_item = item;	// z-aktualizuj hodnoty
		item = item->ptrnext; // skoc na dalci polozku

		if ( (strcmp(key, item->key)) == 0 ){ // nalezena shoda
			last_item->ptrnext = item->ptrnext;	// nadstav pointer
			free(item->key);	// uvolni klic
			free(item);	// uvolni polozku
			return;	// ukonci
		}

	}
}

/* TRP s explicitnì zøetìzenými synonymy.
** Tato procedura zru¹í v¹echny polo¾ky tabulky, korektnì uvolní prostor,
** který tyto polo¾ky zabíraly, a uvede tabulku do poèáteèního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	if ( ptrht == NULL )
		return;

	tHTItem* item;
	tHTItem* help_item;

	for ( int i = 0; i<HTSIZE ; i++ ){	// jed po indexech od 0 po HTSIZE
		item = (*ptrht)[i];

		while( item != NULL ){ // dokud nenarazis na NULL
			help_item = item->ptrnext;
			free(item->key); // uvolni klic
			free(item);	// uvolni polozku
			item = help_item;	// posun se na dalsi polozku
		}
		(*ptrht)[i] = NULL;
	}
}
