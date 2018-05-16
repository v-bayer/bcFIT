/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: interpret.c 
*Autor: Matous Jezersky, xjezer01
*		Vaclav Bayer, xbayer05
*   Maros Janota, xjanot01
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "str.h"
#include "scaner.h"
#include "ial.h"
#include "syntax.h"
#include "interpret.h"
#include "semantics.h"

extern tListOfInstr mainInstrList;
extern tListOfInstr *currInstrList;

extern int errorState;

// po errorech tu mivas return, dam teda tyhle veci globalne a inicializuju je a uklidim po nich v syntax.c
extern FrameList globalFrame;
extern FStack frameStack;

extern tBSTNodePtr globalTable;

// ------------------ zasobnik ------------------

void FSInit(FStack *S) {
  S->top=NULL;
}

int FSEmpty(FStack *S) {
  if (S->top == NULL) return TRUE;
  else return FALSE;
}

void FSPop(FStack *S) {
  FSEptr tmp;
  tmp = S->top;
  if (tmp == NULL) return;
  else {
    FrameListDispose(&S->top->F);
    listInstrFree(&S->top->instrList);
    S->top = tmp->below;
    free(tmp);
    return;
  }
  
}
FrameList *FSTop(FStack *S) {
  if (FSEmpty(S)) {
    error(INTERNAL_ERROR, "empty stack");
    return NULL;
  }
  else return &S->top->F;
}

int FSPush(FStack *S) {
  FSEptr new = malloc(sizeof(struct FStackElem));
  if (new == NULL) {
    error(INTERNAL_ERROR, "empty stack");
    return -1;
  }
  else {
    FrameListInit(&new->F);
    listInstrInit(&new->instrList);
    new->below=S->top;
    S->top=new;
  }
  return 0;
}

void FSDispose(FStack *S) {
  while (!FSEmpty(S)) {
    FSPop(S);
  }
}
// ------------------ konec zasobniku ------------------


// ------------------ ramec (seznam prvků) ------------------
void FrameListInit (FrameList *F) {
  F->Act=NULL;
  F->First=NULL;
  F->Last=NULL;
}

void FrameListDispose (FrameList *F) {
  frameElemPtr element;

  while (F->First!=NULL) {
    element=F->First;
    F->First=element->next;
    //strFree(&element->strdata);
    free(element);
  }
  F->First=NULL;
  F->Act=NULL;
  F->Last=NULL;
}

// funkce pro vlozeni noveho zaznamu do ramce
void FrameListInsert(FrameList *F, char *key, int type, int isLiteral) {
	frameElemPtr new = malloc(sizeof(struct frameElem));
	int sscanfResult = 1; // kontrolni promenna pro return sscanf
	TToken tmpToken; // pomocna promenna pro token pro ulozeni strdata

	if (new == NULL) error(INTERNAL_ERROR, "FrameListInsert: empty stack");
	else {
		new->key = key;
		new->type = type;
		new->intdata = 0;
		new->doudata = 0.0;
		new->strdata = NULL;
		//strInit(&new->strdata); 
    
		new->init = 0; // neni inicializovany, pro literal vlozis hodnotu a nastavis inicializaci az po vytvoreni rucne :)
		new->next = NULL;
		if (F->Last != NULL) F->Last->next=new;
		F->Last = new;
		if (F->First==NULL) F->First = F->Last;
		if (isLiteral) {
			new->init=1; // pracuju s literalem...
			// tady je potreba prevest string na typ literalu!!!!
			if (type == INT) sscanfResult = sscanf(key, "%d", &new->intdata); // OSETRIT PRETECENI!!!
			if (type == REAL) sscanfResult = sscanf(key, "%lf", &new->doudata); // TO SAME!
			if (type == STRING) {
				tmpToken = SACreateToken(key, STRING); // ulozim si novy string, abych mohl odstranit $ a nenarusil key
				new->strdata = strGetStr(&tmpToken.data);
				new->strdata[tmpToken.data.length-1] = '\0'; // odstranim $
			}
			if (type == BOOLEAN) {
				if (key[0]=='t') new->intdata = 1; // >t<rue
				else new->intdata = 0;
			}
			if (sscanfResult == 0 || sscanfResult == EOF) error(RUNTIME_BAD_NUMBER, "string conversion failed");
		}
		if (DEBUGGING) printf("FRAME(%p): inserted %s of type %d / literal:%d\n", (void *) F, key, type, isLiteral);
	}
  
}

