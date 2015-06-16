/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: i_list.h 
*Autor: Tomas Furch, xfurch01
*/

#define I_ADD             	401 // +	citatel		citatel		soucet (int/real)
#define I_DEC             	402 // -	mensenec	mensitel	rozdil (int/real)
#define I_MUL          		403 // *	cinitel		cinitel		soucin (int/real)
#define I_DIV            	404 // /	delenec		delitel		podil (real)
#define I_LES              	405 // <	toto		nez_toto	vysledek (boolean)
#define I_GTR           	406 // >	toto		nez_toto	vysledek (boolean)
#define I_NEQ          		407 // <>	toto		tomuto		vysledek (boolean)
#define I_LEQ       		408 // <=	toto		nez_toto	vysledek (boolean)
#define I_GEQ    			409 // >=	toto		nez_toto	vysledek (boolean)
#define I_EQL             	410 // =	toto		totomuto	vysledek (boolean)

#define I_CONCAT 			420 // kon.	retezec1	s_retezcem2	vysledek (string)

#define I_ASSIGN 			430 // prirazeni 	co			NULL		kam
#define I_CALL 				431 // volani 		NULL		NULL		NULL
#define I_NEWFRAME			432 // novy ramec 	STElemPtr	NULL		NULL
#define I_ASSIGN_PARAM		433 // prir. param. co(z act)	NULL		kam
#define I_RETURN			434 // navrat z fce instrList	NULL		NULL

#define I_READLN			440 // readln		kam_ulozit	NULL	NULL
#define I_WRITE				441 // write		co_vypsat	NULL	NULL

#define I_JMP				450 // skok			NULL		NULL	adresaInstrukce
#define I_JMP_ZERO			451 // skok (p==0)  podminka	NULL	adresaInstrukce
#define I_JMP_NONZERO		452 // skok (p!=0)  podminka	NULL	adresaInstrukce
#define I_LABEL				453	// label		NULL		NULL	NULL

// instrukce built-in funkci
#define I_LENGTH			460	// length		NULL		NULL	NULL
#define I_COPY				461 // copy			NULL		NULL	NULL
#define I_FIND				462 // find			NULL		NULL	NULL		
#define I_SORT				463 // sort			NULL		NULL	NULL


typedef struct
{
  int instType;  
  void *addr1; 
  void *addr2; 
  void *addr3; 
} tInstr;

typedef struct listItem
{
  tInstr Instruction;
  struct listItem *nextItem;
  struct listItem *previousItem;
} tListItem;
    
typedef struct
{
  struct listItem *first;  
  struct listItem *last;   
  struct listItem *active; 
} tListOfInstr;

void listInstrInit(tListOfInstr *L);
void listInstrFree(tListOfInstr *L);
tListItem *listInstrInsertLast(tListOfInstr *L, tInstr I);
void listInstrFirst(tListOfInstr *L);
void listInstrNext(tListOfInstr *L);
void listInstrPrevious(tListOfInstr *L);
void listInstrGoto(tListOfInstr *L, void *gotoInstr);
void *listInstrGetPointerLast(tListOfInstr *L);
tInstr *listInstrGetData(tListOfInstr *L);
void listInstrCopy(tListOfInstr *fromList, tListOfInstr *toList);

/* End of header file iList.h */
