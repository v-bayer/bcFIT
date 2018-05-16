/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: semantics.h 
*Autor: Matous Jezersky, xjezer01
*/


#define SEMANTIC_ERROR_DEF 3
#define SEMANTIC_ERROR_TYPE 4
#define SEMANTIC_ERROR_GENERIC 5


typedef struct listElem {
	TToken token;
	struct listElem *next;
} *LElemPtr;


typedef struct  {
	LElemPtr first;
	LElemPtr last;
} TList;

void listError(int errcode);
void listInit(TList *L);
int listEmpty(TList *L);
int listLast(TList *L);
int listLastElement(TList *L);
void listDeleteFirst(TList *L);
TToken listFirst(TList *L);
int listInsert(TList *L, TToken intoken);
void listDispose(TList *L);




// --------------- kontrolni seznam parametru ---------------
typedef struct ParamListElem {
	char *name;
	int type;
	struct ParamListElem *next;
} *PLElemPtr;


typedef struct  {
	PLElemPtr First;
	PLElemPtr Act;
	PLElemPtr Last;
} ParamList;

void ParamListError();
void ParamListInit(ParamList *L);
void ParamListInsert(ParamList *L, char *name, int type);
void PLSelectFirst (ParamList *L);
void PLSucc (ParamList *L);
int PLActive (ParamList *L);
void ParamListDispose(ParamList *L);
PLElemPtr PLGetElem(ParamList *L, char *tname);
PLElemPtr PLGetAndMove(ParamList *L);
// --------------- kontrolni seznam parametru ---------------


// ----------------- seznam tabulek symbolu -----------------

typedef struct STListElem {
	char *name;
	int forward;
	tBSTNodePtr root;
	tListOfInstr instrList;
	ParamList params;
	struct STListElem *next;
} *STElemPtr;


typedef struct  {
	STElemPtr First;
	STElemPtr Act;
	STElemPtr Last;
} STList;

void STListError();
void STListInit(STList *L);
STElemPtr STNewTable(STList *L, char* tname, int forward);
void STListDispose(STList *L);
void STSucc (STList *L);
int STActive (STList *L);
tBSTNodePtr *STGetTable(STList *L, char *tname);
STElemPtr STGetElem(STList *L, char *tname);
int semIsFunction(TToken token);

// ----------------- seznam tabulek symbolu -----------------

tBSTNodePtr STFind(TToken token); // hleda promennou v aktualni TS
tBSTNodePtr STFindG(TToken token); // hleda promennou v aktualni, pak v globalni TS
tBSTNodePtr STInsertLiteral(TToken token);

void enterLocalTable(char *fname);
int semIsFunction(TToken token);
void semInit();
int semTypeTokenType(TToken token);
void STInsert(TToken varToken, TToken typeToken);
tBSTNodePtr STInsertLiteral(TToken token);
void semDeclaredVariable();
void semDeclaredParameter(STElemPtr funcTable);
void semDeclaredFunction(int isForward);
int semGetType(TToken token);
void semCheckAssignment();
int semCheckExpression();
void semCheckFcall();
void semEndOfFunction();
void semInsertBuiltinFunctions();
