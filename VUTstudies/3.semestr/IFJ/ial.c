/**
*Projekt: IFJ14 intepreter
*Resitele: xjanot01, xjezer01, xbayer05, xfurch01, xhives00
*
*Soubor: ial.c 
*Autor: Matous Jezersky, xjezer01
*		Vaclav Bayer, xbayer05
*		Simon Hives, xhives00
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define TRUE 1
#define FALSE 0


#include "str.h"
#include "ial.h"
#include "scaner.h"
#include "syntax.h"

/*
BSTInit		-> 	Inicializuje strom.
BSTSearch	->	Vyhledavani ve stromu.
BSTInsert	->	Vlozi uzel do stromu.
BSTDelete	->	Odstrani uzel stromu.
BSTDispose	->	Zrusi strom.
*/


void BSTInit (tBSTNodePtr *RootPtr) {
*RootPtr = NULL;
}

tBSTNodePtr BSTSearch (tBSTNodePtr RootPtr, char *K)	{
	
	if (RootPtr == NULL) {
		//printf("BST NULL\n");
		return NULL; // pokud je ukazatel NULL a dosud nebyl K nalezen, vracim FALSE
	}
	else {
		if (strcmp(RootPtr->key, K) == 0) { // pokud jsem nasel klic...
			//printf("STRCMP %s a %s\n", RootPtr->key, K);
			return RootPtr;
		}
		else if (strcmp(RootPtr->key, K) > 0) {
			//printf("STRCMP %s a %s\n", RootPtr->key, K);
		 	return BSTSearch(RootPtr->LPtr, K); // pokud je K mensi nez Key, hledam vlevo
		}
		else {
			//printf("STRCMP %s a %s\n", RootPtr->key, K);
			return BSTSearch(RootPtr->RPtr, K); // pokud je vetsi, hledam vpravo
		}
	}	
	
}


tBSTNodePtr BSTInsert (tBSTNodePtr* RootPtr, char *K, int type, int isLiteral)	{	

	if (*RootPtr == NULL) { // pokud je ukazatel NULL, muzu vlozit novy na jeho misto
		tBSTNodePtr newNode = malloc(sizeof(struct tBSTNode));
		if (newNode == NULL) {
			error(INTERNAL_ERROR, "BST malloc failed");
			return NULL;
		}
		newNode->key = K;
		newNode->type = type;
		newNode->isLiteral = isLiteral;
		newNode->LPtr = NULL;
		newNode->RPtr = NULL;
		*RootPtr = newNode; // nastavim ukazatel na uzel na nove vytvoreny uzel
		return *RootPtr;
	}
	else {
		if (strcmp((*RootPtr)->key, K) == 0) {
			(*RootPtr)->type = type;
			return *RootPtr;
		}
		else if (strcmp((*RootPtr)->key, K) > 0) return BSTInsert(&((*RootPtr)->LPtr), K, type, isLiteral); // pokud je K mensi nez Key, hledam vlevo
		else return BSTInsert(&((*RootPtr)->RPtr), K, type, isLiteral); // pokud je K vetsi nez Key, hledam vpravo
		
	}
}

void ReplaceByRightmost (tBSTNodePtr PtrReplaced, tBSTNodePtr *RootPtr) {
	
	// k situaci RootPtr == NULL nemuze dojit (osetreno v BSTDelete i pri rekurzivnim volani teto funkce)
	
	if ((*RootPtr)->RPtr == NULL) { // pokud strom nepokracuje dale doprava, jsem na nejpravejsim uzlu
		tBSTNodePtr tmpPtr; // pomocna promenna
		tmpPtr = *RootPtr;
		PtrReplaced->key = (*RootPtr)->key; // presunu klic
		PtrReplaced->type = (*RootPtr)->type;
		PtrReplaced->isLiteral = (*RootPtr)->isLiteral;
		*RootPtr = (*RootPtr)->LPtr; // je mozne, ze se nejedna o list, musim uzel nahradit jeho levym uzlem
		free(tmpPtr); // uvolnim nejpravejsi uzel, ktery jsem si kvuli predchozimu radku dal do pomocne promenne
	}
	else ReplaceByRightmost(PtrReplaced, &((*RootPtr)->RPtr)); // pokud pokracuje, predavam funkci ukazatel na uzel napravo
}

