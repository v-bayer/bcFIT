
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                            �pravy: Bohuslav K�ena, ��jen 2014
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu,
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek,
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku,
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu,
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem,
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu,
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/

    L->First = NULL;				// Inicializace seznamu, nadstaveni hodnot na NULL
 	L->Act = NULL;
    L->Last = NULL;
    return;
}

void DLDisposeList (tDLList *L) {
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free.
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
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/

    L->Act = L->First;                  // Aktivuji prvni prvek
    return;
}

void DLLast (tDLList *L) {
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/

    L->Act = L->Last;                  // Aktivuji posledni prvek
    return;
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
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
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
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
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
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
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
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
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
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
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
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
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
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
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/

	if (L->Act == NULL) {		// Overeni, zda je seznam aktivni
		return;					// Ukoncim
	}
	L->Act->data = val;			// Prepisu obsah
	return;                     // Ukoncim
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
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
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
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
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/

    return (L->Act != NULL);
}

/* Konec c206.c*/
