
/* c201.c *********************************************************************}
{* Téma: Jednosmìrný lineární seznam
**
**                     Návrh a referenèní implementace: Petr Pøikryl, øíjen 1994
**                                          Úpravy: Andrea Nìmcová listopad 1996
**                                                   Petr Pøikryl, listopad 1997
**                                Pøepracované zadání: Petr Pøikryl, bøezen 1998
**                                  Pøepis do jazyka C: Martin Tuèek, øíjen 2004
**	                                      Úpravy: Bohuslav Køena, øíjen 2014
**
** Implementujte abstraktní datový typ jednosmìrný lineární seznam.
** U¾iteèným obsahem prvku seznamu je celé èíslo typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou typu tList.
** Definici konstant a typù naleznete v hlavièkovém souboru c201.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu s vý¹e
** uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ tList:
**
**      InitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DisposeList ... zru¹ení v¹ech prvkù seznamu,
**      InsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      First ......... nastavení aktivity na první prvek,
**      CopyFirst ..... vrací hodnotu prvního prvku,
**      DeleteFirst ... zru¹í první prvek seznamu,
**      PostDelete .... ru¹í prvek za aktivním prvkem,
**      PostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      Copy .......... vrací hodnotu aktivního prvku,
**      Actualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      Succ .......... posune aktivitu na dal¹í prvek seznamu,
**      Active ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci funkcí nevolejte ¾ádnou z funkcí implementovaných v rámci
** tohoto pøíkladu, není-li u dané funkce explicitnì uvedeno nìco jiného.
**
** Nemusíte o¹etøovat situaci, kdy místo legálního ukazatele na seznam
** pøedá nìkdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodnì komentujte!
**
** Terminologická poznámka: Jazyk C nepou¾ívá pojem procedura.
** Proto zde pou¾íváme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c201.h"

int solved;
int errflg;

void Error() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;                      /* globální promìnná -- pøíznak chyby */
}

void InitList (tList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/

 	L->Act = NULL;
    L->First = NULL;						// Nastavení obou prvku na NULL
 	return;

}

void DisposeList (tList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam L do stavu, v jakém se nacházel
** po inicializaci. Ve¹kerá pamì» pou¾ívaná prvky seznamu L bude korektnì
** uvolnìna voláním operace free.
***/

    tElemPtr PomUk;

     while ((PomUk = L->First) != NULL) {		// Cykli dokud neni uklizeno po vsech prvcich
        L->First = PomUk->ptr;				    // Prirazeni nasledujiciho prvku na prvni prvek
        free(PomUk);							// Uklizeni
    }
    L->First = NULL;                        // Deaktivace seznamu
    L->Act = NULL;							// Pro oba prvky
    return;
}

void InsertFirst (tList *L, int val) {
/*
** Vlo¾í prvek s hodnotou val na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci Error().
**/

    struct tElem *New;					     // Pomoci pomocne struktury vytvorim novy prvek
    if ((New = malloc(sizeof(struct tElem))) == NULL) {		// Overeni, zda-li se muze pro nej vyhradit misto
        Error();							// Pokud ne, skoncim s chybovou hlaskou
        return;								// Ukoncim
    }
    New->data = val;						// Ulozeni hodnoty
    New->ptr = L->First;
    L->First = New;
    return;                                 //Ukoncim

}

void First (tList *L) {
/*
** Nastaví aktivitu seznamu L na jeho první prvek.
** Funkci implementujte jako jediný pøíkaz, ani¾ byste testovali,
** zda je seznam L prázdný.
**/

        L->Act = L->First;                  // Nadstavi aktivitu na prvni prvek
        return;
}

void CopyFirst (tList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci Error().
**/

    if (L->First == NULL) {		// Overeni, zda je seznam prazdny
		Error();				// Jestlize ano, vyhodim chybovou hlasku
		return;
	}
	*val = L->First->data;		// Jinak vracim hodnotu prvku
	return;
}

