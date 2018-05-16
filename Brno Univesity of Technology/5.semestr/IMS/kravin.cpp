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
	int casSimulacie = 720;	//doba predsatvuje 30 dni
	if(argc > 1)
		casSimulacie = atoi(argv[1]);

	Model *m = new Model("KRAVIN", 1);	//vytvorenie modelu, hodnota 2 predstavuje najvyssiu prioritu prechodu v modele
	//vytvorenie a pridanie miest, hran a prechodov do simulatoru
	m->PridajPrechod(new Prechod("Dozralo", CAS_GENE, 15.0));	
	m->PridajPrechod(new Prechod("T1", OKAMZITY));	
	m->PridajPrechod(new Prechod("T2", PRAVDEPO, (int)10, 1));
	m->PridajPrechod(new Prechod("T3", PRAVDEPO, (int)90, 1));
	m->PridajPrechod(new Prechod("T4", CAS_GENE, 0.25));
	m->PridajPrechod(new Prechod("T5", CAS_GENE, 0.1333));
	m->PridajPrechod(new Prechod("T6", OKAMZITY));
	m->PridajPrechod(new Prechod("T7", OKAMZITY));
	m->PridajPrechod(new Prechod("T8", OKAM_PRIO, (int)1));
	m->PridajPrechod(new Prechod("T9", OKAMZITY));
	m->PridajPrechod(new Prechod("T11", OKAMZITY));
	m->PridajPrechod(new Prechod("T10", CAS_KONS, 0.0333));
	m->PridajPrechod(new Prechod("T12", CAS_KONS, 1.0));

	m->PridajMiesto(new Miesto("P0", 0, 100));
	m->PridajMiesto(new Miesto("P1"));
	m->PridajMiesto(new Miesto("Dojicky", 0, 5));
	m->PridajMiesto(new Miesto("P3"));
	m->PridajMiesto(new Miesto("P4"));
	m->PridajMiesto(new Miesto("P5"));
	m->PridajMiesto(new Miesto("Konvica"));
	m->PridajMiesto(new Miesto("P6"));
	m->PridajMiesto(new Miesto("P7"));
	m->PridajMiesto(new Miesto("Auta", 0, 2));
	m->PridajMiesto(new Miesto("Rampa", 0, 1));
	m->PridajMiesto(new Miesto("na_rampe"));
	m->PridajMiesto(new Miesto("Kapacita"));
	m->PridajMiesto(new Miesto("P8"));
	m->PridajMiesto(new Miesto("P9"));

	m->PridajHranu("P0", "Dozralo", true);
	m->PridajHranu("P1", "Dozralo");
	m->PridajHranu("P1", "T1", true);
	m->PridajHranu("Dojicky", "T1", true);
	m->PridajHranu("P3", "T1");
	m->PridajHranu("P3", "T2", true);
	m->PridajHranu("P3", "T3", true);
	m->PridajHranu("P4", "T2");
	m->PridajHranu("P5", "T3");
	m->PridajHranu("P4", "T4", true);
	m->PridajHranu("P5", "T5", true);
	m->PridajHranu("P6", "T4");
	m->PridajHranu("P6", "T5");
	m->PridajHranu("P6", "T6", true);
	m->PridajHranu("P0", "T6");
	m->PridajHranu("Dojicky", "T6");
	m->PridajHranu("Konvica", "T6");
	m->PridajHranu("Auta", "T7", true);
	m->PridajHranu("Rampa", "T7", true);
	m->PridajHranu("Kapacita", "T7", false, 20);
	m->PridajHranu("na_rampe", "T7");
	m->PridajHranu("Kapacita", "T8", true);
	m->PridajHranu("na_rampe", "T8", true);
	m->PridajHranu("P7", "T8");
	m->PridajHranu("P7", "T9", true);
	m->PridajHranu("Konvica", "T9", true);
	m->PridajHranu("P8", "T9");
	m->PridajHranu("P8", "T10", true);
	m->PridajHranu("na_rampe", "T10");
	m->PridajHranu("na_rampe", "T11", true);
	m->PridajHranu("Rampa", "T11");
	m->PridajHranu("P9", "T11");
	m->PridajHranu("P9", "T12", true);
	m->PridajHranu("Auta", "T12");
	//m->VypisModel();

	Kalendar *k = new Kalendar(casSimulacie, m);	//vytvorenie kalendara, parametre: simulacny cas, model
	// Udalost *u = new Udalost(0, *m->GetKonkretnyPrechod("Dozralo"));
	k->VlozUdalost(new Udalost(0, *m->GetKonkretnyPrechod("Dozralo")));	//vlozenie prvej udalosti
	std::cout << "----------------------------------------";
	std::cout << "\n|-------> MODEL P/T - \"KRAVIN\" <-------|"  << std::endl;
	k->StartSimulacie();	//spustenie simulacie
	m->VypisStatistiky(casSimulacie);	//vypis celkovej statistiky

	return 0;
}
