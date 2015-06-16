/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: key_word_table.c 
*Autor: Tomas Furch, xfurch01
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "key_word_table.h"

extern tHTable* ptrht;

int hashCode ( char* key ) {
	int retval = 0;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += (key[i]);
	return ( retval % KEYWORDTABLESIZE );
} 


void KeyWordTableInit ( tHTable* ptrht ) {
	
 	for(int i = 0; i < KEYWORDTABLESIZE; (*ptrht)[i++] = NULL);
}


/* Hleda v tabulce podle klice */
tHTItem* KeyWordSearch ( tHTable* ptrht, char* key ) {
	tHTItem *tmp;
	
	tmp = (*ptrht)[hashCode(key)]; // do tmp ulozim ukazatel na zacatek seznamu
	if (tmp == NULL) { // pokud je prazdny vracim NULL
		return NULL;
	} else { // jinak prohledavam dokud nenajdu polozku s odpovidajicim klicem
		do {
			if (strcmp(tmp->key, key) == 0) {
				return tmp;
			} else {
				tmp = tmp->ptrnext;
			}
		} while (tmp != NULL);
		return NULL;
	}
}


/* Vklada do tabulky zaznam */
void KeyWordTableInsert ( tHTable* ptrht, char* key) {

 	tHTItem *newItem;
 	tHTItem *tmp;
 	
 	if ((tmp = KeyWordSearch(ptrht, key)) == NULL) {
 		if((newItem = (tHTItem *) malloc(sizeof(tHTItem))) == NULL){
 			exit(1);
 		}
 		newItem->key = key;
 		newItem->ptrnext = (*ptrht)[hashCode(key)];
 		(*ptrht)[hashCode(key)] = newItem;
 	}
}


/* Zrusi tabulku klicovych slov */
void ClearKeyWordTable ( tHTable* ptrht ) {
	tHTItem *tmp; 

	for(int i = 0; i < KEYWORDTABLESIZE; i++){
		tmp = (*ptrht)[i];
 		if (tmp != NULL) {  
			do {
				(*ptrht)[i] = tmp->ptrnext;
				free(tmp);
				tmp = (*ptrht)[i];
			} while (tmp != NULL);
		}  
 	} 
}

/** Naplni tabulku klicovych slov
* find, sort, copy, length vraci hodnoty, jsou to funkce
*/
void fillKeyWordTable ( tHTable* ptrht ) {
	KeyWordTableInsert(ptrht, "begin");
	KeyWordTableInsert(ptrht, "boolean");
	KeyWordTableInsert(ptrht, "do");
	KeyWordTableInsert(ptrht, "else");
	KeyWordTableInsert(ptrht, "end");
	KeyWordTableInsert(ptrht, "false");
	KeyWordTableInsert(ptrht, "forward");
	KeyWordTableInsert(ptrht, "function");
	KeyWordTableInsert(ptrht, "if");
	KeyWordTableInsert(ptrht, "integer");
	KeyWordTableInsert(ptrht, "readln");
	KeyWordTableInsert(ptrht, "real");
	KeyWordTableInsert(ptrht, "string");
	KeyWordTableInsert(ptrht, "then");
	KeyWordTableInsert(ptrht, "true");
	KeyWordTableInsert(ptrht, "var");
	KeyWordTableInsert(ptrht, "while");
	KeyWordTableInsert(ptrht, "write");
}


/** Tiskne tabulku ***/
void htPrintTable( tHTable* ptrht ) {
	int maxlen = 0;
	int sumcnt = 0;
	
	printf ("------------HASH TABLE--------------\n");
	for ( int i=0; i<KEYWORDTABLESIZE; i++ ) {
		printf ("%i:",i);
		int cnt = 0;
		tHTItem* ptr = (*ptrht)[i];
		while ( ptr != NULL ) {
			printf (" (%s)",ptr->key);
			ptr = ptr->ptrnext;
		}
		printf ("\n");
	
		if (cnt > maxlen)
			maxlen = cnt;
		sumcnt+=cnt;
	}
	
	printf ("------------------------------------\n");
	printf ("Items count %i   The longest list %i\n",sumcnt,maxlen);
	printf ("------------------------------------\n");
	return;
}
/***** KONEC SOUBORU keyWordTable.c *****/
