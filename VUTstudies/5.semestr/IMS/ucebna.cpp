/*	Projekt IMS - Simulator P/T Petriho siti
 *	Autori:	
 *		- Jakub Keleceni, xkelec00
 *		- Vaclav Bayer, xbayer05
 *	December 2015, FIT VUT v Brne
 * -------------------------------------------------
 * -------------------------------------------------
 * subor: ucebna.cpp
 * popis:
 *
 *    Obsahom suboru je hlavny program pre spustenie modelu "UCEBNA", ktory
 *	pozostava zo spracovania volitelneho parametru, vytvorenia modelu a spustenia
 * 	simulacie. 
 *	
 * Program je mozne prelozit pomocou prilozeneho makefilu. Vystupom makefilu je
 * subor "simulator_ucebna". Pri spusteni je mozne zadat volitelny parameter vo
 * formate celeho cisla, ktory predstavuje dobu behu simulacie(pocet casovych 
 * jednotiek). V pripade spustenia bez parametru bude simulacia trvat 1000
 * casovych jednotiek.
 * Spustit vsetky simulacie je mozne pomocou prikazu make run.
 * Vystupom budu 2 subory s nazvom "ucebna_vystup.txt" a "kravin_vystup.txt"
 */

//vlozenie hlavickovych suborov
#include "prechod.h"
#include "hrana.h"
#include "miesto.h"
#include "kalendar.h"
#include "model.h"


int main(int argc, char **argv)
{
	int casSimulacie = 10000;	//implicitna doba simulacie, doba predsatuje priblizne 16,5 hodiny
	if(argc > 1)
		casSimulacie = atoi(argv[1]);

	Model *m = new Model("UCEBNA", 2);	//vytvorenie modelu, hodnota 2 predstavuje najvyssiu prioritu prechodu v modele
	//vytvorenie a pridanie miest, hran a prechodov do simulatoru
	m->PridajPrechod(new Prechod("Prichod", CAS_GENE, 10.0, true));	// cas musi byt v desatinnom tvare
	m->PridajPrechod(new Prechod("Obsluha_p", OKAM_PRIO, (int)1));
	m->PridajPrechod(new Prechod("Uvolnenie", CAS_GENE, 100.0));	
	m->PridajPrechod(new Prechod("END", OKAMZITY));
	m->PridajPrechod(new Prechod("P2", OKAMZITY));
	m->PridajPrechod(new Prechod("PP60", PRAVDEPO, (int)60, 1));
	m->PridajPrechod(new Prechod("PP40", PRAVDEPO, (int)40, 1));
	m->PridajPrechod(new Prechod("Obsluha_cak", OKAM_PRIO, (int)2));
	m->PridajPrechod(new Prechod("PP80_2", PRAVDEPO, (int)80, 2));
	m->PridajPrechod(new Prechod("PP20_2", PRAVDEPO, (int)20, 2));
	m->PridajPrechod(new Prechod("VracajuSa", CAS_KONS, 30.0));
	
	m->PridajMiesto(new Miesto("M1"));
	m->PridajMiesto(new Miesto("M2"));
	m->PridajMiesto(new Miesto("M3"));
	m->PridajMiesto(new Miesto("M4"));
	m->PridajMiesto(new Miesto("PC", 0, 10));
	m->PridajMiesto(new Miesto("Cakajuci"));
	m->PridajMiesto(new Miesto("NaLinke"));
	m->PridajMiesto(new Miesto("M_END"));

	m->PridajHranu("M1", "Prichod");
	m->PridajHranu("M1", "Obsluha_p", true);	// true lebo vstupuje do prechodu
	m->PridajHranu("PC", "Obsluha_p", true);
	m->PridajHranu("NaLinke", "Obsluha_p");
	m->PridajHranu("NaLinke", "Uvolnenie", true);
	m->PridajHranu("PC", "Uvolnenie");
	m->PridajHranu("M_END", "Uvolnenie");
	m->PridajHranu("M_END", "END", true);
	m->PridajHranu("M1", "P2", true);
	m->PridajHranu("M2", "P2");
	m->PridajHranu("M2", "PP60", true);
	m->PridajHranu("M2", "PP40", true);
	m->PridajHranu("Cakajuci", "PP60");
	m->PridajHranu("M3", "PP40");
	m->PridajHranu("Cakajuci", "Obsluha_cak", true);
	m->PridajHranu("PC", "Obsluha_cak", true);
	m->PridajHranu("NaLinke", "Obsluha_cak");
	m->PridajHranu("M3", "PP80_2", true);
	m->PridajHranu("M3", "PP20_2", true);
	m->PridajHranu("M4", "PP20_2");
	m->PridajHranu("M4", "VracajuSa", true);
	m->PridajHranu("M1", "VracajuSa");
	//m->VypisModel();

	Kalendar *k = new Kalendar(casSimulacie, m);	//vytvorenie kalendara, parametre: simulacny cas, model
	//Udalost *u = new Udalost(m->GeneratorExp(10), *m->GetKonkretnyPrechod("Prichod"));	//vytvorenie prvej udalosti
	k->VlozUdalost(new Udalost(m->GeneratorExp(10), *m->GetKonkretnyPrechod("Prichod")));	//vlozenie prvej udalosti
	std::cout << "----------------------------------------";
	std::cout << "\n|-------> MODEL P/T - \"UCEBNA\" <-------|"  << std::endl;
	k->StartSimulacie();	//spustenie simulacie
	m->VypisStatistiky(casSimulacie);	//vypis celkovej statistiky

	return 0;
}