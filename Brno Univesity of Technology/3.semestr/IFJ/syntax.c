/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: syntax.c
*Autor: Matouš Jezersky, xjezer01
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "str.h"
#include "syntax.h"
#include "scaner.h"
#include "ial.h"
#include "semantics.h"
#include "key_word_table.h"
#include "precedence.h"
#include "interpret.h"

//#include "ial.h"
//#include "semantics.h"

string attr;

tListOfInstr *currInstrList;
tListOfInstr mainInstrList;

CleanupStack *CStack; // zasobnik na uklizeni stringu z tokenu
int errorState; // globalni promenna, ktera uchovava chyby
int lineNum=1;
TList semList; // seznam pro komunikaci se semantickou analyzou
tBSTNodePtr globalTable;
STList localTableList; // seznam lokalnich tabulek symbolu
tHTable KWTable; // tabulka klicovych slov

int forwardCounter; // pocitadlo doprednych deklaraci

// RWCall pro readln: 1, pro write: 2, jinak 0
int RWCall; // volani read nebo write, kvuli tomu, aby se parametry nedavaly na zasobnik pro semantickou analyzu, to se provede az v interpretu
int isAssignment; // promenna, ktera rozlisuje, jestli jsem ve vyrazu napr v podmince ifu nebo while, nebo jestli se jedna o prirazeni
int exprCountRBRACK; // promenna urcijici kolik pravych zavorek jeste musim ocekavat nez ukoncim vyraz
int isStartOfExpression;

TToken SAcurrToken;

int uniqueId; // promenna pro generovani unikatnich identifikatoru (mezivysledky pro vyrazy)

// globalni promenne pro tvorbu instrukci
void *exprResult; // ukazatel na vysledek vyrazu
int SAinstr;
void *SAaddr1;
void *SAaddr2;
void *SAaddr3;


// pro inicializaci interpretu
FrameList globalFrame;
FStack frameStack;


/*
*	Funkce pro hlaseni chyb a nastaveni navratove hodnoty
*	pouziti:	error(ERROR_CODE, "error details");
*
*
*	chyby a chybove kody:
*	
*	1  LEX_ERROR................ chyba v programu v ramci lexikalni analyzy
*	2  SYNTAX_ERROR............. chyba v programu v ramci syntakticke analyzy
*	3  SEMANTIC_ERROR_DEF....... nedefinovana promenna/pokus o redefinici fce/promenne
*	4  SEMANTIC_ERROR_TYPE...... chyba typove kompatibility ve vyrazu/parametru, spatne parametry
*	5  SEMANTIC_ERROR_GENERIC... ostatni semanticke chyby
*	6  RUNTIME_BAD_NUMBER....... behova chyba pri nacitani ciselne hodnoty ze vstupu
*	7  RUNTIME_UNINIT........... behova chyba pri praci s neinicializovanou promennou
*	8  RUNTIME_ZERODIV.......... behova chyba deleni nulou
*	9  RUNTIME_GENERIC.......... ostatni behove chyby
*	99 INTERNAL_ERROR........... interni chyba, napr. selhani mallocu, spatne argumenty/zdrojak
*/


int error(int errcode, char *errmsg) {
	// kdyz doslo k chybe uz driv, neprepisuju ji novou chybou
	if (errorState == 0) {
		errorState = errcode;
		
		switch (errcode) {
			case LEX_ERROR:
				fprintf(stderr, "LEX ERROR");
				fprintf(stderr," at line %d\n%s\n", lineNum, errmsg);
				break;
			case SYNTAX_ERROR:
				fprintf(stderr, "SYNTAX ERROR");
				fprintf(stderr," near line %d\n%s\n", lineNum, errmsg);
				break;
			case SEMANTIC_ERROR_DEF:
			case SEMANTIC_ERROR_TYPE:
			case SEMANTIC_ERROR_GENERIC: 
				fprintf(stderr, "SEMANTIC ERROR");
				if (lineNum != 0) fprintf(stderr," near line %d", lineNum);
				fprintf(stderr, "\n%s\n", errmsg);
				break;
			case RUNTIME_BAD_NUMBER:
			case RUNTIME_UNINIT:
			case RUNTIME_ZERODIV:
			case RUNTIME_GENERIC:
				fprintf(stderr, "RUNTIME ERROR\n%s\n", errmsg);
				break;	
			
			case INTERNAL_ERROR:
				fprintf(stderr, "INTERNAL ERROR\n%s\n", errmsg);
				break;
		}
		
	}
	return errorState; // neni nutne, ale hodi se v interpretu
}