void BSTDelete (tBSTNodePtr *RootPtr, char *K) 		{

	if (*RootPtr != NULL) {
		if (strcmp((*RootPtr)->key, K) == 0) {
			tBSTNodePtr tmpPtr; // pomocna promenna
			tmpPtr = *RootPtr;
			
			 // pokud uzel obsahuje oba podstromy
			 // nahradim uzel nejpravejsim uzlem leveho podstromu, uvolnovani pameti provedle funkce ReplaceByRightmost
			if ((*RootPtr)->RPtr != NULL && (*RootPtr)->LPtr != NULL) ReplaceByRightmost(*RootPtr, &(*RootPtr)->LPtr); 
			
			// pokud obsahuje pouze pravy podstrom, nahradim jej timto podstromem
			else if ((*RootPtr)->RPtr != NULL) {
				*RootPtr = (*RootPtr)->RPtr;
				free(tmpPtr); // uvolnim ruseny uzel
			}			
			// stejne tak pro levy podstrom
			else {
				*RootPtr = (*RootPtr)->LPtr;
				free(tmpPtr); // uvolnim ruseny uzel
			} 
			
		}
		else if (strcmp((*RootPtr)->key, K) > 0) BSTDelete(&((*RootPtr)->LPtr), K); // pokud je K mensi nez Key, hledam vlevo
		else BSTDelete(&((*RootPtr)->RPtr), K); // pokud je klic mensi nez K, hledam vpravo
		
	}

} 

void BSTDispose (tBSTNodePtr *RootPtr) {		
	if (*RootPtr != NULL) {
		if ((*RootPtr)->LPtr != NULL) BSTDispose(&((*RootPtr)->LPtr)); // pokud uzel obsahuje pravy podstrom, pokracuju vpravo
		if ((*RootPtr)->RPtr != NULL) BSTDispose(&((*RootPtr)->RPtr)); // pokud uzel obsahuje levy podstrom, pokracuju vlevo
		free(*RootPtr); // v tento moment jiz nemuze uzel obsahovat zadny podstrom, muzu jej uvolnit
		*RootPtr = NULL;
	}

}

/*
 * Function: prepare
 * Params: 1) pole skokov
 *         2) pattern
 *         3) dlzka patternu
 * Return:  void
 * Description: Naplni pole skokov hodnotamy
 */
void prepare(int charJump[], char* pattern, int patLen)
{
	int j;
	for(j = 0; j < 256; j++)
		charJump[j] = patLen; // vyplni tabulku skokov hodnotamy dlzky patternu
	for(j = 0; j < patLen; j++)
		charJump[(int)pattern[j]] = patLen -j -1 ;
	// znaky vyskitujuce sa v patterne budu mat znizene cislo skoku
	// podla toho ako daleko su od konca patternu
	return;
}

/*
 * Function: find
 * Param: 1) string , v ktorom hladame pattern
 *        2) pattern
 * Return:  vrati poziciu v ktorej sa nachadza pattern v zadanom stringu, inac vrati 0
 * Description:  Boyer-Moore alogrithm: Bad Character Heuristic
 *				 funkcie urci dlzky retezcov, zavola funkciu prepare, ktera
 *         		 vyhodnoti hledany pattern. Nasledne sa vyhodnoti index na 
 *               ktorom sa (ne)nachadza pattern v stringu
 */
int find(char *string, char *pattern)
{
	// pomocne premenne
	int strLen = strlen(string);
	int patLen = strlen(pattern);
	int charJump[256];
    int missmatch = 0; 

	int i = 0;
	int lastChar = patLen - 1;

	// vyplni tabulku skokov (charJump)
	prepare(charJump, pattern,  patLen);

	if (patLen == 0) // prazdny pattern sa nachadza v lubovolnom stringu na prvom mieste (podla zadania)
		return 1;
	if (strLen < patLen)
	{
		return 0;
	}
	else
	{
		while (i <= strLen - patLen )
		{
            missmatch=0; // reset hodnoty missmatch
			while (string[i+lastChar] == pattern[lastChar]) // porovna ci sa zhoduju retazce na danom indexe
			{
				if (lastChar == 0) // pokial sa pri porovnavani doslo az nakoniec patternu bez nezhody
					return i + 1;  // vrati poziciu +1 z duvodu indexace stringu v pascale od 1
				lastChar--;		   // posunie sa na dalsi znak pre porovnavanie
			    if (string[i+lastChar] != pattern[lastChar])
                    missmatch=1;
            }
			if (missmatch)
                 i=i+lastChar+1; // pokial doslo pri porovnavani k nezhode, pomocou missmatch sa zvacsi i o 1
            else
                i = i + charJump[(int)string[i + lastChar]]; // zisti z tabulky charJump kolko znakov moze preskocit
			lastChar = patLen - 1; // v pripade ze sa manipulovalo s lastCharom, cize sa porovnaval pattern ale nedoslo k finalnej zhode, nastavi sa o5 na povodnu hodnotu
		}
	}
	return 0; // vrati nulu pokial nebol podretazec v retazci najdeny
}

