/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: key_word_table.h
*Autor: Tomas Furch, xfurch01
*/

#define KEYWORDTABLESIZE 31

/*Datová položka TRP s explicitně řetězenými synonymy*/
 typedef struct tHTItem{
	char* key;				/* klíč  */
	struct tHTItem* ptrnext;	/* ukazatel na další synonymum */
} tHTItem;

/* TRP s explicitně zřetězenými synonymy. */
typedef tHTItem* tHTable[KEYWORDTABLESIZE];



/* Hlavičky funkcí. */

int hashCode ( char* key );
void KeyWordTableInit ( tHTable* ptrht );
tHTItem* KeyWordSearch ( tHTable* ptrht, char* key );
void KeyWordTableInsert ( tHTable* ptrht, char* key);
void ClearKeyWordTable(tHTable* ptrht);
void fillKeyWordTable(tHTable* ptrht);	
void htPrintTable(tHTable* ptrht);

/* Konec hlavičkového souboru keyWordTable.h */
