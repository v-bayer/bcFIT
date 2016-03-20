/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: kalendar.cpp
 * popis:
 *
 *    Obsahom suboru je implementacia funkcii definovanych v subore kalendar.h.
 *
 */

//vlozenie hlavickovych suborov
#include "kalendar.h"
#include "prechod.h"
#include "model.h"

//konstruktor pre vytvorenie novej instancie triedy Kalendar
//parametre:
//			- endTime: cas ukoncenia simulacie
//			- *model: ukazatel na vytvoreny model
Kalendar::Kalendar(double endTime, Model *model)
{
	this->endTime = endTime;
	this->model = model;
}

//funkcia, v ktorej je implementovany cely algoritmus riadenia simulacie.
void Kalendar::StartSimulacie()
{
	std::cout << "|------> Simulacny cas: 0 - " << endTime << " <------|"<< std::endl;
	std::cout << "|______________________________________|\n";
	
	Udalost *u;		//vytvorenie udalosti
	while (!q.empty())		//cyklus bezim, kym je vo fronte naplanovana nejaka udalost
	{
		u = GetUdalost();	//nacitam udalost z kalendara: GET Zt, Zb
		VymazUdalost();		//odstranim udalost z fronty

		simulacnyCas = u->GetCasVykonania();	//aktualizujem cas: time = Zt

		if (simulacnyCas >= endTime)	//ak je simulacny cas > cas ukoncenia tak simulacia konci
			break;

		if(simulacnyCas != 0 && !u->GetPrechod()->GetVstup())	//ak sa nejedna o prechod prichodu transakcie
			u->GetPrechod()->SpracujZnacky(true);	//vykona sa udalost: exec Zb
		else if (u->GetPrechod()->GetVstup())	//ak ide o vstupny prechod, ktory modeluje
			u->GetPrechod()->SpracujZnacky();	//prichod transakcie v urcitych intervaloch, tak sa vygeneruje

		if(u->GetPrechod()->GetVstup())	//ak modelujeme prichod transakcie do systemu
			NaplanujAktualnuUdalost(u);	//naplanujeme dalsi
		model->AktualizujStat();


		model->AktualizujStavSystemu();	//vykonaju sa vsetky okamzite prechody
		model->NaplanujUdalosti(this);	//naplanuju sa dalsie udalosti - casovane prechody
		model->VypisPriebeznyStav(endTime, simulacnyCas);	//vypis priebeznej statistiky
		model->AktualizujStat();
	}
}

//funkcia, ktora planuje udalost prichodu transakcie do systemu
void Kalendar::NaplanujAktualnuUdalost(Udalost *u)
{
	double cas;
	Prechod *p = u->GetPrechod();
	if (p->GetTyp() == CAS_GENE && p->JeVykonatelny())	
	{
		cas = model->GeneratorExp(p->GetCas());
		if (cas <= GetTime())
			p->SetStat(cas);
		VlozUdalost(new Udalost((simulacnyCas + model->GeneratorExp(p->GetCas())), *p));
	}
	else if (p->GetTyp() == CAS_KONS && p->JeVykonatelny())
		VlozUdalost(new Udalost((simulacnyCas + p->GetCas()), *p));
}

//Funkcie typu Get/Set
Prechod *Udalost::GetPrechod()
{
	return &prechod;
}

double Kalendar::GetTime()
{
	return simulacnyCas;
}
void Kalendar::VlozUdalost(Udalost *u)
{
	q.push(u);
}

Udalost *Kalendar::GetUdalost()
{
	return q.top();
}

double Udalost::GetCasVykonania()
{
	return casVykonania;
}

void Udalost::VypisID()
{
	prechod.VypisId();
}

void Kalendar::VymazUdalost()
{
	q.pop();
}

//Konstruktor pre triedu Udalost
Udalost::Udalost(double cas, Prechod &p) :casVykonania(cas), prechod(p){}

Udalost::~Udalost(){}
