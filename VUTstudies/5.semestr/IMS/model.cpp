/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: model.cpp
 * popis:
 *
 *    Obsahom suboru je implementacia funkcii definovanych v subore model.h.
 *
 */

//vlozenie hlavickovych suborov
#include "model.h"
#include "prechod.h"
#include "miesto.h"
#include "hrana.h"
#include "kalendar.h"

#include <iomanip>

 Model::Model(std::string nazov, int maxPriorita)
 {
 	this->nazov = nazov;
	this->maxPriorita = maxPriorita;	// najvyssia priorita prechodu v modele
}

//funkcia, ktoroa pridava nove miesto do modelu
void Model::PridajMiesto(Miesto *m)
{
	this->miesta.push_back(m);
}
//funkcia, ktora pridava novy prechod do modelu
void Model::PridajPrechod(Prechod *p)
{
	this->prechody.push_back(p);
}
//funkcia pridava hranu, ktora spaja prechod a miesto
//parametre:
//			- idMiesta: nazov miesta
//			- idPrechodu: nazov prechodu
//			- vstup: true ak ide o prechod vstupu, implicitne false
//			- kapacita: kapacita hrany, implicitne 1
void Model::PridajHranu(std::string idMiesta, std::string idPrechodu, bool vstup, int kapacita)
{
	Prechod *p = GetKonkretnyPrechod(idPrechodu);
	for (unsigned int j = 0; j < miesta.size(); j++)
	{
		if (miesta[j]->GetId().compare(idMiesta) == 0)
		{
			if (vstup)
				p->PridajVstupnuHranu(new Hrana(*miesta[j], *p, kapacita));
			else
				p->PridajVystupnuHranu(new Hrana(*miesta[j], *p, kapacita));

			break;
		}
	}
}

//funkcia vykresli model - idPrechodu ---1---> idMiesta
void Model::VypisModel()
{
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		prechody[i]->VypisPrepojenia();
	}
}

void Model::AktualizujStat()
{
	for (int i = miesta.size()-1; i >= 0; i--)
		miesta[i]->AktualizujStat();
}

//generator pseudonahodnych cisiel, rovnomerne rozlozenie z intervalu
//<a, b>; implicitne hodnoty: a=1, b=100
//pouziva sa pre urcenie pravdepodobnosti
int Model::RandomGenerator(int a, int b)
{
	std::random_device rand;
	std::uniform_int_distribution<int> distrib(a, b);
	std::mt19937 rng(rand());

	return distrib(rng);
}

//generator pseudonahodnych cisiel exponencialneho rozlozenia so stredom v x
double Model::GeneratorExp(double x)
{
	std::mt19937 rng((int)std::time(0) * RandomGenerator());	//ak sa generator zavola v rovnakom strojovom case generuje rovnake hodnoty	
	std::exponential_distribution<double> distrib(1 / x);		//semienko je teda vynasobene nahodnym cislom z intervalu 0 - 100

	return distrib(rng);
}

//funkcia vrati ukazatel prechod so zadanym nazvom
Prechod *Model::GetKonkretnyPrechod(std::string id)
{
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->GetId().compare(id) == 0)
			return prechody[i];
	}
	return NULL;
}

//funkcia, ktora vykona prioritny prechod
bool Model::VykonajPrioritne(int priorita)
{
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->GetPrior() == priorita && prechody[i]->JeVykonatelny())
		{
			prechody[i]->SpracujZnacky();
			return true;	//vracia true po tom co ho vykona
		}
	}
	return false;	//false ak neexistuje vykonatelny prechod so zadanou prioritou 
}

//funkcia, ktora prejde postupne cez vsetky prioritne prechody
//s prioritou od najvacsej po najmensiu, ak narazi na nejaky z nizsou prioritou
//zacne od znova prechodom s najvyssou prioritou - aby nedoslo k tomu ze prechod
//s nizsou prioritou sa vykona pred tym s vyssou
void Model::SpracujPrioritne()
{
	int maxPrior = maxPriorita;
	while (maxPrior > 0)
	{
		if (VykonajPrioritne(maxPrior))
		{
			maxPrior = maxPriorita;
		}
		else
			maxPrior--;
	}
}

//funkcia, ktora spracuje vsetky okamzite prechody s implicitnou prioritou (0)
//ak na nejaky taky narazy, hned sa kontroluju prioritne, ide sa od zaciatku
void Model::SpracujOkamzite()
{
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->GetTyp() == OKAMZITY && prechody[i]->JeVykonatelny())
		{
			prechody[i]->SpracujZnacky();
			//std::cout << prechody[i]->GetId().c_str() << std::endl;
			i = 0;
			SpracujPrioritne();
		}
	}
}

