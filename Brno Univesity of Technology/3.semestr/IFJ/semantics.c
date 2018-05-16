/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: semantics.c 
*Autor: Matous Jezersky, xjezer01
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "syntax.h"
#include "scaner.h"
#include "ial.h"
#include "semantics.h"
#include "key_word_table.h" 

extern TList semList;
extern tBSTNodePtr globalTable;
extern STList localTableList;
extern tListOfInstr *currInstrList;
extern tListOfInstr mainInstrList;
extern void *SAaddr1;
extern void *SAaddr3;
extern int forwardCounter;

STElemPtr currFunction;	// ukazatel na zaznam v seznamu lokalnich tabulek, pokud jsem ve funkci, je neNULLovy, slouzi ke kontrole
						// prirazeni navratove hodnoty... do nazev_fce muzu prirazovat jenom z jejiho tela (navratova hodnota)
						
tBSTNodePtr *currTable; // ukazatel na aktualni tabulku (po semInit currTable=&globalTable)


// ------------------ seznam tokenu ------------------
/*
pouziti seznamu:
nejdriv se inicializuje		listInit(&L);
vlozeni prvku na konec		listInsert(&L, token);
smazani prvniho prvku		listDeleteFirst(&L);
precteni prvniho prvku		listFirst(&L);
pri selhani malloc vraci listInsert hodnotu -1
*/

void listError(int errcode) {
	switch (errcode) {
		case 0:
			error(INTERNAL_ERROR, "Tlist malloc failed\n");
			break;
		case 1:
			error(INTERNAL_ERROR, "Tlist attempt to read empty list\n");
			break;
	}
}
void listInit(TList *L) {
	L->first=NULL;
	L->last=NULL;
}

int listEmpty(TList *L) {
	if (L->first == NULL) return TRUE;
	else return FALSE;
}

int listLast(TList *L) {
	if (L->first == L->last) return TRUE;
	else return FALSE;
}

int listLastElement(TList *L) {
	if ( !listEmpty(L) ) {
		if (L->first == L->last) return TRUE;
	}
	return FALSE;
}

void listDeleteFirst(TList *L) {
	LElemPtr tmp;
	tmp = L->first;
	if (tmp != NULL) {
		L->first = tmp->next;
		if (L->first == NULL) L->last = NULL;
		free(tmp);
	}
	
}
TToken listFirst(TList *L) {
	TToken emptyToken;
	emptyToken.type=0;
	if (listEmpty(L)) {
		listError(1);
		return emptyToken;
	}
	else return L->first->token;
}

//vlozi novy prvek na konec seznamu
int listInsert(TList *L, TToken intoken) {
	LElemPtr new = malloc(sizeof(struct listElem));
	if (new == NULL) {
		listError(0);
		return -1;
	}
	else {
		new->token = intoken;
		new->next = NULL;
		if ( listEmpty(L) ) {
			L->first = new;
			L->last = new;
		}
		else {
			L->last->next = new;
			L->last = new;
		}
	}
	return 0;
}
void listDispose(TList *L) {
	if (L != NULL) {
		while (!listEmpty(L)) {
			listDeleteFirst(L);
		}
	}
}
// ------------------ seznam tokenu ------------------


// ----------------- seznam tabulek symbolu ----------------- 
void STListError() {
    error(INTERNAL_ERROR, "STList malloc failed");
}

void STListInit (STList *L) {
	L->Act=NULL;
	L->First=NULL;
	L->Last=NULL;
}

void STListDispose (STList *L) {
	STElemPtr element;

	while (L->First!=NULL) {
		element=L->First;
		L->First=element->next;
		if (element->root != NULL) BSTDispose(&element->root); // dulezite! seznam musi uklidit strom
		ParamListDispose(&element->params); // i seznam parametru
		listInstrFree(&element->instrList); // i seznam instrukci
		free(element);
	}
	L->First=NULL;
	L->Act=NULL;
	L->Last=NULL;

 
}

