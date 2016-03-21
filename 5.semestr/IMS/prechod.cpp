/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: prechod.cpp
 * popis:
 *
 *    Obsahom suboru je implementacia funkcii definovanych v subore prechod.h.
 *
 */

//vlozenie hlavickovych suborov
#include "prechod.h"
#include "hrana.h"
#include "miesto.h"

//konstruktory pre vytvorenie novej instancie triedy Prechod
/* nazov, typ = okamzity prechod, priorita = 0
 *			  + pravdepodob, pravdepodob_id = prechod s pravdepodobnostou
 *			  + priorita
 *			  + cas = konstantny/generovany (rozhoduje tp)
 *			  
 */
Prechod::Prechod(std::string id, int typ)	//OKAMZITY
{
	this->id = id;
	this->typ = typ;
}
Prechod::Prechod(std::string id, int typ, int priorita)	//OKAMZITY+PRIORITA
{
	this->id = id;
	this->typ = typ;
	this->priorita = priorita;
}
Prechod::Prechod(std::string id, int typ, int pravdepod, int p_id)	//PRAVDEPODOBNOSTNY
{
	this->id = id;
	this->typ = typ;
	this->pravdepod = pravdepod;
	this->pravdep_id = p_id;
}
Prechod::Prechod(std::string id, int typ, double cas, bool vstup)	//casovany - generovany/konstantny cas
{
	this->id = id;
	this->typ = typ;
	this->cas = cas;
	this->vstup = vstup;
}

void Prechod::PridajVstupnuHranu(Hrana *hrana)
{
	this->vstupneHrany.push_back(hrana);
}

void Prechod::PridajVystupnuHranu(Hrana *hrana)
{
	this->vystupneHrany.push_back(hrana);
}


//Funkcie typu GET/SET - pristup/nastavovanie clenskych premennych
std::string Prechod::GetId()
{
	return this->id;
}

int Prechod::GetTyp()
{
	return typ;
}

int Prechod::GetPrior()
{
	return priorita;
}

double Prechod::GetCas()
{
	return cas;
}

int Prechod::GetPravdepod()
{
	return pravdepod;
}

int Prechod::GetIdPrav()
{
	return pravdep_id;
}


bool Prechod::GetVstup()
{
	return vstup;
}

void Prechod::SetStat()
{
	pocitadloVykonani++;
}

void Prechod::SetStat(double cas)
{
	casCakania = cas;
	if (casMin == 0)
		casMin = cas;
	if (casCakania > casMax)
		casMax = casCakania;
	if (casCakania < casMin)
		casMin = casCakania;
	casSum += casCakania;	//avg bude casSum/pocitadlo vykonani
}