void FLSelectFirst (FrameList *F) {
  F->Act=F->First;
}

void FLDeleteFirst (FrameList *F) {
  frameElemPtr nextElement;

  if (F->First!=NULL) {
    if (F->First == F->Last) F->Last = NULL;
    nextElement=F->First->next;
    if (F->Act==F->First) F->Act=NULL;
    //strFree(&F->First->strdata);
    free(F->First);
    F->First=nextElement;
  }
} 

void FLSucc (FrameList *F) {
  if (F->Act!=NULL) {
    F->Act=F->Act->next;
  }
}

int FLActive (FrameList *F) {   
  return (F->Act != NULL ? TRUE : FALSE);
}


/**
* vstupem fce musí být ukazatel na seznam, vetsino &funcTable->params, kde funcTable je zaznam v seznamu lokalnich tabulek ziskany z fce STGetElem
* funkce vrati aktualni vybranehy parametr a posune ukazatel na dalsi parametr
*/
/*frameElemPtr FLGetAndMove(ParamList *F) {
  frameElemPtr active;
  active = F->Act;
  FLSucc(F);
  return active;
}*/

frameElemPtr FLGetElem(FrameList *F, char *key) {
  FLSelectFirst(F);
  while (F->Act != NULL) {
    if (strcmp(F->Act->key, key)==0) return F->Act;
    FLSucc(F);
  }
  return NULL;
}


// hledani podle klice v danem ramci a pak v globalnim ramci
frameElemPtr FLFind(FrameList *F, char *key) {
  frameElemPtr result = NULL;
  if (F != NULL) {
    // pokud je F null a odkazuju z vrcholu zasobniku, zasobnik je prazdny
	result = FLGetElem(F, key);
  }
  // pokud nenajdu v danem ramci, kouknu do globalniho
  if (result == NULL) result = FLGetElem(&globalFrame, key);
  // pokud je ted result NULL, bud jsem dostal spatny ramec, nebo je chyba v syntakticke analyze (nepravdepodobne)
  if (result == NULL) error(INTERNAL_ERROR, "FLFind - key not found");
  return result;
}

// kopirovani dat z tabulky symbolu do ramce
// pouzito v I_NEWFRAME
void STCopyToFrame(tBSTNodePtr fromTable, FrameList *toFrame) {
  if (fromTable != NULL) { 
    FrameListInsert(toFrame, fromTable->key, fromTable->type, fromTable->isLiteral); // kopirovani dat
	// pokud isLiteral, musim provest konverzi ze stringu ve fci FrameListInsert
	STCopyToFrame(fromTable->LPtr, toFrame); // rekurzivne projdu celou tabulku
	STCopyToFrame(fromTable->RPtr, toFrame);
  }
}
// ------------------ konec ramce (seznam prvků) ------------------