STElemPtr STNewTable (STList *L, char *tname, int forward) {
	STElemPtr new = malloc(sizeof(struct STListElem));
	if (new == NULL) STListError();
	else {
		new->name = tname;
		BSTInit(&new->root);
		new->forward = forward;
		ParamListInit(&new->params); // inicializace
		listInstrInit(&new->instrList);
		new->next = NULL;
		if (L->Last != NULL) L->Last->next=new;
		L->Last = new;
		if (L->First==NULL) L->First = L->Last;
		return new;
	}
	return NULL;
}

void STSelectFirst (STList *L) {
	L->Act=L->First;
}

void STSelectLast (STList *L) {
	L->Act=L->Last;
}

void STDeleteFirst (STList *L) {
	STElemPtr nextElement;

	if (L->First!=NULL) {
		if (L->First == L->Last) L->Last = NULL;
		nextElement=L->First->next;
		if (L->Act==L->First) L->Act=NULL;
		if (L->First->root != NULL) BSTDispose(&L->First->root); // dulezite! seznam musi uklidit strom
		ParamListDispose(&L->First->params); // i seznam parametru
		listInstrFree(&L->First->instrList); // i seznam instrukci
		free(L->First);
		L->First=nextElement;
	}
	
}	

void STSucc (STList *L) {
	if (L->Act!=NULL) {
		L->Act=L->Act->next;
	}
}


int STActive (STList *L) {		
	return (L->Act != NULL ? TRUE : FALSE);
}

STElemPtr STGetElem(STList *L, char *tname) {
	STSelectFirst(L);
	while (L->Act != NULL) {
		if (strcmp(L->Act->name, tname)==0) return L->Act;
		STSucc(L);
	}
	return NULL;
}


tBSTNodePtr *STGetTable(STList *L, char *tname) {
	STSelectFirst(L);
	while (L->Act != NULL) {
		if (strcmp(L->Act->name, tname)==0) return &L->Act->root;
		STSucc(L);
	}
	return NULL;
}
// ----------------- seznam tabulek symbolu -----------------


// --------------- kontrolni seznam parametru ---------------
void ParamListError() {
    error(INTERNAL_ERROR, "ParamList malloc failed");
}

void ParamListInit (ParamList *L) {
	L->Act=NULL;
	L->First=NULL;
	L->Last=NULL;
}

void ParamListDispose (ParamList *L) {
	PLElemPtr element;

	while (L->First!=NULL) {
		element=L->First;
		L->First=element->next;
		free(element);
	}
	L->First=NULL;
	L->Act=NULL;
	L->Last=NULL;

 
}

void ParamListInsert (ParamList *L, char *tname, int type) {
	PLElemPtr new = malloc(sizeof(struct ParamListElem));
	if (new == NULL) ParamListError();
	else {
		new->name = tname;
		new->type = type;
		new->next = NULL;
		if (L->Last != NULL) L->Last->next=new;
		L->Last = new;
		if (L->First==NULL) L->First = L->Last;
	}
	
}

void PLSelectFirst (ParamList *L) {
	L->Act=L->First;
}

void PLDeleteFirst (ParamList *L) {
	PLElemPtr nextElement;

	if (L->First!=NULL) {
		if (L->First == L->Last) L->Last = NULL;
		nextElement=L->First->next;
		if (L->Act==L->First) L->Act=NULL;
		free(L->First);
		L->First=nextElement;
	}
	
}	

void PLSucc (ParamList *L) {
	if (L->Act!=NULL) {
		L->Act=L->Act->next;
	}
}


int PLActive (ParamList *L) {		
	return (L->Act != NULL ? TRUE : FALSE);
}


/**
* vstupem fce musí být ukazatel na seznam, vetsino &funcTable->params, kde funcTable je zaznam v seznamu lokalnich tabulek ziskany z fce STGetElem
* funkce vrati aktualni vybranehy parametr a posune ukazatel na dalsi parametr
*/
PLElemPtr PLGetAndMove(ParamList *L) {
	PLElemPtr active;
	active = L->Act;
	PLSucc(L);
	return active;
}

PLElemPtr PLGetElem(ParamList *L, char *tname) {
	PLSelectFirst(L);
	while (L->Act != NULL) {
		if (strcmp(L->Act->name, tname)==0) return L->Act;
		PLSucc(L);
	}
	return NULL;
}
// --------------- kontrolni seznam parametru ---------------