/*
 * Function: sort
 * Params:   string slovo ktore zoradujeme
 * Return:   void
 * Description: Funkcia nastavi pociatocny a koncovy index slova a zavola funkciu quickSort pre zoradenie
 */
void sort(string *slovo)
{
	unsigned int dlzka = slovo->length;
	if (dlzka > 0)
	{
	int zaciatok = 0;
	int koniec = dlzka-1;
	quickSort(zaciatok,koniec,slovo);
	}
	return;
}

/*
 * Function: quickSort
 * Params:  zaciatok
			koniec
			slovo
 * Return: void
 * Description:  Funkcia zoradi znaky v slove od najmensieho po najvacsi pomocou metody quicksort (rekurzivne).
 */
void quickSort(int zaciatok, int koniec, string *slovo)
{
	// pivot deli slovo na dve casti
	// do jednej casti presunie znaky mensie a do druhej vetsie ako pivot
	// p je pomocna premenna pre zamenu znakov, i a j predstavuju pociatocny a konecny index
	int pivot,p; 
	int i = zaciatok;
	int j = koniec;
	// nastavenie pivotu na stredny znak v slove
	pivot = slovo->str[ (zaciatok+koniec) / 2 ];
	do
	{
		// hladanie indexov :
		while (slovo->str[i] < pivot) i++; // znak >= pivot (hlada od zaciatku)
		while (slovo->str[j] > pivot) j--; // znak <= pivot (hlada od konca)
		
		if (i <= j)
		{
			// zamena znakov v slove pokial i<=j 
			p= slovo->str[j];
			slovo->str[j] = slovo->str[i];
			slovo->str[i] = p;
			i++;
			j--;
		}
	}while( i <= j ); // vykonava sa pokial je pociatocny index mensi alebo rovny koncovemu indexu
	// rekurzivne volanie funkcie qiuckSort pre:
	if (j > zaciatok) quickSort(zaciatok,j,slovo); // znaky mensie nez pivot
	if (i < koniec) quickSort(i,koniec,slovo);     // znaky vacsie nez pivot
	return;
}

/*
 * Function: 	length
 * Param: 		char *s, ukazatel na char
 * Return: 		vracia dlzku zadaneho retazca (int)
 * Description: funkcie urci dlzku retezcu
 */
int length(char *s)
{
	if (s==NULL) 
	{
		error(RUNTIME_GENERIC,"length error, wrong parameter *s");
		return 0;
	}
	
	if (s[0]=='\0') return 0; // pokud je retezec prazdny vraci 0
	else
	{
		int i = 0;
		while (s[i] != '\0')
			i++;
		return i; // jinac vrati delku retezce
	}
}

/*
 * Function: 	copy
 * Param: 		char *s, ukazatel na char
 *				int i, pociatocny index
 *				int n, urcuje pocet prvkov 
 * Return: 		void
 * Description: funkcia z povodneho retazca vezme n charov od indexu i a tymto retazcom nahradi povodny
 */
void copy(char *s, int i, int n) 
{
	if (i<1) // v jazyku IFJ14 ja pozice prveho znaku na indexu 1, chovani podle fpc
	{
		i = 1;
	}
	if (n<0) // nelze pracovat se zapornym poctem znaku
	{
		n = 0; // opet podle fpc
	}	
	if (s==NULL) 
	{
		error(RUNTIME_GENERIC,"copy error, wrong parameter *s");
		return;
	}
	
	int l = length(s);
	
	if (i>l) // pokud je index vetsi nez delka celeho retezce vynuluje retezec
	{
		int j;
		for(j = 0; j < l; j++)
		{
			s[j]='\0';
		}
		return;
	}
	
	if (n == l && i == 1) // pocatek retezce ma byt pocitany od jednicky
	    return;			  // pokud n==dlzka retezce a i==pociatok retazca, nic nemeni
	else
	{	
		int j;
		for(j=0; j <= l; j++)
		{
			if (j<n)
				s[j]=s[i-1];
			else
				s[j]='\0'; // vyplni zbyle mista '\0'
			i++;
		}
		return;
	}
}
