/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: i_list.c 
*Autor: Tomas Furch, xfurch01
*/	


#include <stdio.h>
#include <stdlib.h>
#include "str.h"
#include "syntax.h"

void instrPrint(tInstr instr) {
	printf("I:\t[%d]\t%p\t%p\t%p\n", instr.instType, instr.addr1, instr.addr2, instr.addr3);
}

void listInstrInit(tListOfInstr *L) {

	L->first = NULL;
	L->last = NULL;
	L->active = NULL;
}

void listInstrFree(tListOfInstr *L) {
	tListItem *tmp;
  	if (DEBUGGING) printf("-------------------------------- %p --------------------------------\n", (void *) L);
  	while (L->first != NULL){
  		if (DEBUGGING) instrPrint(L->first->Instruction); // vypis instrukci pro debugovani
    	tmp = L->first;
    	L->first = L->first->nextItem;
    	free(tmp);
  	}
  	if (DEBUGGING) printf("-------------------------------- %p --------------------------------\n\n", (void *) L);
}
	

tListItem *listInstrInsertLast(tListOfInstr *L, tInstr I) {
	tListItem *newItem;
		
	if ((newItem = (tListItem *) malloc(sizeof(tListItem))) == NULL){
		error(INTERNAL_ERROR, "InstrList malloc failed");
		return NULL;
	}
	newItem->Instruction = I; 
	newItem->nextItem = NULL; 
	newItem->previousItem = L->last; 
	if (L->last != NULL){
		L->last->nextItem = newItem; 
	} else {
		L->first = newItem; 
	}
	L->last = newItem;
	return newItem;
}

void listInstrFirst(tListOfInstr *L) {
	L->active = L->first;
}

void listInstrNext(tListOfInstr *L) {
  
	if (L->active != NULL) {
	  	L->active = L->active->nextItem;
	}
}

void listInstrPrevious(tListOfInstr *L) {
	
	if (L->active != NULL) {
		L->active = L->active->previousItem;
	}
}

void listInstrGoto(tListOfInstr *L, void *gotoInstr) {
// nastavime aktivni instrukci podle zadaneho ukazatele
  	L->active = (tListItem*) gotoInstr;
}

void *listInstrGetPointerLast(tListOfInstr *L) {
// vrati ukazatel na posledni instrukci
  	return (void*) L->last;
}

tInstr *listInstrGetData(tListOfInstr *L) {
// vrati aktivni instrukci
  	if (L->active == NULL) {
    	//error(INTERNAL_ERROR, "InstrList: no instruction active");
    	return NULL;
  	} else {
  		return &(L->active->Instruction);
	}
}


void listInstrCopy(tListOfInstr *fromList, tListOfInstr *toList) {
// zkopiruje data jednoho seznamu do druheho
	listInstrFirst(fromList);
	while (fromList->active != NULL) {
		listInstrInsertLast(toList, *listInstrGetData(fromList));
		listInstrNext(fromList);
	}
}

/*****  End of file iList.c  *****/