// funkce pro vstup do lokalni tabulky symbolu
void enterLocalTable(char *fname) {
	//tBSTNodePtr found;
	currTable = STGetTable(&localTableList, fname);
	//currTable = &found;
	if (currTable == NULL) dprintf("failed to find table");
	if (currTable == NULL) error(SEMANTIC_ERROR_GENERIC, "variable cannot be called");
}


int semIsFunction(TToken token) {
	if ( STGetTable(&localTableList, strGetStr(&token.data)) == NULL ) return 0;
	else return 1;
}


// funkce na hledani v tabulce symbolu
// vyhleda v ni podle retezce v token.data
tBSTNodePtr STFind(TToken token) {
	tBSTNodePtr currNode = *currTable;
	
	if (currTable == &globalTable) dprintf("searching in global table");
	else dprintf("searching in local table");

	currNode = BSTSearch(*currTable, strGetStr(&token.data));

	return currNode;
}


tBSTNodePtr STFindG(TToken token) {
	tBSTNodePtr currNode = *currTable;
	
	dprintf("searching in local and global table");

	currNode = BSTSearch(*currTable, strGetStr(&token.data)); // hledam v lokalni tabulce
	if (currNode == NULL && currTable != &globalTable ) { // hledam v globalni, pokud jsem v ni uz nebyl
		currNode = BSTSearch(globalTable, strGetStr(&token.data));
	}
	
	return currNode;
}


int semTypeTokenType(TToken token) {
	if (cmpTD(token, "integer")) return INT;
	else if (cmpTD(token, "real")) return REAL;
	else if (cmpTD(token, "string")) return STRING;
	else if (cmpTD(token, "boolean")) return BOOLEAN;
	else {
		return 0;
	}
}


/**
* funkce pro vkladani do tabulky symbolu
* vlozeni promenne	STInsert( varToken, typeToken);
*/
void STInsert(TToken varToken, TToken typeToken) {
	
	BSTInsert (currTable, strGetStr(&varToken.data), semTypeTokenType(typeToken), 0);
	
	if (currTable == &globalTable) {
		dprintf("inserting variable to global table");
	}
	else dprintf("inserting variable to local table");
}


/**
* vlozi literal do tabulky symbolu
* staci jen jeden parametr, protoze obsahuje jak data, tak typ literalu
* vrati zaznam o identifikatoru v tabulce symbolu
*/
tBSTNodePtr STInsertLiteral(TToken token) {
	
	if (currTable == &globalTable) {
		dprintf("inserting literal to global table");
	}
	else dprintf("inserting literal to local table");
	return BSTInsert (currTable, strGetStr(&token.data), token.type, 1);
}


void semInit() {
	listInit(&semList);
	BSTInit(&globalTable);
	currTable = &globalTable;
	currFunction = NULL;
}


void semDeclaredVariable() {
	TToken tmpToken;
	
	// ocekavam 2 tokeny v seznamu... nazev a typ
	tmpToken = listFirst(&semList);
	listDeleteFirst(&semList);
	if ( STFind(tmpToken)!=NULL ) error(SEMANTIC_ERROR_DEF, "identifier already defined");
	else if (semIsFunction(tmpToken)) error(SEMANTIC_ERROR_DEF, "identifier already defined as a function");
	else {
		STInsert( tmpToken, listFirst(&semList));
	}
	listDeleteFirst(&semList);
}

void semDeclaredParameter(STElemPtr funcTable) {
	TToken tmpToken;
	// ocekavam 2 tokeny v seznamu... nazev a typ
	tmpToken = listFirst(&semList);
	listDeleteFirst(&semList);
	if ( STFind(tmpToken)!=NULL ) error(SEMANTIC_ERROR_DEF, "identifier already defined");
	else if (semIsFunction(tmpToken)) error(SEMANTIC_ERROR_DEF, "identifier already defined as a function");
	else {
		STInsert( tmpToken, listFirst(&semList));
		ParamListInsert(&funcTable->params, strGetStr(&tmpToken.data), semTypeTokenType(listFirst(&semList)));
	}
	listDeleteFirst(&semList);
}