void dprintf(char *text) { // DEBUGGING definovano v syntax.h
	if (DEBUGGING) printf("%s\n", text);
}


tListItem *generateInstruction(int instType, void *addr1, void *addr2, void *addr3)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   return listInstrInsertLast(currInstrList, I);
}


void SAResetInstruction() {
	SAinstr = 0;
	SAaddr1 = NULL;
	SAaddr2 = NULL;
	SAaddr3 = NULL;
	exprResult = NULL;
}


// ------------- uklizeci zasobnik ---------------
// zasobnik slouzi jako uloziste tokenu a tedy i stringu

void CleanupStackInit(CleanupStack *S) {
	S->top=NULL;
}
int CleanupStackEmpty(CleanupStack *S) {
	if (S->top == NULL) return TRUE;
	else return FALSE;
}
void CleanupStackPop(CleanupStack *S) {
	CSEptr tmp;
	tmp = S->top;
	if (tmp != NULL) {
		S->top = tmp->below;
		strFree(&tmp->data.data); // klicova cast zasobiku, tohle je jeho poslani
		free(tmp);
	}	
}
void CleanupStackPush(CleanupStack *S, TToken ptr) {
	CSEptr new = malloc(sizeof(struct CleanupStackElem));
	if (new == NULL) {
		error(INTERNAL_ERROR, "CleanupStack malloc failed");
		return;
	}
	else {
		new->data=ptr;
		new->below=S->top;
		S->top=new;
	}
}
void CleanupStackDispose(CleanupStack *S) {
	while (!CleanupStackEmpty(S)) CleanupStackPop(S);
}
// -------------- uklizeci zasobnik --------------


TToken newToken() {
	int returnCode;
	TToken new;
	strInit(&new.data);
	
	if ((returnCode = getToken(&attr)) != END_OF_FILE) {
	   		if (returnCode == REAL) dprintf(strGetStr(&attr));
	 		if (returnCode == INT) dprintf(strGetStr(&attr));
	 		if (returnCode == SEMICOLON) dprintf(":");
	 		if (returnCode == ID) dprintf(strGetStr(&attr));
	 		if (returnCode == ASSIGN) dprintf(":=");
	 		if (returnCode == LEFT_BRACKET) dprintf("(");
	 		if (returnCode == RIGHT_BRACKET) dprintf(")");
	 		if (returnCode == COMMA) dprintf(",");
	 		if (returnCode == DOT) dprintf(".");
	 		if (returnCode == STRING) dprintf(strGetStr(&attr));
	 		if (returnCode == LEX_ERROR) error(LEX_ERROR, strGetStr(&attr));
	 		
	 		if (strCopyString(&new.data, &attr) == 1) { //akce pri chybe kopirovani stringu
	 			error(INTERNAL_ERROR, "SA: strCopyString failed");
	 		}
	 		/* 
	 		* za kazdy string literal vlozim dolar, abych je v TS odlisil od nazvu promennych
	 		* dolar se odstranuje pri kopirovani hodnoty do ramce v interpretu, nenarusi tak tedy
	 		* zadne operace se stringy
	 		*/
	 		if (returnCode == STRING) {
		 		if (strAddChar(&new.data, '$') == 1) {
					error(INTERNAL_ERROR, "SA: strAddChar failed");
				}
			}
   	}
   	else dprintf("EOF\n");
   	if (returnCode == ID) { // doplneni lexikalniho analyzatoru o BOOLEAN
		if ( cmpTD(new, "false") || cmpTD(new, "true") ) {
			returnCode = BOOLEAN;
			dprintf("! je to BOOLEAN\n");
		}
	}
   	new.type = returnCode;
   	CleanupStackPush(CStack, new); // vlozim take do uklizeciho zasobniku
   	return new;
}

// porovnani token.data a retezce, pokud jsou stejne, vraci true
int cmpTD (TToken token, char *D){
	return !strCmpConstStr(&token.data, D);
}


int SAIsOperator(TToken token) {
	switch(token.type) {
		case PLUS:
		case MINUS:
		case MULTIPLY:
		case DIVIDE:
		case EQUAL:
		case NOT_EQUAL:
		case LESS_OR_EQUAL:
		case LESS:
		case GREATER:
		case GREATER_OR_EQUAL:
			return 1;
		default:
			return 0;
	}
}

int SAIsRelationOperator(TToken token) {
	switch(token.type) {
		case EQUAL:
		case NOT_EQUAL:
		case LESS_OR_EQUAL:
		case LESS:
		case GREATER:
		case GREATER_OR_EQUAL:
			return 1;
		default:
			return 0;
	}
}

