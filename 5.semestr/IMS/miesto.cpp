/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: miesto.cpp
 * popis:
 *
 *    Obsahom suboru je implementacia funkcii definovanych v subore miesto.h. 
 *
 */

//vlozenie hlavickoveho suboru
#include "miesto.h"
//konstruktor pre novu instanciu triedy Miesto
//parametre:
//			- id: identifikator miesta
//			- kapacita: kapacita miesta
//			- pocetZnaciek: vychodzi pocet znaciek
Miesto::Miesto(std::string id, int kapacita, int pocetZnaciek)
{
	this->id = id;
	this->kapacita = kapacita;
	this->pocetZnaciek = pocetZnaciek;
	max = pocetZnaciek;
}

//funkcia vrati kapacitu miesta
int Miesto::GetKap()
{
	return kapacita;
}

//funkcia aktualizuje pocet znaciek v mieste
void Miesto::AktualizujZnacky(int pocet)
{

	int x = cakaju + pocetZnaciek;
	if (min == -1)
	 	min = x;
	if (x < min)
	 	min = x;
	if (x > max)
	 	max = x;
	pocetZnaciek += pocet;
	x = cakaju + pocetZnaciek;

	if (min == -1)
	 	min = x;
	if (x < min)
	 	min = x;
	if (x > max)
	 	max = x;

}

//aktualizuje pocet znaciek cakajucih na vykonanie - pri casovanom prechode
void Miesto::AktualizujCak(int pocet)
{
int x = cakaju + pocetZnaciek;
if (min == -1)
 	min = x;
if (x < min)
 	min = x;
if (x > max)
 	max = x;

 cakaju += pocet;
 x = cakaju + pocetZnaciek;

if (min == -1)
 	min = x;
if (x < min)
 	min = x;
if (x > max)
 	max = x;
}

//funkcia aktualizuje statistiky miesta
void Miesto::AktualizujStat()
{
	int x = cakaju + pocetZnaciek;
	if (min == -1)
	 	min = x;
	if (x < min)
	 	min = x;
	if (x > max)
	 	max = x;
	sum += x;
	aktualizovane++;
}

//funkcia vrati aktualny pocet znaciek v mieste
int Miesto::GetPocetZnaciek()
{
	return this->pocetZnaciek;
}

//funkcia vrati identifikator miesta
std::string Miesto::GetId()
{
	return this->id;
}
