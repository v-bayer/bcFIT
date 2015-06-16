/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: precedence.c 
*Autor: Maros Janota, xjanot01
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "str.h"
#include "syntax.h"
#include "scaner.h"
#include "precedence.h"
#include "ial.h"
#include "semantics.h"

#define SIZE_OF_STACK 32;
#define MAX_SIZE 32;

bool FirstUse = true;
bool ErrorFound = false;
int RuleStackNum = 0;
extern tListOfInstr *currInstrList;
bool ignoreLast = false;
bool bracketReduction = false;

PrecStack PS;
RuleStack RS;

int opType;
void *addr1 = NULL;
void *addr2 = NULL;
void *addr3 = NULL;
extern void *exprResult;

/** Precedencna tabulka */
const T_Token PTable [][14] = {

/********    +      -	   *      /      <      >     <>     <=     >=      =      (      )     id      $   */
/* +  */ { Reduc, Reduc, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* -  */ { Reduc, Reduc, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* *  */ { Reduc, Reduc, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* /  */ { Reduc, Reduc, Reduc, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* <  */ { Shift, Shift, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* >  */ { Shift, Shift, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* <> */ { Shift, Shift, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* <= */ { Shift, Shift, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* >= */ { Shift, Shift, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* =  */ { Shift, Shift, Shift, Shift, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Shift, Reduc, Shift, Reduc },
/* (  */ { Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Push,  Shift, Empty },
/* )  */ { Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Empty, Reduc, Empty, Reduc },
/* id */ { Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Reduc, Empty, Reduc, Empty, Reduc },
/* $  */ { Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Shift, Empty, Shift, Empty },
};

/* Vypisane pravidla v poradi priorit*/
static int rule301[3] = {S_Expr, S_Mul, S_Expr};  
static int rule302[3] = {S_Expr, S_Div, S_Expr};  
static int rule303[3] = {S_Expr, S_Plus, S_Expr};  
static int rule304[3] = {S_Expr, S_Minus, S_Expr};  
static int rule305[3] = {S_Expr, S_Less, S_Expr};  
static int rule306[3] = {S_Expr, S_Greater, S_Expr};  
static int rule307[3] = {S_Expr, S_LessEq, S_Expr};  
static int rule308[3] = {S_Expr, S_GreatEq, S_Expr};  
static int rule309[3] = {S_Expr, S_Equal, S_Expr};  
static int rule310[3] = {S_Expr, S_NotEq, S_Expr};
static int rule311[3] = {S_RightB, S_Expr, S_LeftB};  
static int rule312[3] = {S_ID, 0, 0};

/* Docasna funkcia pre Error */
void Error() {
    error(INTERNAL_ERROR, "precedence analysis error");
}

/* Inicializacia hlavneho zasobnika */
void PrecStackInit(PrecStack *PS){
	PS->First = NULL;
	PS->Last = NULL;
	PS->Previous = NULL;
}

/* Vycistenie zasobnika */
void PrecStackDispose(PrecStack *PS){

	PrecStackElemPtr NewElem = NULL;
	
	while(PS->First != NULL){
		NewElem = PS->First;
		PS->First = PS->First->rptr;
		free(NewElem);
	}
	PS->Last = NULL;	
}

/* Vlozi prvok na koniec zasobnika */
void PrecStackPushLast(PrecStack *PS, TToken val){
	
 	PrecStackElemPtr NewElem = NULL;

	if((NewElem = malloc(sizeof(struct PrecStackElem))) == NULL){
		Error();
	} else {
		NewElem->data = val;
		NewElem->rptr = NULL;
		NewElem->lptr = PS->Last;

		if(PS->First != NULL){
			PS->Last->rptr = NewElem;
		} else {
			PS->First = NewElem;
		}

		PS->Last = NewElem;
	}
}

/* Zmaze a uvolni posledny prvok */
void PrecStackDeleteLast(PrecStack *PS){

	if(PS->First != NULL){
		PrecStackElemPtr NewElem = NULL;
		NewElem = PS->Last;

		if(PS->First == PS->Last){
			PS->First = NULL;
			PS->Last = NULL;
		} else {
			PS->Last = PS->Last->lptr;
			PS->Last->rptr = NULL;
		}
		free(NewElem);
	}
}

/* Vracia hodnotu posledneho prvku na zasobniku */
void PrecStackGetLast(PrecStack *PS, TToken *val){
	*val = PS->Last->data;
}

/* Vracia hodnotu prveho prvku na zasobniku */
void PrecStackGetFirst(PrecStack *PS, TToken *val){
	*val = PS->First->data;
}

/* Vracia hodnotu posledneho predposledneho na zasobniku */
void PrecStackGetPrevious(PrecStack	*PS, TToken *val){
	PS->Previous = PS->Last->lptr;
	*val = PS->Previous->data;
}

/* Vracia hodnotu pred-predposledneho prvku na zasobniku */
void PrecStackGetPreviousOfPrevious(PrecStack *PS, TToken *val){
	PS->PreviousOfPrevious = PS->Previous->lptr;
	*val = PS->PreviousOfPrevious->data;
}

/* Inicializacia pomocneho zasobnika*/
void RuleStackInit(RuleStack *RS){
	RS->First = NULL;
	RS->FirstE = NULL;
	RS->MiddleE = NULL;
	RS->LastE = NULL;
}

/* Vycistenie pomocneho zasobnika */
void RuleStackDispose(RuleStack *RS){

	RuleStackElemPtr NewElem = NULL;

	while(RS->First != NULL){
		NewElem = RS->First;
		RS->First = RS->First->rptr;
		free(NewElem);
	}
	RS->LastE = NULL;	
}

/* Vlozi prvok na koniec pomocneho zasobnika */
void RuleStackPushLast(RuleStack *RS, int val){
	
 	RuleStackElemPtr NewElem = NULL;

	if((NewElem = malloc(sizeof(struct RuleStackElem))) == NULL){
		Error();
	} else {
		NewElem->data = val;
		NewElem->rptr = NULL;
		NewElem->lptr = RS->LastE;

		if(RS->First != NULL){
			RS->LastE->rptr = NewElem;
		} else {
			RS->First = NewElem;
		}

		RS->LastE = NewElem;
	}
}

/* Zmaze a uvolni posledny prvok */
void RuleStackDeleteLast(RuleStack *RS){

	if(RS->First != NULL){
		RuleStackElemPtr NewElem = NULL;
		NewElem = RS->LastE;

		if(RS->First == RS->LastE){
			RS->First = NULL;
			RS->LastE = NULL;
		} else {
			RS->LastE = RS->LastE->lptr;
			RS->LastE->rptr = NULL;
		}
		free(NewElem);
	}
}

/* Vracia hodnotu posledneho prvku pre pravidlo */
void RuleStackGetLast(RuleStack *RS, int *val){
	*val = RS->LastE->data;
}

/* Vracia hodnotu  stredneho prvku pre pravidlo */
void RuleStackGetMiddle(RuleStack *RS, int *val){
	RS->MiddleE = RS->LastE->lptr;
	*val = RS->MiddleE->data;
}

/* Vracia hodnotu prveho prvku pre pravidlo */
void RuleStackGetFirst(RuleStack *RS, int *val){
	RS->FirstE = RS->MiddleE->lptr;
	*val = RS->FirstE->data;
}

/* Ziti ci je posledny prvok Neterminal */
/* return 1 = Terminal */
/* return 0 = NeTerminal*/
int getLastType(PrecStack *PS){

	int type;
	if(getEnum(PS->Last->data) <= S_Dollar){
		type = 1;
	} else {
		type = 0; 
	}
	return type;
}

/* Hlada cislo pravidla */
/* Pri najdeni vracia true */
/* Ak sa nenaslo tak false */
bool findRuleMatch(int *rule1, int *rule2){

	for(int i = 0; i < 3; i++){
		if(rule1[i] != rule2[i]){
			return false;
		}
	}
	return true;
}

/* Pomocou funkcie findRuleMatch zisti a vracia cislo pravidla */
int getRule(int *rule){
	if(findRuleMatch(rule, rule301)) return 301;
	else if(findRuleMatch(rule, rule302)) return 302;
	else if(findRuleMatch(rule, rule303)) return 303;
	else if(findRuleMatch(rule, rule304)) return 304;
	else if(findRuleMatch(rule, rule305)) return 305;
	else if(findRuleMatch(rule, rule306)) return 306;
	else if(findRuleMatch(rule, rule307)) return 307;
	else if(findRuleMatch(rule, rule308)) return 308;
	else if(findRuleMatch(rule, rule309)) return 309;
	else if(findRuleMatch(rule, rule310)) return 310;
	else if(findRuleMatch(rule, rule311)) return 311;
	else if(findRuleMatch(rule, rule312)) return 312;
	else return	-1;
}

/* Potrebne ENUM hodnoty pre spracnu funkcnost precedencnej tabulky */
/* Hodnoty su definovane v poradi pre PTable */
/* Upravy v precendence.h su zakazane !!! */ 
int getEnum(TToken token){

	int expr = token.type;
	if(expr == PLUS) 				return S_Plus;
	if(expr == MINUS) 				return S_Minus;
	if(expr == MULTIPLY) 			return S_Mul;
	if(expr == DIVIDE) 				return S_Div;
	if(expr == LESS) 				return S_Less;
	if(expr == GREATER) 			return S_Greater;
	if(expr == NOT_EQUAL) 			return S_NotEq;
	if(expr == LESS_OR_EQUAL) 		return S_LessEq;
	if(expr == GREATER_OR_EQUAL) 	return S_GreatEq;
	if(expr == EQUAL) 				return S_Equal;
	if(expr == LEFT_BRACKET) 		return S_LeftB;
	if(expr == RIGHT_BRACKET) 		return S_RightB;
	if(expr == ID) 					return S_ID;
	if(expr == INT) 				return S_ID;
	if(expr == REAL) 				return S_ID;
	if(expr == STRING) 				return S_ID;
	if(expr == BOOLEAN) 			return S_ID;
	if(expr == END_OF_EXPRESSION) 	return S_Dollar;
	if(expr == S_Expr) 				return S_Expr;
	else return -1;
}

int getOperatorInstruction(TToken token){

	int op = token.type;
	if(op == PLUS) 					return I_ADD;
	if(op == MINUS) 				return I_DEC;
	if(op == MULTIPLY) 				return I_MUL;
	if(op == DIVIDE) 				return I_DIV;
	if(op == LESS) 					return I_LES;
	if(op == GREATER) 				return I_GTR;
	if(op == NOT_EQUAL) 			return I_NEQ;
	if(op == LESS_OR_EQUAL) 		return I_LEQ;
	if(op == GREATER_OR_EQUAL) 		return I_GEQ;
	if(op == EQUAL) 				return I_EQL;
	else return -1;

}

/* Hlavna funkcia PA */
void PrecedenceFunc(TToken token){

	// Pomocne premenne
 	TToken LastPS;
	bool goOn = false;
	bool reduceDone = false;

	TToken tmpToken;
	PrecStackGetLast(&PS, &LastPS);
	
	if(ignoreLast == false){
		PrecStackGetLast(&PS, &LastPS);
		goOn = true;
	} else {
		PrecStackGetPrevious(&PS, &LastPS);
		if(getEnum(LastPS) <= S_Dollar){
			goOn = true;
		} else {
			Error();
			ErrorFound = true;
		}
	}

	// Najde terminal a pokracuje
	if(goOn == true){
		if(getEnum(LastPS) ==  S_LeftB && getEnum(token) == S_RightB){
				// pravidlo E->(E)
				// Treba postupovat inak pri redukcii tochto pravidla
				bracketReduction = true;
		}
		
		// Zisti dalsi krok pomocou PTable 
		switch(PTable[getEnum(LastPS)][getEnum(token)]){

			case Shift:
				PrecStackPushLast(&PS, token);
				RuleStackPushLast(&RS, getEnum(token));
				RuleStackNum++;
				ignoreLast = false;

				break;			

			case Reduc:
			// Funkcia Reduce, prehodi prvok na zasobnik 
			// Ak je posledny prvok neterminal, je potrebne vlozit shift 
				reduceDone = true;

				int LastE = 0;
				int MiddleE = 0;
				int FirstE = 0;

				int ruleCheck[3] = {0, 0, 0};

				if(RuleStackNum <= 2){
					RuleStackGetLast(&RS, &LastE);
				} 
				else if(RuleStackNum >= 3){
					RuleStackGetLast(&RS, &LastE);
					RuleStackGetMiddle(&RS, &MiddleE);
					RuleStackGetFirst(&RS, &FirstE);
				}

				if(LastE == S_ID){
					ruleCheck[0] = LastE;
					ruleCheck[1] = 0;
					ruleCheck[2] = 0;

				} else {
					ruleCheck[0] = LastE;
					ruleCheck[1] = MiddleE;
					ruleCheck[2] = FirstE;
				}

				// Hladanie spravneho pravidla pre redukciu
				// Ak pravidlo naslo, tak sa vykona redukcia
				if(getRule(ruleCheck) >= 301 && getRule(ruleCheck) <= 311){
					PrecStackGetLast(&PS, &tmpToken);
					// Osetrenie ak bola posledna redukcia pravidla E->(E)
					if(bracketReduction == false){
						// Pomocne premenne
						TToken tmpType;
						TToken tmpAddr1;
						TToken tmpAddr2;

						// Nacitame poslednu premennu zo zasobnika
						// Ulozime ju ako addr2
						// Zmazeme z oboch zasobnikov 
						PrecStackGetLast(&PS, &tmpAddr1);
						PrecStackDeleteLast(&PS);
						addr2 = (void *) STFindG(tmpAddr1);
						RuleStackDeleteLast(&RS);
						RuleStackNum--;	

						// Nacitame poslednu premennu zo zasobnika
						// Musi to byt operator
						// Ulozime ho ako tmpType
						// Zmazeme z oboch zasobnikov 
						PrecStackGetLast(&PS, &tmpType);
						PrecStackDeleteLast(&PS);
						opType = getOperatorInstruction(tmpType);
						RuleStackDeleteLast(&RS);
						RuleStackNum--;	

						// Nacitame poslednu premennu zo zasobnika
						// Ulozime ju ako addr1
						// Zmazeme z oboch zasobnikov
						PrecStackGetLast(&PS, &tmpAddr2);
						PrecStackDeleteLast(&PS);
						addr1 = (void *) STFindG(tmpAddr2);
						RuleStackDeleteLast(&RS);
						RuleStackNum--;	

						TToken newtoken;
						newtoken = SACreateUniqueId();

						// Vytvorime premennu ktora bude reprezentovat vysledok redukcie
						// Vlozime ho na zasobnik
						addr3 = (void *) STInsertLiteral(newtoken);
						PrecStackPushLast(&PS, newtoken);
						generateInstruction(opType, addr1, addr2, addr3);	

						// Vynulovanie pomocnych prem.
						opType = 0;
						addr1 = NULL;
						addr2 = NULL;
						addr3 = NULL;

						RuleStackPushLast(&RS, S_Expr);
						RuleStackNum++;

					} else { // Redukcia pravidla (E)
						TToken tmpMiddle;

						// Zmaze ) z konca zasobnika
						PrecStackDeleteLast(&PS);
						RuleStackDeleteLast(&RS);
						RuleStackNum--;

						// Nacita hodnotu prvku v zatovorke a ulozi do pomocnej premennej
						// Potom ju zmaze
						PrecStackGetLast(&PS, &tmpMiddle);
						PrecStackDeleteLast(&PS);
						RuleStackDeleteLast(&RS);
						RuleStackNum--;

						// Zmaze ( zo zasobnika
						PrecStackDeleteLast(&PS);
						RuleStackDeleteLast(&RS);
						RuleStackNum--;

						// Vlozi ulozenu hodnotu znova na koniec zasobnika 
						// Je to vysledok redukcie E->(E)
						PrecStackPushLast(&PS, tmpMiddle);
						RuleStackPushLast(&RS, S_Expr);
						RuleStackNum++;

						// Nastav ze redukcia pravidla E->(E) uz bola urobena
						bracketReduction = false;
					}
					
				// Naslo pravidlo E -> ID
				} else if(getRule(ruleCheck) == 312){
							PrecStackGetLast(&PS, &tmpToken);
							TToken PSLast;
							PrecStackGetLast(&PS, &PSLast);

					RuleStackDeleteLast(&RS);
					RuleStackPushLast(&RS, S_Expr);
				}

				ignoreLast = true;

				break;

			case Push:

				PrecStackPushLast(&PS, token);
				RuleStackPushLast(&RS, getEnum(token));
				RuleStackNum++;
				ignoreLast = false;

				break;


			case Empty:

				if(getEnum(LastPS) == S_Dollar && getEnum(token) == S_Dollar){
					if(ErrorFound == true){
						dprintf("Chybna Precedencna Analyza\n");
					} else {
						TToken lastAns;
						PrecStackGetLast(&PS, &lastAns);
						exprResult = (void *) STFindG(lastAns);
						dprintf("Precedencna Analyza je OK\n");
					}
					
				} else {
					ErrorFound = true;
				}	
				break;

			default :
				ErrorFound = true;
				break;
		}
	} 

	// Ak prebehla redukcia, tak rekurzivne vola funkciu s rovnakym znakom
	if(reduceDone == true){
		PrecedenceFunc(token);
		reduceDone = false;
	}
}

/* Zaciatocna inicializacia hlavneho a pomocneho zasobnika */
void PrecedenceInit(){
	PrecStackInit(&PS);
	RuleStackInit(&RS);
	PrecStackPushLast(&PS, SACreateEmptyToken(END_OF_EXPRESSION));
}

/* Uvolnenie prvkov po dokonceni PA */
void PrecedenceExit(){
	PrecStackDispose(&PS);
	RuleStackDispose(&RS);
	RuleStackNum = 0;
	ErrorFound = false;
	ignoreLast = false;
}

/* End of precendence.c */