//funkcia, ktora spracuje prechody s pravdepodobnostou
void Model::SpracujPravdepod()
{
	std::vector<Prechod *> p;	//vektor, do ktoreho si ulozim vsetky prechody s rovnakym ID
	int priorID;
	int a = 100;
	int b;
	int pravdepodobnost;
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->GetTyp() == PRAVDEPO && prechody[i]->JeVykonatelny())
		{
			pravdepodobnost = RandomGenerator();	//vygenerujem cislo s rovnomernou pravdepodobnostou
			priorID = prechody[i]->GetIdPrav();		//ziskam id pravdepodobnosti prechodu
			p.push_back(prechody[i]);	
			while (i < prechody.size() && prechody[i]->JeVykonatelny())	//najdem si vsetky prechody s tymto pravdep_ID
			{
				if (prechody[i]->GetIdPrav() == priorID)	//a ulozim do vektoru
					p.push_back(prechody[i]);
				i++;
			}
			for (unsigned int j = 0; j < p.size(); j++)	//prejdem cez vsetky prechody v ramci jedneho ID
			{
				b = a - p.back()->GetPravdepod();	//vypocitam si prvy interval, do ktoreho sa musi nahodne cislo trafit
				if (pravdepodobnost <= a && pravdepodobnost > b)	//aby sa vykonal tento prechod
				{
					p.back()->SpracujZnacky();
					p.clear();
				}
				else	//inak si nastavym novy interval
				{
					a = b;
					p.pop_back();	//a vymazem prechod z vektoru
				}
			}
			i = 0;	//po spracovani prechodov s rovnakym ID, sa prejdu vsetky od zaciatku
			SpracujPrioritne();		//spracuju sa prioritne, lebo sa zmenil stav systemu
		}
	}
}

//funkcia, ktora aktualizuje stav systemu
//kontroluje vsetky vykonatelne prechody a vykonava ich => meni stav systemu
void Model::AktualizujStavSystemu()
{
	while (VykonatelnyNecasovanyPrechod())	//kym je v systeme vykonatelny necasovany prechod
	{
		SpracujPrioritne();	//vykonavaj prioritne
		SpracujOkamzite();	//okamzite
		SpracujPravdepod();	//a pravdepodobnostne
	}
}

//funkcia, ktora zistuje ci sa v systeme nachadza necasovany prechod, ktory je mozne vykonat
bool Model::VykonatelnyNecasovanyPrechod()
{
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->JeVykonatelny() && prechody[i]->GetTyp() <= 2)
		{
			//std::cout << prechody[i]->GetId().c_str() << std::endl;
			return true;
		}
	}
	return false;
}

//funkcia, ktora planuje casovane prechody do kalendara
void Model::NaplanujUdalosti(Kalendar *k)
{
	double cas;
	int pocetVykonani;
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		pocetVykonani = prechody[i]->JeVykonatelny();
		if (prechody[i]->GetTyp() == CAS_GENE && pocetVykonani && !prechody[i]->GetVstup())
		{
			for (int j = pocetVykonani; j > 0; j--)		//naplanujem ho tolko krat, kolko krat je mozne ho vykonat - na zaklade
			{											//poctu znaciek na vstupe/vystupe
				cas = GeneratorExp(prechody[i]->GetCas());
				if (cas <= k->GetTime())	
					prechody[i]->SetStat(cas);	//zapis statistiky prechodu
				k->VlozUdalost(new Udalost((k->GetTime() + cas), *prechody[i]));	//vlozenie novej udalosti do kalendara
				prechody[i]->NaplanujZnacky();	
			}
		}

		else if (prechody[i]->GetTyp() == CAS_KONS && pocetVykonani && !prechody[i]->GetVstup())	//
		{
			for (int j = pocetVykonani; j > 0; j--)
			{
				if (cas <= k->GetTime())	
					prechody[i]->SetStat(cas);	//zapis statistiky prechodu
				k->VlozUdalost(new Udalost((k->GetTime() + prechody[i]->GetCas()), *prechody[i]));
				prechody[i]->NaplanujZnacky();
			}
		}
	}
}

//sablony pre vypis polozky do tabulky
template<typename T> void printElement(T t, const int& width)
{
    //cout << left << setw(width) << setfill(separator) << t;
	std::cout << std::left << std::setw(width) << std::setfill(' ') << t;
}

