
/* c206.c **********************************************************}
{* Téma: Dvousmìrnì vázaný lineární seznam
**
**                   Návrh a referenèní implementace: Bohuslav Køena, øíjen 2001
**                            Pøepracované do jazyka C: Martin Tuèek, øíjen 2004
**                                            Úpravy: Bohuslav Køena, øíjen 2014
**
** Implementujte abstraktní datový typ dvousmìrnì vázaný lineární seznam.
** U¾iteèným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován promìnnou
** typu tDLList (DL znamená Double-Linked a slou¾í pro odli¹ení
** jmen konstant, typù a funkcí od jmen u jednosmìrnì vázaného lineárního
** seznamu). Definici konstant a typù naleznete v hlavièkovém souboru c206.h.
**
** Va¹ím úkolem je implementovat následující operace, které spolu
** s vý¹e uvedenou datovou èástí abstrakce tvoøí abstraktní datový typ
** obousmìrnì vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu pøed prvním pou¾itím,
**      DLDisposeList ... zru¹ení v¹ech prvkù seznamu,
**      DLInsertFirst ... vlo¾ení prvku na zaèátek seznamu,
**      DLInsertLast .... vlo¾ení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zru¹í první prvek seznamu,
**      DLDeleteLast .... zru¹í poslední prvek seznamu,
**      DLPostDelete .... ru¹í prvek za aktivním prvkem,
**      DLPreDelete ..... ru¹í prvek pøed aktivním prvkem,
**      DLPostInsert .... vlo¾í nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vlo¾í nový prvek pøed aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... pøepí¹e obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal¹í prvek seznamu,
**      DLPred .......... posune aktivitu na pøedchozí prvek seznamu,
**      DLActive ........ zji¹»uje aktivitu seznamu.
**
** Pøi implementaci jednotlivých funkcí nevolejte ¾ádnou z funkcí
** implementovaných v rámci tohoto pøíkladu, není-li u funkce
** explicitnì uvedeno nìco jiného.
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

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornìní na to, ¾e do¹lo k chybì.
** Tato funkce bude volána z nìkterých dále implementovaných operací.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální promìnná -- pøíznak o¹etøení chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L pøed jeho prvním pou¾itím (tzn. ¾ádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádìt nad ji¾ inicializovaným
** seznamem, a proto tuto mo¾nost neo¹etøujte. V¾dy pøedpokládejte,
** ¾e neinicializované promìnné mají nedefinovanou hodnotu.
**/

    L->First = NULL;				// Inicializace seznamu, nadstaveni hodnot na NULL
 	L->Act = NULL;
    L->Last = NULL;
    return;
}

void DLDisposeList (tDLList *L) {
/*
** Zru¹í v¹echny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Ru¹ené prvky seznamu budou korektnì
** uvolnìny voláním operace free.
**/

    tDLElemPtr PomUk; 						// Vytvoreni pomocneho ukazatele

    while ((PomUk = L->First) != NULL) {	// Cykli dokud neni uklizeno po vsech prvcich
        L->First = PomUk->rptr;				// Prirazeni nasledujiciho prvku na prvni prvek
        free(PomUk);					    // Uklizeni
    }
    L->First = NULL;                        // Uvedeni prvku do puvodniho stavu
    L->Last = NULL;
    L->Act = NULL;
    return;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vlo¾í nový prvek na zaèátek seznamu L.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

    struct tDLElem *New;					// Pomoci pomocne struktury vytvorim novy prvek
    if ((New = malloc(sizeof(struct tDLElem))) == NULL) {   // Overeni, zda-li se muze pro nej vyhradit misto
        DLError();							// Pokud ne, skoncim s chybovou hlaskou
        return;								// Ukoncim
    }

    New->data = val;						// Ulozeni hodnoty
    New->lptr = NULL;						// Alokuju levy ukazatel
    New->rptr = L->First;					// Provaz novy prvek s prvnim prvkem

    if (L->First == NULL) {					// Pokud se jedna o prvni prvek
        L->Last = New;						// Musi na nej ukazovat i konec seznamu
    }else{
        L->First->lptr = New;               // Jinak provaz novy prvek s prvni prvkem
    }
    L->First = New;						    // Ukazuj zacatek na novy prvek
    return;
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vlo¾í nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

    struct tDLElem *New;					// Pomoci pomocne struktury vytvorim novy prvek
    if ((New = malloc(sizeof(struct tDLElem))) == NULL) {   // Overeni, zda-li se muze pro nej vyhradit misto
        DLError();							// Pokud ne, skoncim s chybovou hlaskou
        return;								// Ukoncim
    }

    New->data = val;						// Ulozeni hodnoty
    New->lptr = L->Last;					// Provaz novy prvek s poslednim prvkem
    New->rptr = NULL;					    // Alokuju pravy ukazatel

    if (L->First == NULL) {					// Pokud se jedna o prvni prvek
        L->First = New;						// Musi na nej ukazovat i konec seznamu
    }else{
        L->Last->rptr = New;                // Jinak provaz novy prvek s prvni prvkem
    }
    L->Last = New;						    // Ukazuj zacatek na novy prvek
    return;
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->First;                  // Aktivuji prvni prvek
    return;
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný pøíkaz (nepoèítáme-li return),
** ani¾ byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->Last;                  // Aktivuji posledni prvek
    return;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if (L->First == NULL) {		// Overeni, je-li seznam prazdny
		DLError();				// Vyhodim chybovou hlasku
		return;                 // Ukoncim
	}
	*val = L->First->data;		// Jinak vrat hodnotu prvniho prvku seznamu
	return;
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if (L->First == NULL) {		// Overeni, je-li seznam prazdny
		DLError();				// Vyhodim chybovou hlasku
		return;                 // Ukoncim
	}
	*val = L->Last->data;		// Jinak vrat hodnotu posledniho prvku seznamu
	return;
}