// je boolean literal?
int SAIsLiteral(TToken token) {
	switch(token.type) {
		case STRING:
		case REAL:
		case INT:
		case BOOLEAN:
			return 1;
		default:
			return 0;
	}
}


// vytvori novy token typu tokenType bez retezce v token.data
TToken SACreateEmptyToken(int tokenType) { 
	TToken new;
	new.type = tokenType;
	return new;
}


// vytvori novy token i s retezcem a vlozi ho do uklizeciho zasobniku
// muze slouzit jako uloziste retezcu+typu
TToken SACreateToken(char *data, int type) {
	TToken new;
	int i = 0;
   	strInit(&new.data);
   	new.type = type;
   	if (data != NULL) {
   		while (data[i] != '\0') {
   			strAddChar(&new.data, data[i]);
   			i++;
   		}
   	}
   	CleanupStackPush(CStack, new); // vlozim take do uklizeciho zasobniku
   	return new;
}


TToken SACreateUniqueId() {
	TToken tmp;
	char uniquename[13];
	sprintf(uniquename, "0_%d", uniqueId);
	uniqueId++;
	tmp = SACreateToken(uniquename, TEMP);
	return tmp;
}

/*
 rekurzivni sestup, hlavni funkce syntakticke analyzy
 jediny vstupni parametr je pravidlo, ktere je aplikovano na vystup lexikalni analyzy
 
 SA taky vola precedencni analyzu pro vyrazy a semantickou analyzu, kde je potreba
 taky tady probiha vytvareni instrukci s 3AK
*/
void SAProcess(int rule) { // hlavni funkce syntakticke analyzy - rekurzivni sestup

	tListItem *tmpInstr1 = NULL; // pomocne promenne pro jmp/label
	tListItem *tmpInstr2 = NULL; // musi byt lokalni, globalni by byly spatne v zanorenych cyklech
	tListItem *tmpInstr3 = NULL;
	
	if (errorState) return;
	
	else if (rule == LL_PROG) {
		if (SAcurrToken.type != END_OF_FILE) { // pokud je eof, proste uklidi a skonci; pokud neni, pokracuje
			SAProcess(LL_VAR);
			SAProcess(LL_FUNC_SEQ);
			if (forwardCounter != 0) {
				lineNum = 0; // aby se nevypisovalo cislo radku
				error(SEMANTIC_ERROR_DEF, "forward-declared function was not defined");
				return;
			}
			SAProcess(LL_COMP_STAT);
			SAProcess(LL_DOT);
			SAProcess(LL_EOF);
		}
		else error(SYNTAX_ERROR, "empty source file");
	}
	
	
	else if (rule == LL_COMP_STAT) {
		if (cmpTD(SAcurrToken, "begin")) {
			SAcurrToken = newToken();
			SAProcess(LL_STAT_SEQ);
		}
		else {
			error(SYNTAX_ERROR, "expected begin");
		}
	}
	
	
	else if (rule == LL_STAT_SEQ) {
		if (cmpTD(SAcurrToken, "end")) { // begin end ... prazdny compound statement
			SAcurrToken = newToken();
		}
		else {
			SAProcess(LL_STATEMENT);
			SAProcess(LL_SEM_OR_END);
		}
	}
	
	
	else if (rule == LL_SEM_OR_END) {
		if (cmpTD(SAcurrToken, "end")) {
			SAcurrToken = newToken();
		}
		else if ( SAcurrToken.type == SEMICOLON ) {
			SAcurrToken = newToken();
			SAProcess(LL_STATEMENT);
			SAProcess(LL_SEM_OR_END);
		}
		else error(SYNTAX_ERROR, "; or end expected\n");
	}
	
	
	else if (rule == LL_FUNC_SEQ) {
		if ( !cmpTD(SAcurrToken, "begin") ) { // pokud prijde begin, je to v podstate eps, jedine co muze nasledovat je stejne begin
			if ( cmpTD(SAcurrToken, "function") ) { // pokud nenasleduje begin, musi tam byt function, jinak SE
				SAProcess(LL_FUNCTION);
				SAProcess(LL_FUNC_SEQ);
			}
			else error(SYNTAX_ERROR, "expected function or begin\n");
		}
		// else eps
	}
	
	
	else if (rule == LL_FUNCTION) {
		if ( cmpTD(SAcurrToken, "function") ) {
			SAcurrToken = newToken();
			SAProcess(LL_ID);
			SAProcess(LL_LBRACK);
			SAProcess(LL_DECLARE_PARAMS);
			SAProcess(LL_RBRACK);
			SAProcess(LL_COLON);
			SAProcess(LL_FUNC_TYPE);
			SAProcess(LL_SEMICOLON);
			SAProcess(LL_FUNC_BODY);
			SAProcess(LL_SEMICOLON);
			SAProcess(END_OF_FUNCTION);
		}
		else error(SYNTAX_ERROR, "expected function - not supposed to happen?!\n");
	}
	
	
	else if (rule == LL_FUNC_BODY) {
		if ( cmpTD(SAcurrToken, "forward") ) {
			//SAProcess(LL_SEMICOLON);
			semDeclaredFunction(1);
			SAcurrToken = newToken();
		}
		else {
			semDeclaredFunction(0);
			SAProcess(LL_VAR);
			SAProcess(LL_COMP_STAT);		
		}
	}
	
	
	else if (rule == LL_ID) {
		if ( SAcurrToken.type == ID ) {
			// overeni, zda se nejedna o klicove slovo
			if ( KeyWordSearch(&KWTable, strGetStr(&SAcurrToken.data)) != NULL ) {
				error(SYNTAX_ERROR, "expected identifier, got keyword");
				return;
			}
			if (isStartOfExpression) PrecedenceFunc(SAcurrToken); // vlozeni do precedence
			isStartOfExpression = 0;
			if (!RWCall) listInsert(&semList, SAcurrToken); // // vlozeni do semantiky
			if (RWCall == 1) { // pro readln vytvorim instrukci
				SAaddr1 = (void *) STFindG(SAcurrToken);
				if (SAaddr1 == NULL) error(SEMANTIC_ERROR_DEF, "variable not defined");
				else if (semIsFunction(SAcurrToken)) error(SEMANTIC_ERROR_DEF, "variable is defined as a function");
				generateInstruction(I_READLN, SAaddr1, NULL, NULL);
				SAResetInstruction();
			}
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "expected identifier\n");
	}
	
	
	else if (rule == LL_STATEMENT) {
		if ( cmpTD(SAcurrToken, "begin") ) { // vnoreny begin...end
			SAcurrToken = newToken();
			SAProcess(LL_STAT_SEQ);
		}
		else if ( cmpTD(SAcurrToken, "if") ) {
			PrecedenceInit(); // zahajeni precedence - bude nasledovat vyraz
			SAcurrToken = newToken();
			SAProcess(LL_EXPR);
			if (exprResult != NULL) SAaddr1 = exprResult;
			SAProcess(LL_THEN);
			// nachystani instrukce skoku (preskoceni if-then pokud je podminka false)
			if (DEBUGGING )printf("ZERO JMP BASED ON %p\n", exprResult);
			tmpInstr1 = generateInstruction(I_JMP_ZERO, SAaddr1, NULL, NULL); // skok do else
			SAResetInstruction();
			SAProcess(LL_COMP_STAT); // telo ifu
			tmpInstr2 = generateInstruction(I_JMP, NULL, NULL, NULL); // konec then -> skok za else
			tmpInstr3 = generateInstruction(I_LABEL, NULL, NULL, NULL); // label else
			tmpInstr1->Instruction.addr3 = (void *) tmpInstr3; // nastavim at jump v tmpInstr1 at skace do labelu else
			SAProcess(LL_ELSE);
			tmpInstr3 = generateInstruction(I_LABEL, NULL, NULL, NULL); // label za else
			tmpInstr2->Instruction.addr3 = (void *) tmpInstr3; // nastavim at jump v tmpInstr2 at skace do labelu za else
		}
		else if ( cmpTD(SAcurrToken, "while") ) {
			PrecedenceInit();
			SAcurrToken = newToken();
			tmpInstr1 = generateInstruction(I_LABEL, NULL, NULL, NULL); // label pred podminkou while
			SAProcess(LL_EXPR);
			if (exprResult != NULL) SAaddr1 = exprResult;
			SAProcess(LL_DO);
			// nachystani instrukce skoku (preskoceni while pokud je podminka false)
			tmpInstr2 = generateInstruction(I_JMP_ZERO, SAaddr1, NULL, NULL); // skok za do pokud je podminka 0
			SAResetInstruction();
			SAProcess(LL_COMP_STAT);  // telo whilu
			generateInstruction(I_JMP, NULL, NULL, (void *) tmpInstr1); // skocim na zacatek while
			tmpInstr3 = generateInstruction(I_LABEL, NULL, NULL, NULL); // label za do (pokud je podminka 0 skoci se sem)
			tmpInstr2->Instruction.addr3 = (void *) tmpInstr3; // sem bude skakat 
		}
		else if ( cmpTD(SAcurrToken, "readln") ) {
			dprintf("---READLN---\n");
			RWCall = 1; // nastavi se na 0 na prave zavorce
			SAcurrToken = newToken();
			SAProcess(LL_LBRACK);
			SAProcess(LL_ID);
			SAProcess(LL_RBRACK);
		}
		else if ( cmpTD(SAcurrToken, "write") ) {
			dprintf("---WRITE---\n");
			RWCall = 2; // nastavi se na 0 na prave zavorce
			SAcurrToken = newToken();
			SAProcess(LL_LBRACK);
			SAProcess(LL_PARAMS);
			SAProcess(LL_RBRACK);
		}
		
		
		else if ( SAcurrToken.type == ID ){
			if ( cmpTD(SAcurrToken, "end") ) error(SYNTAX_ERROR, "statement expected");
			else {
				SAProcess(LL_ID);
				SAProcess(LL_ASSIGN);
				// nastavim instrukci na prirazeni
				// tohle je 1.krok, zbytek se provede az po semanticke analyze
				SAProcess(LL_EXPR_OR_FCALL);
				// v tento moment uz mam vsechno co potrebuju na assign
				generateInstruction(I_ASSIGN, SAaddr1, SAaddr2, SAaddr3);
				SAResetInstruction();
			}
		}
		else error(SYNTAX_ERROR, "invalid statement");
	}
	
	
	else if (rule == LL_EXPR_OR_FCALL) {
		if ( SAIsLiteral(SAcurrToken) ) {
			// zahajuju vyraz... musim vlozit literal na stack semantiky (postara se o to LL_EXPR)
			PrecedenceInit();
			SAProcess(LL_EXPR);
		}
		else if ( SAcurrToken.type == LEFT_BRACKET ) {
			PrecedenceInit();
			SAProcess(LL_EXPR);
		}
		else if ( SAcurrToken.type == ID ) { // volani funkce nebo vyraz
			PrecedenceInit();
			isStartOfExpression = 1; // necham zpracovani ID na LL_ID, ale reknu mu, ze se muze jednat o zahajeni vyrazu (pri volani fce se zahodi)
			SAProcess(LL_ID);
			SAProcess(LL_OP_OR_FCALL);
		}
		else error(SYNTAX_ERROR, "expected expression or function call\n");
	}
	
	
	else if (rule == LL_OP_OR_FCALL) {
		if ( SAcurrToken.type == LEFT_BRACKET ) { // volani funkce
			PrecedenceExit(); // predchozi bylo ID, ted ho zahodim, neni to vyraz
			SAcurrToken = newToken();
			SAProcess(LL_PARAMS);
			SAProcess(LL_RBRACK);
			SAProcess(GENERATE_END_OF_FCALL); // pro semantiku -  tam probehne taky generovani instrukci pro parametry
		}
		else SAProcess(LL_OPERATOR); // bude to operator... pokracuju
	}
	
	
	else if (rule == LL_ELSE) {
		if ( cmpTD(SAcurrToken, "else") ) {
			SAcurrToken = newToken();
			SAProcess(LL_COMP_STAT);
		}
		// else eps
	}
		
	
	// pravidlo pro vyrazy
	else if (rule == LL_EXPR) {
		if ( SAcurrToken.type == REAL || SAcurrToken.type == INT || SAcurrToken.type == STRING || SAcurrToken.type == BOOLEAN) {
			listInsert(&semList, SAcurrToken); // vlozeni do semantiky
			STInsertLiteral(SAcurrToken); // literal vlozim do tabulky symbolu
			PrecedenceFunc(SAcurrToken); // // vlozeni do precedence
			SAcurrToken = newToken();
			SAProcess(LL_OPERATOR);
		}
		else if ( SAcurrToken.type == ID ) { // je to identifikator, pro rozsireni muze prijit ( <params> )
			listInsert(&semList, SAcurrToken); // SEM_TEST
			PrecedenceFunc(SAcurrToken); // PRECEDENCE
			if ( KeyWordSearch(&KWTable, strGetStr(&SAcurrToken.data)) != NULL ) { // kontrola klicovych slov
				error(SYNTAX_ERROR, "expected identifier, got keyword");
				return;
			}
			SAcurrToken = newToken();
			SAProcess(LL_OPERATOR);
		}
		else if ( SAcurrToken.type == LEFT_BRACKET ) {
			PrecedenceFunc(SAcurrToken); // PRECEDENCE
			exprCountRBRACK++; // inkrementuju ocekavany pocet pravych zavorek
			dprintf("++\n");
			SAcurrToken = newToken();
			SAProcess(LL_EXPR);
			SAProcess(LL_EXPR_RBRACK);
			SAProcess(LL_OPERATOR);
		}
		else {
			error(SYNTAX_ERROR, "incorrect expression\n");
			exprCountRBRACK=0;
		}
	
	}
	
	// zpracovani operatoru
	else if (rule == LL_OPERATOR) {
		if ( SAIsOperator(SAcurrToken) ) {
			listInsert(&semList, SAcurrToken); // operatory taky vkladam do semantiky
			PrecedenceFunc(SAcurrToken); // a do precedence
			SAcurrToken = newToken();
			SAProcess(LL_EXPR);
		}
		
		else if (SAcurrToken.type != RIGHT_BRACKET) { // eps, konec vyrazu, pokud toto neni jeste prava zavorka
			listInsert(&semList, SACreateEmptyToken(END_OF_EXPRESSION));
			if ( !exprCountRBRACK ) { // pokud jeste zbyvaji nejake zavorky, nemuzu poslat ukonceni vyrazu, pokud nezbyvaji, je to OK
				// ukonceni precedence
				PrecedenceFunc(SACreateEmptyToken(END_OF_EXPRESSION));
				PrecedenceExit();
				SAaddr1 = exprResult; // vysledek z vyrazu bude prvni adresa v prirazeni / podmince
				dprintf("EOE\n");
				// semanticka kontrola
				if (isAssignment) { // jedna se vyraz v prirazeni
					semCheckAssignment();
					isAssignment = 0; // opet vratim na false
				}
				else semCheckExpression(); // jedna se pouze o vyraz
			}
		}
	}
	
	
	else if (rule == LL_VAR) {
		if ( cmpTD(SAcurrToken, "var") ) {
			SAcurrToken = newToken();
			SAProcess(LL_DECLARATION);
			SAProcess(LL_DECL_SEQ);
		}
		// else eps
	}
	
	
	else if (rule == LL_DECL_SEQ) {
		if ( !(cmpTD(SAcurrToken, "begin") || cmpTD(SAcurrToken, "function")) ) { // begin nebo function znamena v podstate eps
			SAProcess(LL_DECLARATION);
			SAProcess(LL_DECL_SEQ);
		}
		// else eps
	}
	
	
	else if (rule == LL_DECLARATION) {
		SAProcess(LL_ID);
		SAProcess(LL_COLON);
		SAProcess(LL_TYPE);
		SAProcess(LL_SEMICOLON);
	}
	
	
	// typ u deklarace promenne
	else if (rule == LL_TYPE) {
		listInsert(&semList, SAcurrToken); 	// vlozim do semantiky
		semDeclaredVariable(); 				// spustim semantickou akci
		if ( cmpTD(SAcurrToken, "integer") ) {
			dprintf("SA: integer declared\n");
			SAcurrToken = newToken();
		}
		else if ( cmpTD(SAcurrToken, "real")) {
			dprintf("SA: real declared\n");
			SAcurrToken = newToken();
		}
		else if ( cmpTD(SAcurrToken, "string")) {
			dprintf("SA: string declared\n");
			SAcurrToken = newToken();
		}
		else if ( cmpTD(SAcurrToken, "boolean")) {
			dprintf("SA: boolean declared\n");
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "invalid type\n");
	}
	
	
	// typ u deklarace funkce
	else if (rule == LL_FUNC_TYPE) {
		listInsert(&semList, SAcurrToken);
		// jenom vlozim do sem. seznamu
		if ( cmpTD(SAcurrToken, "integer") ) {
			dprintf("SA: integer declared\n");
			SAcurrToken = newToken();
		}
		else if ( cmpTD(SAcurrToken, "real")) {
			dprintf("SA: real declared\n");
			SAcurrToken = newToken();
		}
		else if ( cmpTD(SAcurrToken, "string")) {
			dprintf("SA: string declared\n");
			SAcurrToken = newToken();
		}
		else if ( cmpTD(SAcurrToken, "boolean")) {
			dprintf("SA: boolean declared\n");
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "invalid type\n");
	}
	
	
	else if (rule == LL_PARAMS) {
		if ( SAcurrToken.type != RIGHT_BRACKET ) { // () je eps
			SAProcess(LL_ID_OR_LITERAL);
			SAProcess(LL_PARAM_OR_EPS);
		}
		// else eps
	}
	
	
	// bez eps po carce
	else if (rule == LL_ID_OR_LITERAL) {
		if ( SAIsLiteral(SAcurrToken) ) { // je to literal
			STInsertLiteral(SAcurrToken); // kazdou konstantu vkladam do tabulky symbolu
			if (!RWCall) listInsert(&semList, SAcurrToken); // vlozim literal do seznamu semantiky (literalni parametr), pokud nevolam readln/write
			if (RWCall == 2) { // pro write vytvorim instrukci
				SAaddr1 = (void *) STFindG(SAcurrToken);
				if (SAaddr1 == NULL) error(SEMANTIC_ERROR_DEF, "variable not defined");
				generateInstruction(I_WRITE, SAaddr1, NULL, NULL);
				SAResetInstruction();
			}
			
			SAcurrToken = newToken();
		}
		else if ( SAcurrToken.type == ID ) { // musim zjistit, jestli to neni klicove slovo, a pak zjistit deklaraci
			if ( KeyWordSearch(&KWTable, strGetStr(&SAcurrToken.data)) != NULL ) {
				error(SYNTAX_ERROR, "expected identifier, got keyword");
				return;
			}
			if (!RWCall) listInsert(&semList, SAcurrToken); // vlozeni do semantiky, pokud se nejedna o read/write
			// jenom vlozim do seznamu, semantika jej zpracuje jako parametr
			
			if (RWCall == 2) { // pro write vytvorim instrukci
				SAaddr1 = (void *) STFindG(SAcurrToken);
				if (SAaddr1 == NULL) error(SEMANTIC_ERROR_DEF, "variable not defined");
				else if (semIsFunction(SAcurrToken)) error(SEMANTIC_ERROR_DEF, "variable is defined as a function");
				generateInstruction(I_WRITE, SAaddr1, NULL, NULL);
				SAResetInstruction();
			}
			
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "expected identifier or literal\n");
	}
	
	
	else if (rule == LL_PARAM_OR_EPS) {
		if ( SAcurrToken.type == COMMA ) {
			SAcurrToken = newToken();
			SAProcess(LL_ID_OR_LITERAL);
			SAProcess(LL_PARAM_OR_EPS);
		}
		// else eps
	}
	
	
	else if (rule == LL_DECLARE_PARAMS) {
		if (SAcurrToken.type != RIGHT_BRACKET) {
			SAProcess(LL_ID);
			SAProcess(LL_COLON);
			SAProcess(LL_FUNC_TYPE);
			SAProcess(LL_DECLARE_PARAM_OR_EPS);
		}
		// else eps
	}
	
	else if (rule == LL_DECLARE_PARAM_OR_EPS) {
		if ( SAcurrToken.type == SEMICOLON ) {
			SAcurrToken = newToken();
			SAProcess(LL_ID);
			SAProcess(LL_COLON);
			SAProcess(LL_FUNC_TYPE);
			SAProcess(LL_DECLARE_PARAM_OR_EPS);
		}
		// else eps
	}
	
	else if (rule == LL_BEGIN) {
		if ( cmpTD(SAcurrToken, "begin") ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected begin\n");
	}
	
	else if (rule == LL_THEN) {
		if ( cmpTD(SAcurrToken, "then") ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected then\n");
	}
	
	
	else if (rule == LL_DO) {
		if ( cmpTD(SAcurrToken, "do") ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected do\n");
	}
	
	
	else if (rule == LL_COLON) {
		if ( SAcurrToken.type == COLON ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected :\n");
	}
	
	
	else if (rule == LL_SEMICOLON) {
		if ( SAcurrToken.type == SEMICOLON ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected ;\n");
	}
	
	
	else if (rule == LL_LBRACK) {
		if ( SAcurrToken.type == LEFT_BRACKET ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected (\n");
	}
	
	
	else if (rule == LL_RBRACK) { // tahle zavorka muze byt treba u volani fce apod., u vyrazu NE
		if ( SAcurrToken.type == RIGHT_BRACKET ) {
			RWCall = 0; // tady je jistota, ze konci RWCall, kdyz se bude u kazde zavorky nastavovat nula, nicemu to neuskodi
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "expected )\n");
	}
	
	
	else if (rule == LL_EXPR_RBRACK) { // prava zavorka vyrazu
		if ( SAcurrToken.type == RIGHT_BRACKET ) {
			PrecedenceFunc(SAcurrToken);
			if ( exprCountRBRACK == 0 ) { // hotovo, konec vyrazu
				// PRECEDENCE
				PrecedenceFunc(SACreateEmptyToken(END_OF_EXPRESSION));
				PrecedenceExit();
				SAaddr1 = exprResult; // vysledek z vyrazu bude prvni adresa v prirazeni / podmince
				// semanticka kontrola
				if (isAssignment) { // jedna se vyraz v prirazeni
					semCheckAssignment();
					isAssignment = 0; // opet vratim na false
				}
				else semCheckExpression(); // jedna se pouze o vyraz
				dprintf("rbEOE\n");
			}
			exprCountRBRACK--; // dekrementuju pocet ocekavanych pravych zavorek
			dprintf("--\n");
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "expected )\n");
	}
	
	
	else if (rule == LL_DOT) {
		if ( SAcurrToken.type == DOT ) SAcurrToken = newToken();
		else error(SYNTAX_ERROR, "expected .\n");
	}
	
	
	else if (rule == LL_ASSIGN) {
		if ( SAcurrToken.type == ASSIGN ) {
			isAssignment = 1; // nastavim, ze se bude jednat o prirazeni, pri dalsim END_OF_EXPRESSION nebo END_OF_FCALL to odstranim
			SAcurrToken = newToken();
		}
		else error(SYNTAX_ERROR, "expected :=\n");
	}
	
	
	else if (rule == GENERATE_END_OF_FCALL) {
		isAssignment = 0; // nejedna se o prirazeni ale o volani fce
		// vlozit do seznamu pro semantiku SAGenerateEmptyToken(END_OF_FCALL) - rozsireni
		semCheckFcall(); // zahajim semantickou akci - kontrola volani funkce
	}
	
	
	else if (rule == END_OF_FUNCTION) {
		// konec tela funkce, vynoreni z lokalni tabulky
		dprintf("end_of_function - zpet do globalTable\n");
		semEndOfFunction();
	}

	
	else if (rule == LL_EOF) {
		if ( SAcurrToken.type == END_OF_FILE ) {
			
			dprintf("syntax+semantics OK\n");
		}
		else error(SYNTAX_ERROR, "expected EOF\n");
	}
	
	
	else error(SYNTAX_ERROR, "generic\n");

}


int main(int argc, char** argv){
	FILE *sourceFile;
	CleanupStack CS;
	
	listInstrInit(&mainInstrList);
	currInstrList = &mainInstrList;
	
	RWCall = 0;
	isAssignment = 0;
	exprCountRBRACK = 0;
	errorState = 0;
	isStartOfExpression = 0;
	
	uniqueId = 0;

	if (argc == 1) {
      	error(INTERNAL_ERROR, "input file not specified");
      	return INTERNAL_ERROR;
   	}
   	else if (argc > 2) {
   		error(INTERNAL_ERROR, "invalid input arguments, only file path expected");
      	return INTERNAL_ERROR;
   	}
   	if ((sourceFile = fopen(argv[1], "r")) == NULL) {
      	error(INTERNAL_ERROR, "failed to open input file");
      	return INTERNAL_ERROR;
   	}
   	
   	strInit(&attr);
   	setSourceFile(sourceFile);
   	
   	CleanupStackInit(&CS);
   	CStack = &CS;
   	
   	KeyWordTableInit(&KWTable); // inicializace tabulky klicovych slov
   	fillKeyWordTable(&KWTable);
   	// inicializace semantickeho analyzatoru
   	STListInit(&localTableList);
   	semInit(); // SEM_TEST
   	SAResetInstruction();
   	
   	SAcurrToken = newToken(); // pocatecni stav zasobniku obsahuje <PROG>
   	dprintf("init\n");
   	semInsertBuiltinFunctions();
   	SAProcess(LL_PROG); // spusteni syntakticke analyzy
   	lineNum = 0; // nedojde k vypsani radku u sem chyby detekovane v interpretace
	if (!errorState) {
		// inicializace interpretu
		FrameListInit(&globalFrame);
		FSInit(&frameStack);
	   	// spusteni interpretace
		inter();
		// uklizeni po interpretu
		FrameListDispose(&globalFrame);
		FSDispose(&frameStack);
	}
   	
   	listInstrFree(&mainInstrList);
   	strFree(&attr);
	fclose(sourceFile);
   	listDispose(&semList); // SEM_TEST
   	BSTDispose(&globalTable); // SEM_TEST
   	STListDispose(&localTableList);
   	ClearKeyWordTable(&KWTable);
   	CleanupStackDispose(CStack);
   	return errorState;
}