//Funkcia zistuje ci je dany prechod mozne vykonat.
//Ak ano, navracia cele cislo, ktoreho hodnota vypoveda o tom,
//ze kolko krat je mozne prechod vykonat.
int Prechod::JeVykonatelny()
{
	int vstupMax;
	int vystupMax = 1;
	int tmp;
	int kap;
	if (vstup)	// ak je to vstupny prechod, je stale vykonatelny
		return true;

	for (unsigned int i = 0; i < vstupneHrany.size(); i++)	//cyklus cez vsetky vstupne hrany
	{
		kap = vstupneHrany[i]->GetKap();
		if (vstupneHrany[i]->PocetZnaciek() < kap)		//ak je v zdrojovom mieste menej znaciek ako je kapacita hrany
			return false;	//vrati false
		else
		{
			tmp = vstupneHrany[i]->PocetZnaciek() / kap;	//inak zistim, kolko krat je mozne presunut znacky
			if(i == 0)
				vstupMax = tmp;
			if(tmp < vstupMax)
				vstupMax = tmp;
		}
	}

	for (unsigned int i = 0; i < vystupneHrany.size(); i++)	//cyklus cez vsetky vystupne hrany
	{
		tmp = 0;
		kap = vystupneHrany[i]->GetKap();
		if (vystupneHrany[i]->GetMiesto()->GetKap() == 0)	//ak ide o miesto z nekonecnou kapacitou
		{
			if (i == 0)
				vystupMax = vstupMax;
			continue;	//pokracuje sa dalsou iteraciou
		}
		else if ((kap + vystupneHrany[i]->PocetZnaciek()) > vystupneHrany[i]->GetMiesto()->GetKap())	//ak je miesto s obmedzenou
			return false;		//pocet znaciek, ktore sa maju presunut + aktualny pocet znaciek v danom mieste sa porovnava s kapacitou
		else
		{	//zistujem, kolko krat je mozne previest prechod, kym sa nezaplni kapacita miesta
			for (int j = kap + vystupneHrany[i]->PocetZnaciek(); j <= vystupneHrany[i]->GetMiesto()->GetKap(); j += kap)
			{
				tmp++;
			}
			if (i == 0)
				vystupMax = tmp;
			if(tmp < vystupMax)
				vystupMax = tmp;
		}
	}
	//tmp = vystupMax < vstupMax ? vystupMax:vstupMax;
	//std::cout << id.c_str() << " " << tmp << std::endl;
	return vystupMax < vstupMax ? vystupMax:vstupMax;	//vraciam mensiu hodnotu
}

//Funkcia, ktora spracuje znacky v danom prechode
void Prechod::SpracujZnacky(bool planovany)
{
	pocitadloVykonani++;	//inkrementuje sa statistika prechodu
	if (!vstup)
	{
		for (unsigned int i = 0; i < vstupneHrany.size(); i++)	// prejdem vsetky vstupne hrany
		{
			if(planovany)												//ak je planovany
				vstupneHrany[i]->AktualizujPocet(-(vstupneHrany[i]->GetKap()), true);	// odcitam cakajuce znacky z miesta
			else
				vstupneHrany[i]->AktualizujPocet(-(vstupneHrany[i]->GetKap()));	// odcitam znacky z predchadzajuceho miesta

			//vstupneHrany[i]->GetMiesto()->AktualizujStat();		//aktualizacia statistiky miesta
		}
	}

	for (unsigned int i = 0; i < vystupneHrany.size(); i++)	//prechadzaju sa vystupne hrany
	{	
		vystupneHrany[i]->AktualizujPocet(vystupneHrany[i]->GetKap());	//pricitanie znaciek
		//std::cout << "++";// << std::endl;
		//vystupneHrany[i]->VypisMiesto();
		//vystupneHrany[i]->GetMiesto()->AktualizujStat();	//aktualizacia statistiky miesta
	}
}

//Funkcia, ktora presunie znacky do "fronty" cakajucich na vykonanie(zacne im tykat budik)
void Prechod::NaplanujZnacky()
{
	for (unsigned int i = 0; i < vstupneHrany.size(); i++)
	{
		vstupneHrany[i]->AktualizujPocet(-(vstupneHrany[i]->GetKap()));
		vstupneHrany[i]->AktualizujPocet(vstupneHrany[i]->GetKap(), true);
	}
}

//Funkcia pre vypis nazvu prechodu
void Prechod::VypisId()
{
	std::cout << id.c_str() << std::endl;
}

//Funkcia vykresli hrany pripojene k prechodu + ich kapacitu + miesto
void Prechod::VypisPrepojenia()
{
	for(unsigned int i = 0; i < vstupneHrany.size(); i++)
	{
		std::cout << id.c_str() << " <--" << vstupneHrany[i]->GetKap() << "--- ";
		vstupneHrany[i]->VypisMiesto();
		std::cout << std::endl;
	}

	for (unsigned int i = 0; i < vystupneHrany.size(); i++)
	{
		std::cout << id.c_str() << " ---" << vystupneHrany[i]->GetKap() << "--> ";
		vystupneHrany[i]->VypisMiesto();
		std::cout << std::endl;
	}
}