void semDeclaredFunction(int isForward) {
	TToken tmpToken;
	tBSTNodePtr funcNode; // zaznam o funkci v globalni tabulce symbolu
	//tBSTNodePtr fwCurrNode;
	//int fwCurrType;
	STElemPtr funcTable; // zaznam v seznamu tabulek
	PLElemPtr currParam; // vybrany parametr v seznamu parametru
		
	tmpToken = listFirst(&semList);
	listDeleteFirst(&semList);
	
	if ( (funcNode = STFind(tmpToken)) !=NULL ) {
		funcTable = STGetElem(&localTableList, funcNode->key);
		if (funcTable == NULL) {
			error(SEMANTIC_ERROR_DEF, "function name already declared as variable");
			return;
		}
		if ( funcTable->forward && !isForward ) {
		forwardCounter--;
		// funkce byla dopredne deklarovana... vlezu do jeji tabulky symbolu a overim shodu parametru, neoveruje se poradi!!!
		// opakovana dopredna deklarace je chyba... proto && !isForward
			funcTable->forward = 0; 					// ted ji nadeklaruju normalne, nastavim forward na false
			enterLocalTable(funcNode->key); 			// vstoupim do jeji tabulky ktera vznikla pri forward dekl.
			currFunction = funcTable; 					// nastavim, ze se nachazim ve funkci
			currInstrList = &funcTable->instrList; 		// a taky nastavim jeji seznam instrukci jako aktualni
			
			PLSelectFirst(&funcTable->params); // nastavim aktivitu seznamu parametru na zacatek
			// dokud neni seznam aktualnich parametru prazdny, nebo pokud jsem nedosel na konec seznamu parametru, provadim kontrolu
			while (!listLast(&semList)) { 
				// tmpToken uz nepotrebuju, pouziju ho tady
				currParam = PLGetAndMove(&funcTable->params); // nactu si ocekavany parametr
				if (currParam == NULL) {
					error(SEMANTIC_ERROR_DEF, "more parameters than in forward declaration");
					break;
				}
				tmpToken = listFirst(&semList);
				listDeleteFirst(&semList);
				if (!cmpTD(tmpToken, currParam->name)) { // porovnam nazev parametru a ocekavany nazev parametru
					error(SEMANTIC_ERROR_DEF, "forward declaration and function declaration parameters do not match");
					break;
				}
				
				tmpToken = listFirst(&semList);
				listDeleteFirst(&semList);
				if (semTypeTokenType(tmpToken) != currParam->type) { // porovnam typ parametru a ocekavany typ parametru
					error(SEMANTIC_ERROR_DEF, "forward declaration and function declaration parameter types do not match");
					break;
				}
			}
			currParam = PLGetAndMove(&funcTable->params); // po docteni by mel byt dalsi parametr NULL, jinak jsme jeste nedocetli -> malo parametru volani
			if (currParam != NULL) error(SEMANTIC_ERROR_DEF, "less parameters than in forward declaration");
			// ted jsem docetl parametry, jeste musim zkontrolovat, jestli se shodujou navratove hodnoty
			tmpToken = listFirst(&semList);
			listDeleteFirst(&semList);
			if (semTypeTokenType(tmpToken) != funcNode->type) error(SEMANTIC_ERROR_DEF, "function type does not match forward declaration type");
		}
		else error(SEMANTIC_ERROR_DEF, "function already declared");
	}
	else { // funkce nebyla deklarovana ani dopredne
		// vytvoreni nove lokalni tabulky a vnoreni do ni
		funcTable = STNewTable(&localTableList, strGetStr(&tmpToken.data), isForward);
		currTable = &funcTable->root; // vstoupim do nove tabulky
		
		while (!listLast(&semList)) {
			semDeclaredParameter(funcTable);
		}
		// funkci musim vkladat do globalni tabulky... tak tam na chvili nakouknu (vyjimka je forward)
		currTable = &globalTable; // navrat do globalni tabulky
		STInsert( tmpToken, listFirst(&semList));
		listDeleteFirst(&semList);
		if ( !isForward ) {
			enterLocalTable(strGetStr(&tmpToken.data)); // pokud se nejedna o doprednou deklaraci, vstoupim do lokalni tabulky
			currFunction = funcTable; // nastavim ji jako aktualni fci
			currInstrList = &funcTable->instrList; // a taky nastavim jeji seznam instrukci jako aktualni
		}
		else forwardCounter++;
	}
}



