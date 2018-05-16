/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: interpret.h
*Autor: Matej Jezersky, xjezer01
*		Vaclav Bayer, xbayer05
*/

// ------------------ ramec (seznam prvků) ------------------
typedef struct frameElem {
	char *key;				// ID
	int type;				// rozliseni typu ze ktereho se bude cist promenne
	char *strdata;			// string hodnota
  	int intdata;			// int hodnota
  	double doudata;			// double hodnota
  	int init;				// je hodnota inicializovana?
  	struct frameElem *next;	// odkaz na predposledni prvek
} *frameElemPtr;


typedef struct  {
	frameElemPtr First;
	frameElemPtr Act;
	frameElemPtr Last;
} FrameList;
// ------------------ konec ramce (seznam prvků) ------------------


// ------------------ zasobnik ------------------
typedef struct FStackElem {
	FrameList F;
	tListOfInstr instrList;
	struct FStackElem *below;
} *FSEptr;

typedef struct {
	FSEptr top;
} FStack;
// ------------------ konec zasobniku ------------------


/*
pouziti zasobniku:
nejdriv se inicializuje   FSInit(FrameStack *S);
vlozeni prvku na vrchol   FSPush(FrameStack *S);
ziskani a smazani vrcholu FSPop(FrameStack *S);
precteni dat na vrcholu   FSTop(FrameStack *S);
pri selhani malloc vraci ISPush hodnotu -1
*/
void FSInit(FStack *S);
int FSEmpty(FStack *S);
void FSPop(FStack *S);
FrameList *FSTop(FStack *S);
int FSPush(FStack *S);
void FSDispose(FStack *S);


/*
pouziti seznamu:
inicializace			  	FrameListInit(FrameList *F);
vlozeni prvku na konec    	FrameListInsert(FrameList *F, char *key, int type, int data, int init);
uklizeni seznamu		 	FrameListDispose(FrameList *F);
vraceni prvku   			FLGetElem(FrameList *F, char *key);
aktivita na prvni prvek		FLSelectFirst (FrameList *F);
smazani prvniho	prvku		FLDeleteFirst (FrameList *F);
aktivita doprava			FLSucc (FrameList *F);
overeni aktivity			FLActive (FrameList *F);
*/
void FrameListInit(FrameList *F);
void FrameListInsert(FrameList *F, char *key, int type, int isInit);
void FrameListDispose(FrameList *F);
frameElemPtr FLGetElem(FrameList *F, char *key);
void FLSelectFirst (FrameList *F);
void FLDeleteFirst (FrameList *F);
void FLSucc (FrameList *F);
int FLActive (FrameList *F);

frameElemPtr FLFind(FrameList *F, char *key);
void STCopyToFrame(tBSTNodePtr fromTable, FrameList *toFrame);

int inter();
