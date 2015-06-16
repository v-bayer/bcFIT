/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: ial.h 
*Autor: Matous Jezersky, xjezer01
*		Vaclav Bayer, xbayer05
*		Simon Hives, xhives00
*/
typedef struct tBSTNode {
	char *key;
	int type;
	int isLiteral;
	struct tBSTNode *LPtr;
	struct tBSTNode *RPtr;
} *tBSTNodePtr;

void BSTInit (tBSTNodePtr *RootPtr);
tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char *K);
tBSTNodePtr BSTInsert (tBSTNodePtr *RootPtr, char *K, int type, int isLiteral);
void BSTDelete (tBSTNodePtr *RootPtr, char *K);
void BSTDispose (tBSTNodePtr *RootPtr);

void prepare(int charJump[], char* pattern, int patLen);
int find(char *string, char *pattern);
void sort(string *slovo);
void quickSort(int zaciatok, int koniec, string *slovo);
int length(char *s);
void copy(char *s, int i, int n);