// pokusi se najit promennou z tokenu v TS, pokud najde, vrati jeji deklarovany typ, jinak 0
int semGetType(TToken token) {
	tBSTNodePtr node;
	tBSTNodePtr *tmpTable; //docasny ukazatel na aktualni tabulku symbolu, kvuli nakouknuti do globalni
	
	if ( (node = STFind(token)) == NULL ) {
		if (currTable != &globalTable) { // pokud neni deklarovana, ale jsem v lokalni tabulce, kouknu jeste do globalni
			tmpTable = currTable;
			currTable = &globalTable;
			
			if ( (node = STFind(token)) == NULL ) {
				error(SEMANTIC_ERROR_DEF, "variable not declared"); // byl jsem v lokalni, nebyla, neni deklarovana ani globalne
				currTable = tmpTable;
				return 0;
			}
			currTable = tmpTable;
		}
		else { 
			error(SEMANTIC_ERROR_DEF, "variable not declared"); // byl jsem v globalni, neni deklarovana
			return 0;
		}
	}
	// kontrola definice, zda byla prirazena hodnota je uz na interpretu
	return node->type;
}



// fce volana ze syntakticke analyzy pri volani fce " result := func ( param ); "
void semCheckFcall() {
	TToken currToken;
	int assignType;
	int returnType;
	tBSTNodePtr *tmpTable; // docasne si ulozim currTable, kouknu do globalTable, enterLocalTable(fnode->key) a zase se vratim, pak jeste u parametru
	tBSTNodePtr tmpNode; // docasna promenna pro uzel / koren
	STElemPtr funcTable; // zaznam v seznamu lokalnich tabulek obsahujici informace o funkci a parametrech
	PLElemPtr currParam; // zaznam v seznamu parametru
	// docasne promenne instrukci
	void *addr1;
	void *addr3;
	
	
	currToken = listFirst(&semList); // token na leve strane prirazeni
	listDeleteFirst(&semList);
	assignType = semGetType(currToken);
	if (assignType == 0) return; // pokud neni promenna deklarovana, ohlasi to fce semGetType
	if ( semIsFunction(currToken) ) {
		if (currFunction == NULL) {
			error(SEMANTIC_ERROR_GENERIC, "cannot assign value to a function unless as a return value");
			return;
		}
		if (!cmpTD(currToken, currFunction->name)) {
		// pokud se jedna o funkci, prirazuju do funkce, pokud se nazev fce do ktere se prirazuje
		// rovna nazvu aktualni funkce, jedna se o nastaveni navratove hodnoty, jinak je to spatne!
			error(SEMANTIC_ERROR_GENERIC, "cannot assign value to a function unless as a return value");
			return;
		}
	}
	
	SAaddr3 = (void *) STFindG(currToken); // leva strana prirazeni, sem se potom ulozi navratova hodnota funkce
	
	dprintf("semCF");
	
	tmpTable = currTable; // zapamatuju si aktualni tabulku
	currTable = &globalTable; // kouknu do globalni tabulky
	currToken = listFirst(&semList);
	listDeleteFirst(&semList);
	if ( (tmpNode = STFind(currToken)) == NULL ) {
		error(SEMANTIC_ERROR_DEF, "function not defined");
		currTable = tmpTable;
		return;
	}
	currTable = tmpTable; // vratim se zpet do ulozene tabulky
	
	returnType=tmpNode->type; // nastavim navratovou hodnotu fce, ta se porovnava na konci
	
	funcTable = STGetElem(&localTableList, tmpNode->key); // ziskam si zaznam s lokalni tabulkou podle nazvu fce
	
	/*if (funcTable->forward) {
		error(SEMANTIC_ERROR_DEF, "function declared, but not defined");
		return;
	}*/
	
	// vytvorim instrukci pro novy ramec z TS volane funkce
	addr1 = (void *) funcTable;
	generateInstruction(I_NEWFRAME, addr1, NULL, NULL);
	
	PLSelectFirst(&funcTable->params); // nastavim aktivni prvek seznamu parametru na zacatek
	while (!listEmpty(&semList)) {
		currToken = listFirst(&semList); // prectu parametr a ulozim do currToken - pracuju s parametrem ve volani!
		listDeleteFirst(&semList);
		currParam = PLGetAndMove(&funcTable->params); // vezmu si ocekavany parametr ze seznamu deklarovanych parametru
		if (currParam == NULL) { // pokud mam parametr z volani, ale dosly ocekavane, volam ji s vice nez bych mel -> chyba
			error(SEMANTIC_ERROR_TYPE, "called function with more parameters then were declared");
			break;
		}
		if (currToken.type == ID) { // pokud je id, musim kouknout do tabulky symbolu a ziskat typ parametru
			if (semGetType(currToken) != currParam->type) { // pokud se typy parametru a ocekavaneho parametru, je to chyba
				error(SEMANTIC_ERROR_TYPE, "function called with different type of parameter then was declared");
				break;
			}
		}
		else {
			if (currToken.type != currParam->type) { // jedna se o konstantu, pokud se neshoduji typy, opet chyba
				error(SEMANTIC_ERROR_TYPE, "function called with different type of parameter then was declared");
				break;
			}	
		}
		// vytvorim instrukci nastaveni parametru (prirazeni)
		addr1 = (void *) STFindG(currToken);
		// parametr musim hledat rucne, mam totiz *char ne TToken, navic potrebuju hledat v TS funkce
		addr3 = (void *) BSTSearch(funcTable->root, currParam->name);
		generateInstruction(I_ASSIGN_PARAM, addr1, NULL, addr3);
				
	}
	// ted pokud si reknu, ze chci dalsi ocekavany parametr, mel by byt NULL, jinak jsem volal fci s mene nez bych mel -> chyba
	if ((currParam = PLGetAndMove(&funcTable->params)) != NULL) error(SEMANTIC_ERROR_TYPE, "called function with less parameters then were declared");

	// pro rozsireni bude na konci END_OF_FCALL narozdil od prazdneho seznamu a semCheckFcall nebude void ale int kvuli navr. typu fce
	
	if (assignType != returnType) error(SEMANTIC_ERROR_TYPE, "type of left side of assignment is not compatible with function return type");
	
	addr1 = (void *) &funcTable->instrList;
	generateInstruction(I_CALL, addr1, NULL, NULL); // volani s instr. seznamem volane funkce
	//addr1 = (void *) &currFunction->instrList;
	//generateInstruction(I_RETURN, addr1, NULL, NULL); // vraceni s instr. seznamem volajici funkce
	SAaddr1 = BSTSearch(globalTable, funcTable->name); // vysledek se priradi z navratove hodnoty funkce
}