void DLDeleteFirst (tDLList *L) {
/*
** Zru¹í první prvek seznamu L. Pokud byl první prvek aktivní, aktivita
** se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/

    if (L->First == NULL) {		    // Overeni, je-li seznam prazdny
		return;					    // Ukoncim
	}
	if (L->Last == L->First) {	    // Pokud je seznam jen o jedinem prvku
		L->First = NULL;             // Bude ukazovat na NULL
        L->Act = NULL;
		L->Last = NULL;
	}
	if (L->Act == L->First) {	    // Pokud je prvni prvek aktivni
		L->Act = NULL;			    // Zahodim aktivitu
	}

	tDLElemPtr PomUk = L->First;	// Na pomocny prvek vlozim prvni prvek
	L->First = PomUk->rptr;		    // Vlozeni druheho prvku na prvni prvek
	L->First->lptr = NULL;		    // Odstraneni leveho ukazatele prvniho prvku
	free(PomUk);					// Odstraneni pomocneho prvku
	return;                         // Ukoncim
}

void DLDeleteLast (tDLList *L) {
/*
** Zru¹í poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se nedìje.
**/

    if (L->First == NULL) {		    // Overeni, je-li seznam prazdny
		return;					    // Ukoncim
	}
	if (L->Last == L->First) {	    // Pokud je seznam jen o jedinem prvku
		L->First = NULL;            // Bude ukazovat na NULL
        L->Act = NULL;
		L->Last = NULL;
	}
	if (L->Act == L->Last) {	    // Pokud je posledni prvek aktivni
		L->Act = NULL;			    // Zahodim aktivitu
	}

	tDLElemPtr PomUk = L->Last;	    // Na pomocny prvek vlozim posledni prvek
	L->Last = PomUk->lptr;		    // Vlozeni posledniho prvku na predposledni prvek
	L->Last->rptr = NULL;		    // Odstraneni praveho ukazatele posledniho prvku
	free(PomUk);					// Odstraneni pomocneho prvku
	return;                         // Ukoncim
}

void DLPostDelete (tDLList *L) {
/*
** Zru¹í prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se nedìje.
**/

    if (L->Act == NULL || L->Act == L->Last) {	    // Pokud neni zadny aktivni prvek, nebo je zaroven poslednim prvkem
		return;									    // Ukoncim
	}

	tDLElemPtr PomUk = L->Act->rptr;				// Nasledujici prvek za aktivnim prvkem se vlozi do pomocneho prvku
	if (PomUk->rptr != NULL) {						// Overeni, zda pomocny prvek neni posledni
		L->Act->rptr = PomUk->rptr;				    // Vynechani nasledujiciho prvku za aktivnim prvkem
		PomUk->rptr->lptr = L->Act;
	}
	else {
		L->Act->rptr = NULL;
		L->Last = L->Act;						    // Aktivni prvek je poslednim prvkem
	}
	free(PomUk);									// Uklizeni
	return;                                         // Ukonceni
}

