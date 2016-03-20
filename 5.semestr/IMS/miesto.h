#ifndef MIESTO_H
#define MIESTO_H

/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: miesto.h
 * popis:
 *
 *    Obsahom suboru je vytvorenie triedy Miesto, definicia clenskych 
 * premennych a funkcii. 
 *
 */

//vlozenie hlavickoveho suboru
#include "headers.h"

//vytvorenie triedy Miesto
class Miesto
{
	int kapacita;	//kapacita miesta
	int pocetZnaciek;	//pocet znaciek v mieste
	int cakaju = 0;		//pocet znaciek, ktore cakaju na vykonanie casovaneho prechodu
	std::string id;		//identifikator miesta
public:
	//parametre:
	//			- kapacita: 0 => nekonecna, implicitne
	//			- pocetZnaciek: 0 => implicitny pocet znaciek po vytvoreni miesta
	Miesto(std::string, int kapacita = 0, int pocetZnaciek = 0);	//konstruktor
	std::string GetId();	//vrati id
	void AktualizujZnacky(int);	
	void AktualizujCak(int);
	int GetPocetZnaciek();
	void AktualizujStat();
	int GetKap();
	//verejne premenne - statistika
	int max = 0;
	int min = -1;
	int sum = 0;
	int aktualizovane = 0;
	int act = 0;
	int sum2 = 0;
};

#endif
