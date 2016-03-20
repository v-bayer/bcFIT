#ifndef MODEL_H
#define MODEL_H

/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: model.h
 * popis:
 *
 *    Obsahom suboru je vytvorenie triedy Model, definicia clenskych 
 * premennych a funkcii. 
 *
 */

//vlozenie hlavickoveho suboru
#include "headers.h"

class Prechod;
class Miesto;
class Hrana;
class Kalendar;

//vytvorenie triedy Model
class Model
{
	int maxPriorita;	//premenna, ktora uchovava informaciu o najvyssej priorite prechodu v systeme
	int statistika = 0;	//riadi vypis priebeznej statistiky
	std::vector<Prechod*> prechody;		//vektor ukazatelov na vsetky prechody v systeme
	std::vector<Miesto *> miesta;		//vektor ukazatelov na vsetky miesta v systeme
	std::string nazov;
public:
	Model(std::string, int);	// max priorita
	//funkcie, ktore sluzia pre vytvorenie modelu
	void PridajPrechod(Prechod *);
	void PridajMiesto(Miesto *);
	void PridajHranu(std::string, std::string, bool vstup = false, int kapacita = 1);
	//funkcie riadiace operacie/udalosti v systeme
	void AktualizujStavSystemu();
	void SpracujOkamzite();
	void SpracujPravdepod();
	void SpracujPrioritne();
	bool VykonajPrioritne(int);
	void NaplanujUdalosti(Kalendar *);
	//fukncie pre vypis informacii
	void VypisModel();
	void VypisPriebeznyStav(double, double);
	void VypisStav(double);
	void VypisStatistiky(double);
	//generatory nahodnych cisiel
	int RandomGenerator(int a = 1, int b = 100);
	double GeneratorExp(double);
	void AktualizujStat();
	bool VykonatelnyNecasovanyPrechod();	//vrati true ak je v systeme vykonatelny necasovany prechod
	Prechod *GetKonkretnyPrechod(std::string);	//vrati ukazatel na prechod so zadanym nazvom
};

#endif