int inter()
{

    frameElemPtr frame_addr1;
    frameElemPtr frame_addr2;
    frameElemPtr frame_addr3;
    frameElemPtr frame_addr4; // slouzi pouze jako docasna promenna, nesouvisi s adresou v instrukci
    
    STElemPtr tmpFTab;
    
    string tmpString;
    TToken tmpToken;
    int iter = 0;
    int newCall = 0; // 0/1 jestli prave byla instrukce I_CALL
    int sscanfResult = 1;

    char c;
    
    // nakopiruju data z globalni tabulky do globalniho ramce
    dprintf("---------- COPYING GLOBAL TABLE TO GLOBAL FRAME ----------");
    STCopyToFrame(globalTable, &globalFrame);
    dprintf("-------- DONE COPYING GLOBAL TABLE TO GLOBAL FRAME -------");
    
    
    listInstrFirst(currInstrList);
    tInstr *I = listInstrGetData(currInstrList);
    
    while (I != NULL)
    {
        newCall = 0;
        if (errorState) return 0; // pokud doslo k chybe, nema smysl pokracovat
    if (DEBUGGING) printf("INTERPRETER: I %d %p %p %p\n", I->instType, I->addr1, I->addr2, I->addr3);
        switch (I->instType)
        {
            case I_CONCAT:
            case I_ADD:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr1->type == STRING && frame_addr2->type == STRING){
                	if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "concatenation of uninitialised value");
                	frame_addr3->init = 1;
                	
                    if (frame_addr3->type == TEMP || frame_addr3->type == STRING) frame_addr3->type = STRING;
                
                    strInit(&tmpString);
		            iter = 0;
		            if (frame_addr1->strdata != NULL) {
				        while (frame_addr1->strdata[iter] != '\0') {
				            strAddChar(&tmpString, frame_addr1->strdata[iter]);
				            iter++;
				        }
		            }
		            iter = 0;
		            if (frame_addr2->strdata != NULL) {
				        while (frame_addr2->strdata[iter] != '\0') {
				            strAddChar(&tmpString, frame_addr2->strdata[iter]);
				            iter++;
				        }
		            }
		        	// vytvorim a ulozim vysledek konkatenace do tokenu, tak si zajistim i uklizeni po stringu
		        	tmpToken = SACreateToken(strGetStr(&tmpString), STRING);
		        	strFree(&tmpString);
            		frame_addr3->strdata = strGetStr(&tmpToken.data);
                }
                
                
                else if(frame_addr1->type == REAL && frame_addr2->type == INT) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->doudata + frame_addr2->intdata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == INT && frame_addr2->type == REAL) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->intdata + frame_addr2->doudata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->doudata + frame_addr2->doudata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) {
                    if(frame_addr3->type == INT || frame_addr3->type == TEMP) {
                        frame_addr3->intdata = frame_addr1->intdata + frame_addr2->intdata;
                        frame_addr3->type = INT;
                    }
                    else frame_addr3->doudata = frame_addr1->intdata + frame_addr2->intdata;
                }
                else {
                    return error(SEMANTIC_ERROR_TYPE, "addition not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "addition of uninitialised value");
                	frame_addr3->init = 1;
                
                if (DEBUGGING) printf("ADD (intdata printf): %d + %d -> %d\n", frame_addr1->intdata, frame_addr2->intdata, frame_addr3->intdata);       
            break;


            case I_DEC:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
				
				if(frame_addr1->type == REAL && frame_addr2->type == INT) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->doudata - frame_addr2->intdata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == INT && frame_addr2->type == REAL) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->intdata - frame_addr2->doudata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->doudata - frame_addr2->doudata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) {
                    if(frame_addr3->type == INT || frame_addr3->type == TEMP) {
                        frame_addr3->intdata = frame_addr1->intdata - frame_addr2->intdata;
                        frame_addr3->type = INT;
                    }
                    else frame_addr3->doudata = frame_addr1->intdata - frame_addr2->intdata;
                }
                else {
                    return error(SEMANTIC_ERROR_TYPE, "decrementation not supported with given variable types");
                }
                
				if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in decrementation");
				frame_addr3->init = 1;
				
                if (DEBUGGING) printf("DEC (intdata printf): %d - %d -> %d\n", frame_addr1->intdata, frame_addr2->intdata, frame_addr3->intdata); 
                
                break;


            case I_MUL:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
				
				if(frame_addr1->type == REAL && frame_addr2->type == INT) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->doudata * frame_addr2->intdata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == INT && frame_addr2->type == REAL) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->intdata * frame_addr2->doudata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) {
                    if(frame_addr3->type == REAL || frame_addr3->type == TEMP) {
                        frame_addr3->doudata = frame_addr1->doudata * frame_addr2->doudata;
                        frame_addr3->type = REAL;
                    }
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) {
                    if(frame_addr3->type == INT || frame_addr3->type == TEMP) {
                        frame_addr3->intdata = frame_addr1->intdata * frame_addr2->intdata;
                        frame_addr3->type = INT;
                    }
                    else frame_addr3->doudata = frame_addr1->intdata * frame_addr2->intdata;
                }
                else {
                    return error(SEMANTIC_ERROR_TYPE, "multiplication not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "multiplication of uninitialised value");
                frame_addr3->init = 1;
                
                if (DEBUGGING) printf("MUL (intdata printf): %d * %d -> %d\n", frame_addr1->intdata, frame_addr2->intdata, frame_addr3->intdata); 
                break;


            case I_DIV:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr1->type == BOOLEAN || frame_addr2->type == BOOLEAN || frame_addr1->type == STRING || frame_addr2->type == STRING) {
                	return error(SEMANTIC_ERROR_TYPE, "< operation not supported with given variable types");
                }

                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in division");
                frame_addr3->init = 1;
				
				if ( (frame_addr2->type == REAL && frame_addr2->doudata == 0.0) || (frame_addr2->type == INT && frame_addr2->intdata == 0.0) ) {
					return error(RUNTIME_ZERODIV, "division by zero");
				}
				
				if (frame_addr3->type == TEMP) frame_addr3->type = REAL;
				if (frame_addr3->type != REAL) return error(INTERNAL_ERROR, "interpreter - semantics fault - division result stored in non-real variable");
				
				if(frame_addr1->type == REAL && frame_addr2->type == INT) 		frame_addr3->doudata = frame_addr1->doudata / frame_addr2->intdata;
                else if(frame_addr1->type == INT && frame_addr2->type == REAL) 	frame_addr3->doudata = frame_addr1->intdata / frame_addr2->doudata;
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->doudata = frame_addr1->doudata / frame_addr2->doudata;
                else if(frame_addr1->type == INT && frame_addr2->type == INT) 	frame_addr3->doudata = frame_addr1->intdata / (frame_addr2->intdata + 0.0);
                else {
                    return error(RUNTIME_GENERIC, "division error");
                }
            break;

            
            case I_LES:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
				if (frame_addr3->type == TEMP) frame_addr3->type = BOOLEAN;
				if (frame_addr3->type != BOOLEAN) return error(INTERNAL_ERROR, "interpreter - semantics fault - I_LES result stored in non-real variable");
				
                if(frame_addr1->type == STRING && frame_addr2->type == STRING){
                    if(strcmp(frame_addr1->strdata, frame_addr2->strdata) < 0) frame_addr3->intdata = 1;
                    else frame_addr3->intdata = 0;
                }
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->intdata = (int)(frame_addr1->doudata < frame_addr2->doudata);
                else if(frame_addr1->type == INT && frame_addr2->type == INT) frame_addr3->intdata = (int)(frame_addr1->intdata < frame_addr2->intdata);
                else if(frame_addr1->type == BOOLEAN && frame_addr2->type == BOOLEAN) frame_addr3->intdata = (int)(frame_addr1->intdata < frame_addr2->intdata);
                else{
                    return error(SEMANTIC_ERROR_TYPE, "< operation not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in < comparison");
                frame_addr3->init = 1;
				
            break;


            case I_GTR:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr3->type == TEMP) frame_addr3->type = BOOLEAN;
				if (frame_addr3->type != BOOLEAN) return error(INTERNAL_ERROR, "interpreter - semantics fault - I_GTR result stored in non-real variable");

                if(frame_addr1->type == STRING && frame_addr2->type == STRING){
                    if(strcmp(frame_addr1->strdata, frame_addr2->strdata) > 0)
                        frame_addr3->intdata=1;
                    else frame_addr3->intdata=0;
                }
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->intdata = (int)(frame_addr1->doudata > frame_addr2->doudata);
                else if(frame_addr1->type == INT && frame_addr2->type == INT) frame_addr3->intdata = (int)(frame_addr1->intdata > frame_addr2->intdata);
                else if(frame_addr1->type == BOOLEAN && frame_addr2->type == BOOLEAN) frame_addr3->intdata = (int)(frame_addr1->intdata > frame_addr2->intdata);
                else{
                    return error(SEMANTIC_ERROR_TYPE, "> operation not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in > comparison");
                frame_addr3->init = 1;
                
            break;


            case I_NEQ:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr3->type == TEMP) frame_addr3->type = BOOLEAN;
				if (frame_addr3->type != BOOLEAN) return error(INTERNAL_ERROR, "interpreter - semantics fault - I_NEQ result stored in non-real variable");

                if(frame_addr1->type == STRING && frame_addr2->type == STRING){
                    if(strcmp(frame_addr1->strdata, frame_addr2->strdata) != 0) 
                        frame_addr3->intdata=1;
                    else frame_addr3->intdata=0;
                    frame_addr3->type=BOOLEAN;
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) frame_addr3->intdata = (int)(frame_addr1->intdata != frame_addr2->intdata);
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->intdata = (int)(frame_addr1->doudata != frame_addr2->doudata);
                else if(frame_addr1->type == BOOLEAN && frame_addr2->type == BOOLEAN) frame_addr3->intdata = (int)(frame_addr1->intdata != frame_addr2->intdata);
                else{
                    return error(SEMANTIC_ERROR_TYPE, "<> operation not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in <> comparison");
                frame_addr3->init = 1;
                
            break;


            case I_LEQ:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr3->type == TEMP) frame_addr3->type = BOOLEAN;
				if (frame_addr3->type != BOOLEAN) return error(INTERNAL_ERROR, "interpreter - semantics fault - I_LEQ result stored in non-real variable");

                if(frame_addr1->type == STRING && frame_addr2->type == STRING){
                    if(strcmp(frame_addr1->strdata, frame_addr2->strdata) <= 0)
                        frame_addr3->intdata=1;
                    else frame_addr3->intdata=0;
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) frame_addr3->intdata = (int)(frame_addr1->intdata <= frame_addr2->intdata);
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->intdata = (int)(frame_addr1->doudata <= frame_addr2->doudata);
                else if(frame_addr1->type == BOOLEAN && frame_addr2->type == BOOLEAN) frame_addr3->intdata = (int)(frame_addr1->intdata <= frame_addr2->intdata);
                else{
                    return error(SEMANTIC_ERROR_TYPE, "<= operation not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in <= comparison");
                frame_addr3->init = 1;
                
            break;


            case I_GEQ:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr3->type == TEMP) frame_addr3->type = BOOLEAN;
				if (frame_addr3->type != BOOLEAN) return error(INTERNAL_ERROR, "interpreter - semantics fault - I_GEQ result stored in non-real variable");

                if(frame_addr1->type == STRING && frame_addr2->type == STRING){
                    if(strcmp(frame_addr1->strdata, frame_addr2->strdata) >= 0)
                        frame_addr3->intdata=1;
                    else frame_addr3->intdata=0;
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) frame_addr3->intdata = (int)(frame_addr1->intdata >= frame_addr2->intdata);
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->intdata = (int)(frame_addr1->doudata >= frame_addr2->doudata);
                else if(frame_addr1->type == BOOLEAN && frame_addr2->type == BOOLEAN) frame_addr3->intdata = (int)(frame_addr1->intdata >= frame_addr2->intdata);
                else{
                    return error(SEMANTIC_ERROR_TYPE, ">= operation not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in >= comparison");
                frame_addr3->init = 1;
                
            break;


            case I_EQL:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                frame_addr2 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr2)->key);
                frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                
                if (frame_addr3->type == TEMP) frame_addr3->type = BOOLEAN;
				if (frame_addr3->type != BOOLEAN) return error(INTERNAL_ERROR, "interpreter - semantics fault - I_EQL result stored in non-real variable");

                if(frame_addr1->type == STRING && frame_addr2->type == STRING){
                    if(strcmp(frame_addr1->strdata, frame_addr2->strdata) == 0)
                        frame_addr3->intdata=1;
                    else frame_addr3->intdata=0;
                }
                else if(frame_addr1->type == INT && frame_addr2->type == INT) frame_addr3->intdata = (int)(frame_addr1->intdata == frame_addr2->intdata);
                else if(frame_addr1->type == REAL && frame_addr2->type == REAL) frame_addr3->intdata = (int)(frame_addr1->doudata == frame_addr2->doudata);
                else if(frame_addr1->type == BOOLEAN && frame_addr2->type == BOOLEAN) frame_addr3->intdata = (int)(frame_addr1->intdata == frame_addr2->intdata);
                else{
                	return error(SEMANTIC_ERROR_TYPE, "= operation not supported with given variable types");
                }
                
                if (frame_addr1->init == 0 || frame_addr2->init == 0) return error(RUNTIME_UNINIT, "uninitialised value in = comparison");
                frame_addr3->init = 1;
                
            break;
            
            case I_ASSIGN_PARAM:
            case I_ASSIGN:
                if (I->instType == I_ASSIGN_PARAM) { // assign pro parametry
                    frame_addr1 = FLFind(&frameStack.top->below->F, ((tBSTNodePtr) I->addr1)->key); // novou hodnotu hledam v predchozim ramci + glob.
                    frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key); // parametr uz hledam zase ve fci
                }
                else { // bezny assign
                    frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                    frame_addr3 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr3)->key);
                }

				if (!frame_addr1->init) {
				    if (DEBUGGING) printf("UNINIT: %s T:%d I:%d D:%lf S:%s INIT:%d\n", frame_addr1->key, frame_addr1->type, frame_addr1->intdata, frame_addr1->doudata, frame_addr1->strdata, frame_addr1->init);
				    error(RUNTIME_UNINIT, "assigning uninitialised value");
				    return RUNTIME_UNINIT;
				}

                if(frame_addr1->type == INT && frame_addr3->type == INT)
                    frame_addr3->intdata = frame_addr1->intdata;
                else if(frame_addr1->type == REAL && frame_addr3->type == REAL)
                    frame_addr3->doudata = frame_addr1->doudata;
                else if(frame_addr1->type == STRING && frame_addr3->type == STRING)
                    frame_addr3->strdata = frame_addr1->strdata;
                else if(frame_addr1->type == BOOLEAN && frame_addr3->type == BOOLEAN)
                    frame_addr3->intdata = frame_addr1->intdata;
                else{
                    error(RUNTIME_GENERIC, "Instruction I_ASSIGN failed");
                    return RUNTIME_GENERIC;
                }
                frame_addr3->init = 1; // po prirazeni je inicializovana
                if (DEBUGGING) printf("ASSIGN (intdata printf): %d -> %d\n", frame_addr1->intdata, frame_addr3->intdata);
            break;


            case I_WRITE:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                if (!frame_addr1->init) {
                    if (DEBUGGING) printf("UNINIT: %s T:%d I:%d D:%lf S:%s INIT:%d\n", frame_addr1->key, frame_addr1->type, frame_addr1->intdata, frame_addr1->doudata, frame_addr1->strdata, frame_addr1->init);
                    return error(RUNTIME_UNINIT, "attempt to write uninitalised value");
                }

                if(frame_addr1->type == INT)
                    printf("%d",frame_addr1->intdata);
                else if(frame_addr1->type == REAL)
                    printf("%g",frame_addr1->doudata);
                else if(frame_addr1->type == STRING)
                    printf("%s",frame_addr1->strdata);
                else if(frame_addr1->type == BOOLEAN){
                    if(frame_addr1->intdata == 0)
                        printf("FALSE");
                    if(frame_addr1->intdata == 1)
                        printf("TRUE");
                }
                else{
                    error(INTERNAL_ERROR, "I_WRITE of uknown type");
                    return INTERNAL_ERROR;
                }
            break;


            case I_READLN:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);        
            
                frame_addr1->init = 1;
                strInit(&tmpString);
                
                c = getchar();
                while (c != '\n' && c != EOF && c!='\0'){
                    strAddChar(&tmpString, c);
                    c = getchar();
                }
                
		        // vytvorim a ulozim vysledek readln do tokenu, tak si zajistim i uklizeni po stringu
		        tmpToken = SACreateToken(strGetStr(&tmpString), STRING);
		        strFree(&tmpString);
		        sscanfResult = 1;
		        if (frame_addr1->type == INT) sscanfResult = sscanf(strGetStr(&tmpToken.data), "%d", &frame_addr1->intdata);
		        if (frame_addr1->type == REAL) sscanfResult = sscanf(strGetStr(&tmpToken.data), "%lf", &frame_addr1->doudata);
		        if (frame_addr1->type == STRING) {
		        	strAddChar(&tmpToken.data, '$'); // pridam jeste $ na konec, pro identifikaci string literalu v key
		        	frame_addr1->strdata = strGetStr(&tmpToken.data);
		        	// tmpToken uz nepotrebuju, dam si do nej novy docasny token
		        	tmpToken = SACreateToken(frame_addr1->strdata, STRING); // ulozim si novy string, abych mohl odstranit $ a nenarusil key
					frame_addr1->strdata = strGetStr(&tmpToken.data);
					frame_addr1->strdata[tmpToken.data.length-1] = '\0'; // odstranim $
		        }
		        if (frame_addr1->type == BOOLEAN) {
		        	if (strGetStr(&tmpToken.data)[0]=='t') frame_addr1->intdata = 1; // >t<rue
		        	else frame_addr1->intdata = 0;
		        }
		        if (sscanfResult == 0 || sscanfResult == EOF) error(RUNTIME_BAD_NUMBER, "string conversion failed");
                    
            break;


            case I_JMP:
                currInstrList->active = (tListItem *) I->addr3;
                break;
                
            case I_JMP_ZERO:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                if (!frame_addr1->init) return error(RUNTIME_UNINIT, "conditional jump based on uninitialised value");
                if (frame_addr1->type != BOOLEAN) return error(SEMANTIC_ERROR_TYPE, "non-boolean value in condition");
                if (frame_addr1->intdata == 0) currInstrList->active = (tListItem *) I->addr3;
                             
                break;
                
            case I_JMP_NONZERO:
                frame_addr1 = FLFind(&frameStack.top->F, ((tBSTNodePtr) I->addr1)->key);
                if (!frame_addr1->init) return error(RUNTIME_UNINIT, "conditional jump based on uninitialised value");
                if (frame_addr1->type != BOOLEAN) return error(SEMANTIC_ERROR_TYPE, "non-boolean value in condition");
                else if (frame_addr1->intdata != 0) currInstrList->active = (tListItem *) I->addr3;
                            
                break;


            case I_NEWFRAME: // nahrazuje i CALL
                dprintf("CREATING NEW FRAME\n");
                FSPush(&frameStack); // vytvorim novy prazdny ramec v zasobniku
                tmpFTab = (STElemPtr) I->addr1;
                if (tmpFTab != NULL) {
            STCopyToFrame(tmpFTab->root, &frameStack.top->F); // nakopiruju hodnoty z TS
            listInstrCopy(&tmpFTab->instrList, &frameStack.top->instrList); // nakopiruju instrukce
        }
        break;



            case I_CALL:
                newCall = 1;
                currInstrList = &frameStack.top->instrList;
                listInstrFirst(currInstrList);
                break;
                
        // instrukce vestavenych funkci
        case I_SORT:
			frame_addr1 = FLFind(&frameStack.top->F, "s");
			frame_addr2 = FLFind(&frameStack.top->F, "sort");

			strInit(&tmpString);
			iter = 0;
			if (frame_addr1->strdata != NULL) {
				while (frame_addr1->strdata[iter] != '\0') {
					strAddChar(&tmpString, frame_addr1->strdata[iter]);
					iter++;
				 }
			}
			sort(&tmpString);
			tmpToken = SACreateToken(strGetStr(&tmpString), STRING);
			frame_addr2->strdata = strGetStr(&tmpToken.data);
			frame_addr2->init = 1;

			strFree(&tmpString);
			break;

		case I_LENGTH:

			frame_addr1 = FLFind(&frameStack.top->F, "s");
			frame_addr2 = FLFind(&frameStack.top->F, "length");

			frame_addr2->intdata = length(frame_addr1->strdata);
			frame_addr2->init = 1;
			break;


		case I_COPY:

			frame_addr1 = FLFind(&frameStack.top->F, "s");
			frame_addr2 = FLFind(&frameStack.top->F, "i");
			frame_addr3 = FLFind(&frameStack.top->F, "n");
			frame_addr4  = FLFind(&frameStack.top->F, "copy");

			strInit(&tmpString);

			iter = 0;
			if (frame_addr1->strdata != NULL) {
				while (frame_addr1->strdata[iter] != '\0') {
					strAddChar(&tmpString, frame_addr1->strdata[iter]);
					iter++;
				}
			}
		
			copy(strGetStr(&tmpString), frame_addr2->intdata, frame_addr3->intdata);

			tmpToken = SACreateToken(strGetStr(&tmpString), STRING);
			frame_addr4->strdata = strGetStr(&tmpToken.data);
			frame_addr4->init = 1;
		
			strFree(&tmpString);
			break;

		
		case I_FIND:

			frame_addr1 = FLFind(&frameStack.top->F, "s");
			frame_addr2 = FLFind(&frameStack.top->F, "search");
			frame_addr3 = FLFind(&frameStack.top->F, "find");
			
			frame_addr3->intdata = find(frame_addr1->strdata, frame_addr2->strdata);
			frame_addr3->init = 1;

			break;
            
        
    

        } // konec switche
        


        // prejdeme na dalsi instrukci
        if (newCall) { // pokud jsem volal funkci, jsem na prvni instrukci, nechci ji preskocit
            if (currInstrList == NULL) I = NULL; // pokud ma prazdny instrukcni seznam
            else I = listInstrGetData(currInstrList);
        }
        else {
            listInstrNext(currInstrList);
            I = listInstrGetData(currInstrList);
        }
        if (currInstrList != &mainInstrList && I == NULL){ // pokud jsem byl v instrukcnim seznamu fce a je prazdny...
            dprintf("RETURN");
            FSPop(&frameStack); // zahodim ramec
            if (FSEmpty(&frameStack)) currInstrList = &mainInstrList; // FS je prazdny, zbyva jenom mainInstrList
            else currInstrList = &frameStack.top->instrList; // vratim se do predchoziho instrukcniho seznamu
            listInstrNext(currInstrList);
            I = listInstrGetData(currInstrList);
        }

    }
    return 0;
}
