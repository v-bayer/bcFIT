#ifndef KALENDAR_H
#define KALENDAR_H

/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: kalendar.h
 * popis:
 *
 *    Obsahom suboru je vytvorenie triedy Kalendar, definicia clenskych 
 * premennych a funkcii. 
 *
 */

//vlozenie hlavickoveho suboru
#include "headers.h"

class Prechod;
class Model;

//vytvorenie triedy Udalost
class Udalost
{
	double casVykonania;	//simulacny cas, v ktorom ma udalost prebehnut
	Prechod &prechod;	//referencia na prechod, v ktorom ma udalost vykonat
public:
	Udalost(double, Prechod &);	//konstruktor
	~Udalost();
	Prechod *GetPrechod();
	void VypisID();
	double GetCasVykonania();
};

//pretypovanie operatora pre priority_queue
//zoradenie zostupne podla casu vykonanie
struct QueueDesc
{
	bool operator()(Udalost *x, Udalost *y) {
		return y->GetCasVykonania() < x->GetCasVykonania();
	}
};

//trieda kalendar
class Kalendar
{
	double startTime = 0;	//cas zaciatku simulacie - vzdy 0
	double simulacnyCas = startTime;	//aktualny simulacny cas
	double endTime;		//cas ukoncenia simulacie
	Model *model;	//ukazatel na model
	std::priority_queue<Udalost*, std::vector<Udalost*>, QueueDesc> q;		//fronta udalosti zoradena podla aktivacneho casu
public:
	Kalendar(double, Model*);	//konstruktor
	void VlozUdalost(Udalost *);	//vlozenie udalosti do kalendara
	void VymazUdalost();		//odstranenie udalosti z kalendara
	void StartSimulacie();		//spustenie simulacie
	void NaplanujAktualnuUdalost(Udalost *);
	Udalost *GetUdalost();	//nacita udalost z kalendara
	double GetTime();	//vrati aktualny cas
};

#endif