int semCheckExpression() {
	TToken currToken;
	currToken = listFirst(&semList);
	listDeleteFirst(&semList);
	int currType = currToken.type; // typ aktualniho tokenu
	int exprType = 0; // typ, ktery dosud vyraz mel
	int cmpOperator = 0; // zda se vyskytl operator porovnani
	
	while (currToken.type != END_OF_EXPRESSION && !listEmpty(&semList)) {
		if (currToken.type == ID) {
			currType = semGetType(currToken);
		}
		else if (currToken.type == DIVIDE) currType = REAL;
		else if (SAIsOperator(currToken)) {
			// kontrola spravnost pouziti operatoru probehne v interpretu
			currType = exprType;
		}
		else currType = currToken.type;
		if (exprType==0) exprType = currType;
		
		if (currType == INT && exprType == REAL) exprType = REAL;
		else if (currType == REAL && exprType == INT) exprType = REAL;
		// pokud se vyskytl operator porovnani, vysledek je boolean, to ale muzu nastavit az pri dalsim id/literalu
		if (cmpOperator) {
			exprType = BOOLEAN;
			cmpOperator = 0;
		}
		switch(currToken.type) { // nastaveni cmpOperator, pokud prijde operator porovnani
			case EQUAL:
			case NOT_EQUAL:
			case LESS:
			case GREATER:
			case GREATER_OR_EQUAL:
			case LESS_OR_EQUAL:
				cmpOperator = 1;
			default:
				break; // jinak nic
		}
		currToken = listFirst(&semList);
		listDeleteFirst(&semList);
	}
	return exprType;	
}

