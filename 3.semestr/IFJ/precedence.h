/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: precedence.h 
*Autor: Maros Janota, xjanot01
*/

/* Konstanty potrebne pre precedencnu analyzu */
/* Zakazane upravovat !!! Poskodi to Preced.Tabulku */
/* Nemenit ani poradie.	*/
typedef enum 
{
	S_Plus,			// +   0
	S_Minus,		// -   1
	S_Mul,			// *   2
	S_Div,			// /   3 
	S_Less,			// <   4
	S_Greater, 		// >   5
	S_NotEq,		// <>  6 
	S_LessEq,		// <=  7
	S_GreatEq,		// >=  8
	S_Equal,		// =   9
	S_LeftB,		// (   10
	S_RightB,		// )   11
	S_ID, 			// id  12 
	S_Dollar,		// $   13

	S_Expr,  		// E   14

	Reduc,			// <   15
	Shift,			// >   16
	Empty,			// X   17
	Push,			// =   18

	S_X,

}T_Token;

/* Zasobniky pre PA */
/* Pomocny zasobnik */
typedef struct RuleStackElem {                 
        int data;                                           
        struct RuleStackElem *lptr;          
        struct RuleStackElem *rptr;       
} *RuleStackElemPtr;

typedef struct {                               
    RuleStackElemPtr First;
    RuleStackElemPtr FirstE;
    RuleStackElemPtr MiddleE;                   
    RuleStackElemPtr LastE;
} RuleStack;

/* Hlavny zasobnik */
typedef struct PrecStackElem {                 
        TToken data;                                           
        struct PrecStackElem *lptr;          
        struct PrecStackElem *rptr;       
} *PrecStackElemPtr;

typedef struct {                               
    PrecStackElemPtr First; 
    PrecStackElemPtr PreviousOfPrevious;  
    PrecStackElemPtr Previous;  
    PrecStackElemPtr Last;                   
} PrecStack;

/* Prototypy funkcii */
void PrecStackInit(PrecStack *PS);
void PrecStackDispose(PrecStack *PS);
void PrecStackPushLast(PrecStack *PS, TToken val);
void PrecStackDeleteLast(PrecStack *PS);
void PrecStackGetLast(PrecStack *PS, TToken *val);
void PrecStackGetFirst(PrecStack *PS, TToken *val);
void PrecStackGetPrevious(PrecStack	*PS, TToken *val);
void RuleStackInit(RuleStack *RS);
void RuleStackDispose(RuleStack *RS);
void RuleStackPushLast(RuleStack *RS, int val);
void RuleStackDeleteLast(RuleStack *RS);
void RuleStackGetLast(RuleStack *RS, int *val);
void RuleStackGetMiddle(RuleStack *RS, int *val);
void RuleStackGetFirst(RuleStack *RS, int *val);
int getLastType(PrecStack *PS);
bool findRuleMatch(int *rule1, int *rule2);
int getRule(int *rule);
int getEnum(TToken token);
void PrecedenceFunc(TToken token);
void PrecedenceInit();
void PrecedenceExit();
