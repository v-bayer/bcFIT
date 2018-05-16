#ifndef PRECHOD_H
#define PRECHOD_H

/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: prechod.h
 * popis:
 *
 *    Obsahom suboru je vytvorenie triedy Prechod, definicia clenskych 
 * premennych a funkcii. 
 *	
 */

//vlozenie hlavickoveho suboru
#include "headers.h"

//definicia konstant 
#define OKAMZITY 0
#define OKAM_PRIO 1
#define PRAVDEPO 2
#define CAS_KONS 3
#define CAS_GENE 4

class Hrana;

//vytvorenie triedy Prechod
class Prechod
{
	//bool udalost = false;
	int priorita = 0;	//implicitna priorita
	int typ;		//typ prechodu
	int pravdepod = 0;		//pravdepodonvost - prechody s pravdepodobnostou
	int pravdep_id = 0;		//identifikacia prechodu s pravdepodobnostou
	bool vstup = false;		//true ak ide o prichod transakcie do systemu
	double cas = 0;		//pouziva sa pri casovanych prechodoch
	std::string id;		//nazov prechodu
	std::vector<Hrana *> vstupneHrany;	//vektor hran smerujucich do prechodu
	std::vector<Hrana *> vystupneHrany;	//vektor hran smerujucich z prechodu
public:
	//konstruktory
	Prechod(std::string, int);	// okamzity prechod, implicitna priorita
	Prechod(std::string, int, int);	// okamzity s prioritou
	Prechod(std::string, int, int, int);	// okamzity prechod s prevdepodobnostou
	Prechod(std::string, int, double, bool vstup = false);	// casovany - konstantny/generovany cas
	void PridajVstupnuHranu(Hrana *);	//prida vstupnu hranu
	void PridajVystupnuHranu(Hrana *);	//prida vystupnu hranu
	void SpracujZnacky(bool planovany = false);	//true - ak ide o prechod casovany, implicitne false
	void NaplanujZnacky();	//
	void VypisPrepojenia();		//vypis hran smerujucich do/z prechodu
	void SetStat(double);	//nastavy statistiky - casovany prechod
	void SetStat();		//nastavy statistiky - ostatne prechody
	void VypisId();		//vypise nazov prechodu
	int JeVykonatelny();	//vrati 0 ak prechod nie je mozne vykonat, inak kolko krat je mozne ho vykonat
	int GetTyp();		//vrati premennu typ 
	int GetPrior();		//vrati premennu priorita 
	int GetIdPrav();	//vrati premennu pravdep_id 
	int GetPravdepod();		//vrati premennu pravdepod
	bool GetVstup();	//vrati premennu vstup
	double GetCas();	//vrati premennu cas
	std::string GetId();	//vrati nazov prechodu

	//verejne premenne - statistiky
	int pocitadloVykonani = 0;
	double casCakania = 0;
	double casSum = 0;
	double casMin = 0;
	double casMax = 0;
};

#endif