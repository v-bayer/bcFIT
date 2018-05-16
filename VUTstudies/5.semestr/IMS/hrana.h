#ifndef HRANA_H
#define HRANA_H

/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: hrana.h
 * popis:
 *
 *    Obsahom suboru je vytvorenie triedy Hrana, definicia clenskych 
 * premennych a funkcii. 
 *
 */

//vlozenie hlavickoveho suboru
#include "headers.h"

class Miesto;
class Prechod;

//Vytvorenie triedy Hrana
class Hrana
{
	int kapacita;	//kapacita hrany
	Miesto &miesto;		//referencia na miesto
	Prechod &prechod;	//referencia na prechod
public:
	//definicia funkcii
	Hrana(Miesto &, Prechod &, int);	//konstruktor
	int GetKap();	//vrati kapacitu hrany
	int PocetZnaciek();		//vrati pocet znaciek v mieste od/do ktoreho smeruje
	void AktualizujPocet(int pocet, bool planovany = false); //implicitna hodnota false - necasovany prechod
	void VypisMiesto();		//vypise nazov miesta
	Miesto *GetMiesto();	//vrati ukazatel na miesto
};

#endif
