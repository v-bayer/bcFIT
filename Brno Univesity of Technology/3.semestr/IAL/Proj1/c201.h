/**/
/* P�edm�t: Algoritmy (IAL) - FIT VUT v Brn�
 * Hlavi�kov� soubor pro c201.c (Jednosm�rn� v�zan� line�rn� seznam)
 * Vytvo�il: Martin Tu�ek, z��� 2005
 * Upravil: Bohuslav K�ena, ��jen 2014
 * 
 *
 * Tento soubor, pros�me, neupravujte! 
 * Please, do not modify this file! 
 */
 
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>

#define TRUE 1
#define FALSE 0
                                           /* Indikace, zda byl p��klad �e�en */
                                   /* Detects whether the task is implemented */
extern int solved;                         
                                                            /* Indikace chyby */
                                                                /* Error flag */
extern int errflg;
                                                             /* Prvek seznamu */
                                                /* Definition of list element */
typedef struct tElem {
    struct tElem *ptr;
    int data;
} *tElemPtr;	               
                                                            /* Vlastn� seznam */
                                                        /* Definition of list */
typedef struct { 
    tElemPtr Act;
    tElemPtr First;
} tList;
                                                   /* Funkce pro implementaci */
                                               /* Functions to be implemented */
void InitList (tList *);
void DisposeList (tList *);
void InsertFirst (tList *, int);
void First (tList *);
void CopyFirst (tList *, int *);
void DeleteFirst (tList *);
void PostDelete (tList *);
void PostInsert (tList *, int);
void Succ (tList *);
void Copy (tList *, int *);	
void Actualize (tList *, int);
int  Active (tList *);

/* Konec hlavi�kov�ho souboru c201.h */
/**/