void DLPreDelete (tDLList *L) {
/*
** Zru¹í prvek pøed aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se nedìje.
**/

    if (L->Act == NULL || L->Act == L->First) {	    // Pokud neni zadny aktivni prvek, nebo je zaroven poslednim prvkem
		return;									    // Ukoncim
	}

	tDLElemPtr PomUk = L->Act->lptr;				// Predchazejici prvek pred aktivnim prvkem se vlozi do pomocneho prvku
	if (PomUk->lptr != NULL) {						// Overeni, zda pomocny prvek neni prvni
		L->Act->lptr = PomUk->lptr;				    // Vynechani predchazejiciho prvku za pred prvkem
		PomUk->lptr->rptr = L->Act;
	}
	else {
		L->Act->lptr = NULL;
		L->First = L->Act;						    // Aktivni prvek je prvnim prvkem
	}
	free(PomUk);									// Uklizeni
	return;                                         // Ukonceni
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

    if (L->Act == NULL) {						// Pokud neni zadny aktivni prvek
        return;									// Ukoncim
    }
    struct tDLElem *New;						// Vytvoreni pomocne struktury
    if ((New = malloc(sizeof(struct tDLElem))) == NULL) {	// Overeni, zda-li je volne misto pro vytvoreni
        DLError();								// Vyhodim chybovou hlasku
        return;				                    // Ukonceni
    }

    New->data = val;							// Ulozeni hodnoty
    New->rptr = NULL;							// Ukazatel na nasledujici alokuji
    New->lptr = L->Act;							// Provaz novy prvek s aktualnim prvkem

    if (L->Act->rptr != NULL) {					// Pokud neni aktivni prvek poslednim prvkem
        L->Act->rptr->lptr = New;				// Provedu vlozeni a vzajemne provazani prvku
        New->rptr = L->Act->rptr;
    }
    else {
        L->Last = New;							// Novy prvek je poslednim prvkem seznamu
	}

    L->Act->rptr = New;
    New->lptr = L->Act;                         // Provazani
	return;
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vlo¾í prvek pøed aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se nedìje.
** V pøípadì, ¾e není dostatek pamìti pro nový prvek pøi operaci malloc,
** volá funkci DLError().
**/

    if (L->Act == NULL) {						// Pokud neni zadny aktivni prvek
        return;									// Ukoncim
    }
    struct tDLElem *New;						// Vytvoreni pomocne struktury
    if ((New = malloc(sizeof(struct tDLElem))) == NULL) {	// Overeni, zda-li je volne misto pro vytvoreni
        DLError();								// Vyhodim chybovou hlasku
        return;				                    // Ukonceni
    }

    New->data = val;							// Ulozeni hodnoty
    New->rptr = L->Act;							// Provaz novy prvek s aktualnim prvkem
    New->lptr = NULL;							// Ukazatel na predchozi alokuji

    if (L->Act->lptr != NULL) {					// Pokud neni aktivni prvek prvnim prvkem
        L->Act->lptr->rptr = New;				// Provedu vlozeni a vzajemne provazani prvku
        New->lptr = L->Act->lptr;
    }
    else {
        L->First = New;							// Novy prvek je prvnim prvkem seznamu
	}

    L->Act->lptr = New;
    New->rptr = L->Act;                         // Provazani
	return;
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostøednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/

	if (L->Act == NULL) {	    // Overeni, zda je seznam aktivni
		DLError();				// Vyhodim chybovou hlasku
		return;                 // Ukoncim
	}
	*val = L->Act->data;		// Prirazeni hodnoty prvku
	return;	                    // Ukoncim
}

void DLActualize (tDLList *L, int val) {
/*
** Pøepí¹e obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedìlá nic.
**/

	if (L->Act == NULL) {		// Overeni, zda je seznam aktivni
		return;					// Ukoncim
	}
	L->Act->data = val;			// Prepisu obsah
	return;                     // Ukoncim
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na posledním prvku se seznam stane neaktivním.
**/

	if (L->Act == NULL) {		// Overeni, zda je seznam aktivni
		return;					// Ukoncim
	}
	if (L->Act != L->Last)
        L->Act = L->Act->rptr;      // Posunu aktivitu
	else
        L->Act = NULL;
	return;                     // Ukoncim
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na pøedchozí prvek seznamu L.
** Není-li seznam aktivní, nedìlá nic.
** V¹imnìte si, ¾e pøi aktivitì na prvním prvku se seznam stane neaktivním.
**/

	if (L->Act == NULL) {		// Overeni, zda je seznam aktivni
		return;					// Ukoncim
	}
    if (L->Act != L->First)
        L->Act = L->Act->lptr;      // Posunu aktivitu
	else
        L->Act = NULL;
	return;                     // Ukoncim
}

int DLActive (tDLList *L) {
/*
** Je-li seznam aktivní, vrací true. V opaèném pøípadì vrací false.
** Funkci implementujte jako jediný pøíkaz.
**/

    return (L->Act != NULL);
}

/* Konec c206.c*/