void semCheckAssignment() {
	int leftType;
	int rightType;
	TToken currToken = listFirst(&semList);
	
	listDeleteFirst(&semList);
	leftType = semGetType(currToken); // zjistim jaky ma leva strana typ, o to, jestli je promenna deklarovana se postara fce semGetType 
	if ( semIsFunction(currToken) ) {
		// pokud se jedna o funkci, prirazuju do funkce, pokud se nazev fce do ktere se prirazuje
		// rovna nazvu aktualni funkce, jedna se o nastaveni navratove hodnoty, jinak je to spatne!
		if (currFunction == NULL) {
			error(SEMANTIC_ERROR_DEF, "cannot assign value to a function unless as a return value");
			return;
		}
		if (!cmpTD(currToken, currFunction->name)) {
			error(SEMANTIC_ERROR_DEF, "cannot assign value to a function unless as a return value");
			return;
		}
	}
	//dprintf("semCA: type: %d\n", leftType);
	SAaddr3 = (void *) STFindG(currToken); // ulozim adresu vysledne promenne pro tvorbu instrukci
	rightType = semCheckExpression(); 
	if (leftType != rightType) error(SEMANTIC_ERROR_TYPE, "type of left side of assignment is not compatible with result type of expression");
}


// funkce ktera se vola po dokonceni definice funkce
void semEndOfFunction() {
	currTable = &globalTable;
	currInstrList = &mainInstrList;
	currFunction = NULL;
	dprintf("leaving function table and instruction list\n");
}



// funkce vlozi vestavene funkce do TS a vytvori jejich instrukce
void semInsertBuiltinFunctions() {
	STElemPtr funcTableData;
	tInstr I;
	I.addr1 = NULL;
	I.addr2 = NULL;
	I.addr3 = NULL;	
	
	funcTableData = STNewTable(&localTableList, "length", 0);	// vytvorim jeji lokalni TS
	BSTInsert (&funcTableData->root, "s", STRING, 0);	// vlozim do ni parametry
	ParamListInsert(&funcTableData->params, "s", STRING);
	BSTInsert (&globalTable, "length", INT, 0);				// vlozim fci do globalni TS
	I.instType = I_LENGTH;									// nastavim instrukci na fci
	listInstrInsertLast(&funcTableData->instrList, I);			// vlozim ji do seznamu instr. fce
	
	// analogicky s ostatnima
	funcTableData = STNewTable(&localTableList, "copy", 0);
	BSTInsert (&funcTableData->root, "s", STRING, 0);
	ParamListInsert(&funcTableData->params, "s", STRING);
	BSTInsert (&funcTableData->root, "i", INT, 0);
	ParamListInsert(&funcTableData->params, "i", INT);
	BSTInsert (&funcTableData->root, "n", INT, 0);
	ParamListInsert(&funcTableData->params, "n", INT);
	BSTInsert (&globalTable, "copy", STRING, 0);
	I.instType = I_COPY;
	listInstrInsertLast(&funcTableData->instrList, I);
	
	funcTableData = STNewTable(&localTableList, "find", 0);
	BSTInsert (&funcTableData->root, "s", STRING, 0);
	ParamListInsert(&funcTableData->params, "s", STRING);
	BSTInsert (&funcTableData->root, "search", STRING, 0);
	ParamListInsert(&funcTableData->params, "search", STRING);
	BSTInsert (&globalTable, "find", INT, 0);
	I.instType = I_FIND;
	listInstrInsertLast(&funcTableData->instrList, I);
	
	funcTableData = STNewTable(&localTableList, "sort", 0);
	BSTInsert (&funcTableData->root, "s", STRING, 0);
	ParamListInsert(&funcTableData->params, "s", STRING);
	BSTInsert (&globalTable, "sort", STRING, 0);
	I.instType = I_SORT;
	listInstrInsertLast(&funcTableData->instrList, I);
}