template<typename T> void printDElement(T t, const int& width)
{
	std::cout.precision(2);
	std::cout << std::left << std::setw(width) << std::setfill(' ') << std::fixed << t;
}

//funkcia pre vypis priebeznych statistyk => pocet znaciek v miestach
//										  => pocet vykonani prechodov
void Model::VypisStav(double simTime)
{
	const int nameWidth     = 12;
	const int numWidth      = 7;
	double avg;
	char c = '-';
	std::cout << std::endl;
	//std::cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"; 
	std::cout << "Priebezna statistika v case: " << simTime << std::endl; 

	std::cout << std::endl << "---> MIESTA (pocet znaciek)\n";
	std::cout << std::setw((3+nameWidth+numWidth*2)) << std::setfill('_') << "_" << std::endl	
	<< "| ";
	printElement("nazov ", nameWidth);
	printElement("act ", numWidth);
	printElement("avg ", numWidth);

	std::cout << std::left << "|" << std::endl
	<< std::left << std::setw((1+nameWidth+numWidth*2)) << std::setfill('-') << "|-"
	<< std::left << "-|";
	std::cout << std::endl;

	for (unsigned int i = 0; i < miesta.size(); i++)
	{
		if(miesta[i]->aktualizovane != 0)
			avg = (double)miesta[i]->sum / miesta[i]->aktualizovane;
		else
			avg = 0;	

		if(miesta[i]->GetPocetZnaciek() > 0 || avg > 0)
		{
			std::cout << "| ";
			printElement(miesta[i]->GetId().c_str(), nameWidth);
			printElement(miesta[i]->GetPocetZnaciek(), numWidth);
			if (avg != 0)
				printDElement(avg, numWidth-1);
			else
				printElement(c, numWidth-1);

			std::cout << " |" << std::endl;
		}
	}			  
	std::cout << std::left << std::setw((1+nameWidth+numWidth*2)) << std::setfill('_') << "|_"
	<< std::left << "_|";
	std::cout << std::endl;

	std::cout << std::endl << "---> PRECHODY\n";
	std::cout << std::setw((3+nameWidth+numWidth*2)) << std::setfill('_') << "_" << std::endl	
	<< "| ";
	printElement("nazov ", nameWidth);
	printElement("count ", numWidth);
	printElement("avg ", numWidth);
	std::cout << std::left << "|" << std::endl
	<< std::left << std::setw((1+nameWidth+numWidth*2)) << std::setfill('-') << "|-"
	<< std::left << "-|";
	std::cout << std::endl;
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if(prechody[i]->pocitadloVykonani != 0)
		{
			avg = prechody[i]->casSum / prechody[i]->pocitadloVykonani;
			std::cout << "| ";
			printElement(prechody[i]->GetId().c_str(), nameWidth);
			printElement(prechody[i]->pocitadloVykonani, numWidth);
			if (avg > 0.001)
				printDElement(avg, numWidth-1);
			else
				printElement(c, numWidth-1);
			std::cout << " |"<< std::endl;
		}
	}
	std::cout << std::left << std::setw(1+nameWidth+numWidth*2) << std::setfill('_') << "|_"
	<< std::left << "_|";
	std::cout << std::endl;
	std::cout << "--------------------------------------------------------------\n"; 
}

void Model::VypisPriebeznyStav(double endTime, double simTime)
{
	double printTime = endTime / 5;
	if(simTime >= (printTime / 2) && statistika == 0)
	{
		VypisStav(simTime);
		statistika++;
	}
	else if(simTime >= printTime && statistika == 1)
	{
		VypisStav(simTime);
		statistika++;	
	}
	else if(simTime >= (2 * printTime) && statistika == 2)
	{
		VypisStav(simTime);
		statistika++;	
	}
	else if(simTime >= (3 * printTime) && statistika == 3)
	{
		VypisStav(simTime);
		statistika++;	
	}
	else if(simTime >= (4 * printTime) && statistika == 4)
	{
		VypisStav(simTime);
		statistika++;	
	}
	else if(simTime >= (5 * printTime) && statistika == 5)
	{
		VypisStav(simTime);
		statistika++;	
	}
}

