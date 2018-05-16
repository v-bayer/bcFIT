/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: syntax.h
*Autor: Matouš Jezersky, xjezer01
*/

#include "i_list.h"

#define DEBUGGING 0

#define SYNTAX_ERROR 2
#define TRUE 1
#define FALSE 0
#define RUNTIME_BAD_NUMBER 6
#define RUNTIME_UNINIT 7
#define RUNTIME_ZERODIV 8
#define RUNTIME_GENERIC 9

#define INTERNAL_ERROR 99



#define BOOLEAN 199 // doplneni lexikalni analyzy

// pravidla LL gramatiky
#define LL_BEGIN 200
#define LL_COLON 201
#define LL_SEMICOLON 202
#define LL_RBRACK 203
#define LL_LBRACK 204
#define LL_DOT 205
#define LL_EOF 206

#define LL_PROG 207 
#define LL_COMP_STAT 208
#define LL_FUNC_SEQ 209
#define LL_DECL_SEQ 210
#define LL_STAT_SEQ 211
#define LL_SEM_OR_END 212
#define LL_STATEMENT 213
#define LL_FUNCTION 214
#define LL_FUNC_BODY 215
#define LL_TYPE 216
#define LL_PARAMS 217
#define LL_ID 218
#define LL_ELSE 219
#define LL_THEN 220
#define LL_DO 221
#define LL_EXPR_OR_FCALL 222
#define LL_OPERATOR 223
#define LL_EXPR 224
#define LL_VAR 225
#define LL_DECLARATION 226
#define LL_PARAM_OR_EPS 227
#define LL_DECLARE_PARAMS 228
#define LL_DECLARE_PARAM_OR_EPS 229
// #define LL_FCALL 230 rozsireni
#define LL_ID_OR_LITERAL 231
#define LL_ASSIGN 232
#define LL_OP_OR_FCALL 233
#define LL_READLN 234
#define LL_WRITE 235

#define LL_EXPR_RBRACK 236

// pro semantiku
#define LL_FUNC_TYPE 237

#define GENERATE_END_OF_FCALL 238

// #define END_OF_FCALL 238 rozsireni
#define END_OF_FUNCTION 240
#define END_OF_EXPRESSION 241 // pro sem. an. a prec. an. ; neni zahrnuto jako pravidlo, pouze jako typ tokenu


#define TEMP 987 // docasny typ - muze se zmenit na INT nebo REAL behem interpretace ( typ mezivysledku )



typedef struct {
	int type;
	string data;
} TToken;




// ------------------ zasobniky ------------------

typedef struct CleanupStackElem {
	TToken data;
	struct CleanupStackElem *below;
} *CSEptr;


typedef struct {
	CSEptr top;
} CleanupStack;



void CleanupStackInit(CleanupStack *S);
int CleanupStackEmpty(CleanupStack *S);
void CleanupStackPop(CleanupStack *S);
void CleanupStackPush(CleanupStack *S, TToken ptr);
void CleanupStackDispose(CleanupStack *S);
// -------------- konec zasobniku ---------------

int error(int errcode, char *errmsg);
void dprintf(char *text);
TToken newToken();
int cmpTD (TToken token, char *D);
int SAIsOperator(TToken token);
int SAIsRelationOperator(TToken token);
int SAIsLiteral(TToken token);
TToken SACreateEmptyToken(int tokenType);
TToken SACreateToken(char *data, int type);
TToken SACreateUniqueId();

tListItem *generateInstruction(int instType, void *addr1, void *addr2, void *addr3);
void SAResetInstruction();

void SAProcess(int rule);
