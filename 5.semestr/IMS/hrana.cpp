/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: hrana.cpp
 * popis:
 *
 *    Obsahom suboru je implementacia funkcii definovanych v subore hrana.h. 
 *
 */

//vlozenie hlavickovych suborov
#include "hrana.h"
#include "miesto.h"
#include "prechod.h"

//konstruktor pre novu instanciu triedy Hrana
//parametre:
//			- &miesto: referencia na miesto, ktore spaja
// 			- &prechod: referencia na prechod, ktory spaja
// 			- kapatica: kapacita hrany
Hrana::Hrana(Miesto &miesto, Prechod &prechod, int kapacita):miesto(miesto), prechod(prechod)
{
	this->kapacita = kapacita;
}

//funkcia vrati kapacitu hrany
int Hrana::GetKap()
{
	return kapacita;
}

//funkcia vrati pocet znaciek v mieste, ktore prepaja
int Hrana::PocetZnaciek()
{
	return miesto.GetPocetZnaciek();
}

//aktualizuje pocet znaciek v mieste
//parametre:
//		- pocet: pocet znaciek, ktory sa pricita/odcita
//		- planovany: true ak ide o casovany prechod
void Hrana::AktualizujPocet(int pocet, bool planovany)
{
	if (planovany)
		miesto.AktualizujCak(pocet);
	else
		miesto.AktualizujZnacky(pocet);
}

//funkcia vypise ID miesta
void Hrana::VypisMiesto()
{
	std::cout << miesto.GetId().c_str() << std::endl;
}

//funkcia vrati ukazatel na miesto, ktore prepaja
Miesto *Hrana::GetMiesto()
{
	return &miesto;
}