void DeleteFirst (tList *L) {
/*
** Zru¹í první prvek seznamu L a uvolní jím pou¾ívanou pamì».
** Pokud byl ru¹ený prvek aktivní, aktivita seznamu se ztrácí.
** Pokud byl seznam L prázdný, nic se nedìje.
**/

    if (L->First != NULL) {		    // Overeni, zda seznam neni prazdny
        if (L->Act == L->First) {	// Pokud je prvni prvek zaroven aktivni
            L->Act = NULL;			// Zahodim aktivitu
        }
	tElemPtr PomUk = L->First;	    // Na pomocny prvek vlozim prvni prvek
    L->First = PomUk->ptr;		    // Vlozeni druheho prvku na prvni prvek
	free(PomUk);					// Odstraneni pomocneho prvku
	return;
    }
}

void PostDelete (tList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem a uvolní jím pou¾ívanou pamì».
** Pokud není seznam L aktivní nebo pokud je aktivní poslední prvek seznamu L,
** nic se nedìje.
**/

    if (L->Act == NULL || L->Act->ptr == NULL) {	// Pokud neni zadny aktivni prvek, nebo je zaroven poslednim prvkem
		return;									    // Ukoncim
	}
	tElemPtr PomUk = L->Act->ptr;                   // Nasledujici prvek za aktivnim prvkem se vlozi do pomocneho prvku
    if(PomUk->ptr != NULL){                         // Overeni, zda pomocny prvek neni posledni
        L->Act->ptr = PomUk->ptr;                   // Vynechani nasledujiciho prvku za aktivnim prvkem
    }
    else {
		L->Act->ptr = NULL;                         // Aktivni prvek je poslednim prvkem
	}
	free(PomUk);                                    // Uklizeni
	return;                                         // Ukonceni
}

void PostInsert (tList *L, int val) {
/*
** Vlo¾í prvek s hodnotou val za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje!
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** zavolá funkci Error().
**/

    struct tElem *New;                                      // Vytvoreni pomocne struktury
    if ((New = malloc(sizeof(struct tElem))) == NULL) {		// Overeni, zda-li je volne misto pro vytvoreni
        Error();							                // Vyhodim chybovou hlasku
        return;								                // Ukonceni
    }

    if (L->Act != NULL) {                                   // Overeni, zda je seznam aktivni
        New->data = val;                                    // Prirazeni hodnoty
        New->ptr = L->Act->ptr;
        L->Act->ptr = New;                                  // Presmerovani ukazatele
    }
}

void Copy (tList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam není aktivní, zavolá funkci Error().
**/

    if (L->Act != NULL) {           // Overeni, zda je seznam aktivni
        *val = L->Act->data;        // Prirazeni hodnoty prvku
        return;                     // Ukonceni
    }else{
        Error();                    // Nebo vyhodi chybovou hlasku
        return;                     // A ukonci
    }
}

void Actualize (tList *L, int val) {
/*
** Pøepí¹e data aktivního prvku seznamu L hodnotou val.
** Pokud seznam L není aktivní, nedìlá nic!
**/

    if (L->Act != NULL) {           // Overeni, zda je seznam aktivni
        L->Act->data = val;         // Prirazeni hodnoty prvku
        return;                     // A ukonci
    }
}

void Succ (tList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** V¹imnìte si, ¾e touto operací se mù¾e aktivní seznam stát neaktivním.
** Pokud seznam L není aktivní, nedìlá nic.
**/

    if (L->Act == NULL) {		// Overeni, zda je seznam aktivni
		return;					// Ukoncim
	}
	L->Act = L->Act->ptr;		// Prepisu aktivitu na prvek za aktivnim
	return;					    // Ukoncim
}

int Active (tList *L) {
/*
** Je-li seznam L aktivní, vrací TRUE. V opaèném pøípadì vrací FALSE.
** Tuto funkci implementujte jako jediný pøíkaz return.
**/

    return (L->Act != NULL);
}

/* Konec c201.c */