//Vypis celkovej statistiky
void Model::VypisStatistiky(double cas)
{
	const int nameWidth     = 12;
	const int numWidth      = 8;
	double priemernaDoba;

	std::cout << "\n\n************* Celkove statistiky *************\n";

	std::cout << std::setw(32) << std::setfill('_') << "_" << std::endl
	<< "| ";
	printElement("model: ", 7);
	printElement(nazov.c_str(), 22);
	std::cout << std::left << "|" << std::endl
	<< std::left << std::setw(30) << std::setfill('-') << "|-"
	<< std::left << "-|";
	std::cout << std::endl << "| ";
	printElement("casovy interval: 0 - ", 21);
	
	printElement(cas, 7);
	std::cout << " |" << std::endl;
	std::cout << std::left << std::setw(30) << std::setfill('_') << "|_"
	<< std::left << "_|\n";
	std::cout << std::endl;

	std::cout << "\n------> MIESTA (pocet znaciek)\n";
	std::cout << std::setw((3+nameWidth+numWidth*4)) << std::setfill('_') << "_" << std::endl	
	<< "| ";
	printElement("nazov ", nameWidth);
	printElement("max ", numWidth);
	printElement("min ", numWidth);
	printElement("act ", numWidth);
	printElement("avg ", numWidth);
	std::cout << std::left << "|" << std::endl
	<< std::left << std::setw((1+nameWidth+numWidth*4)) << std::setfill('-') << "|-"
	<< std::left << "-|";
	std::cout << std::endl;

	for (unsigned int i = 0; i < miesta.size(); i++)
	{
		if(miesta[i]->aktualizovane != 0)
			priemernaDoba = (double)miesta[i]->sum / miesta[i]->aktualizovane;
		else
			priemernaDoba = 0;

		std::cout << "| ";
		printElement(miesta[i]->GetId().c_str(), nameWidth);
		printElement(miesta[i]->max, numWidth);
		printElement(miesta[i]->min, numWidth);
		printElement(miesta[i]->GetPocetZnaciek(), numWidth);
		printDElement(priemernaDoba, numWidth-1);

		std::cout << " |" << std::endl;
	}			  
	std::cout << std::left << std::setw((1+nameWidth+numWidth*4)) << std::setfill('_') << "|_"
	<< std::left << "_|";
	std::cout << std::endl;

	std::cout << std::endl << "\n------> PRECHODY - S GENEROVANYM CASOM\n";
	std::cout << std::setw((3+nameWidth+numWidth*4)) << std::setfill('_') << "_" << std::endl	
	<< "| ";
	printElement("nazov ", nameWidth);
	printElement("count ", numWidth);
	printElement("max ", numWidth);
	printElement("min ", numWidth);
	printElement("avg ", numWidth);
	std::cout << std::left << "|" << std::endl
	<< std::left << std::setw((1+nameWidth+numWidth*4)) << std::setfill('-') << "|-"
	<< std::left << "-|";
	std::cout << std::endl;

	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->GetTyp() == CAS_GENE)
		{
			if(prechody[i]->pocitadloVykonani != 0)
				priemernaDoba = prechody[i]->casSum / prechody[i]->pocitadloVykonani;
			else
				priemernaDoba = 0;

			std::cout << "| ";
			printElement(prechody[i]->GetId().c_str(), nameWidth);
			printElement(prechody[i]->pocitadloVykonani, numWidth);
			printElement(prechody[i]->casMax, numWidth);
			printElement(prechody[i]->casMin, numWidth);
			printDElement(priemernaDoba, numWidth-1);
			std::cout << " |"<< std::endl;
		}
	}
	std::cout << std::left << std::setw(1+nameWidth+numWidth*4) << std::setfill('_') << "|_"
	<< std::left << "_|";
	std::cout << std::endl;


	std::cout << std::endl << "\n------> PRECHODY - OSTATNE\n";
	std::cout << std::setw((3+nameWidth+numWidth)) << std::setfill('_') << "_" << std::endl	
	<< "| ";
	printElement("nazov ", nameWidth);
	printElement("count ", numWidth);
	std::cout << std::left << "|" << std::endl
	<< std::left << std::setw((1+nameWidth+numWidth)) << std::setfill('-') << "|-"
	<< std::left << "-|";
	std::cout << std::endl;
	for (unsigned int i = 0; i < prechody.size(); i++)
	{
		if (prechody[i]->GetTyp() != CAS_GENE)
		{
			std::cout << "| ";
			printElement(prechody[i]->GetId().c_str(), nameWidth);
			printElement(prechody[i]->pocitadloVykonani, numWidth-1);
			std::cout << " |"<< std::endl;
		}
	}
	std::cout << std::left << std::setw(1+nameWidth+numWidth) << std::setfill('_') << "|_"
	<< std::left << "_|";
	std::cout << std::endl;
}
