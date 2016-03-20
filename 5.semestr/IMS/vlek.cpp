/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: kravin.cpp
 * popis:
 *
 *    Obsahom suboru je hlavny program pre spustenie modelu "KRAVIN", ktory
 *	pozostava zo spracovania volitelneho parametru, vytvorenia modelu a spustenia
 * 	simulacie. 
 *	
 * Program je mozne prelozit pomocou prilozeneho makefilu. Vystupom makefilu je
 * subor "simulator_kravin". Pri spusteni je mozne zadat volitelny parameter vo
 * formate celeho cisla, ktory predstavuje dobu behu simulacie(pocet casovych 
 * jednotiek). V pripade spustenia bez parametru bude simulacia trvat 72
 * casovych jednotiek.
 * Spustit vsetky simulacie je mozne pomocou prikazu make run.
 * Vystupom budu 2 subory s nazvom "kravin_vystup.txt" a "ucebna_vystup.txt"
 */

//vlozenie hlavickovych suborov
#include "prechod.h"
#include "hrana.h"
#include "miesto.h"
#include "kalendar.h"
#include "model.h"


int main(int argc, char **argv)
{
	int casSimulacie = 1000;
	if(argc > 1)
		casSimulacie = atoi(argv[1]);

	Model *m = new Model("VLEK", 1);	//vytvorenie modelu, hodnota 2 predstavuje najvyssiu prioritu prechodu v modele
	//vytvorenie a pridanie miest, hran a prechodov do simulatoru
	m->PridajPrechod(new Prechod("P1", CAS_GENE, 1.0, true));
	m->PridajMiesto(new Miesto("M1"));
	m->PridajHranu("M1", "P1");
	m->PridajPrechod(new Prechod("P2", CAS_GENE, 10.0, true));
	m->PridajMiesto(new Miesto("M2"));
	m->PridajHranu("M2", "P2");
	m->PridajPrechod(new Prechod("P3", OKAMZITY));
	m->PridajHranu("M1", "P3", true);
	m->PridajPrechod(new Prechod("P4", OKAM_PRIO, (int)1));
	m->PridajHranu("M2", "P4", true);
	m->PridajMiesto(new Miesto("Stanoviste", 0, 1));
	m->PridajHranu("Stanoviste", "P3", true);
	m->PridajHranu("Stanoviste", "P4", true);
	m->PridajMiesto(new Miesto("M3"));
	m->PridajHranu("M3", "P3");
	m->PridajHranu("M3", "P4");
	m->PridajPrechod(new Prechod("P5", OKAMZITY));
	m->PridajHranu("M3", "P5", true);
	m->PridajMiesto(new Miesto("Kotvy", 0, 40));
	m->PridajHranu("Kotvy", "P5", true);
	m->PridajMiesto(new Miesto("M5"));
	m->PridajHranu("M5", "P5");
	m->PridajPrechod(new Prechod("P6_10", PRAVDEPO, (int)10, 1));
	m->PridajHranu("M5", "P6_10", true);
	m->PridajHranu("M3", "P6_10");
	m->PridajMiesto(new Miesto("M6"));
	m->PridajHranu("M6", "P6_10");
	m->PridajPrechod(new Prechod("P7_90", PRAVDEPO, (int)90, 1));
	m->PridajHranu("M5", "P7_90", true);
	m->PridajHranu("Stanoviste", "P7_90");
	m->PridajMiesto(new Miesto("M7"));
	m->PridajHranu("M7", "P7_90");
	m->PridajPrechod(new Prechod("P8", CAS_KONS, 4.0));
	m->PridajHranu("M6", "P8", true);
	m->PridajMiesto(new Miesto("M8"));
	m->PridajHranu("M8", "P8");
	m->PridajPrechod(new Prechod("P9", CAS_KONS, 4.0));
	m->PridajHranu("M7", "P9", true);
	m->PridajHranu("M8", "P9");
	m->PridajMiesto(new Miesto("M_END"));
	m->PridajPrechod(new Prechod("P_END", OKAMZITY));
	m->PridajHranu("M_END", "P9");
	m->PridajHranu("M_END", "P_END", true);
	m->PridajPrechod(new Prechod("P10", CAS_KONS, 4.0));
	m->PridajHranu("M8", "P10", true);
	m->PridajHranu("Kotvy", "P10");


	//m->VypisModel();

	Kalendar *k = new Kalendar(casSimulacie, m);	//vytvorenie kalendara, parametre: simulacny cas, model
	// Udalost *u = new Udalost(0, *m->GetKonkretnyPrechod("Dozralo"));
	k->VlozUdalost(new Udalost(1.0, *m->GetKonkretnyPrechod("P1")));	//vlozenie prvej udalosti
	k->VlozUdalost(new Udalost(10.0, *m->GetKonkretnyPrechod("P2")));	//vlozenie prvej udalosti
	std::cout << "----------------------------------------";
	std::cout << "\n|-------> MODEL P/T - \"VLEK\" <-------|"  << std::endl;
	k->StartSimulacie();	//spustenie simulacie
	m->VypisStatistiky(casSimulacie);	//vypis celkovej statistiky

	return 0;
}
