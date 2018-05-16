<?php

namespace App\Presenters;
use Nette;
use App\Model;
use Nette\Application\UI\Form;

class KlientPresenter extends BasePresenter
{

	private $database;
	private $rodne_cislo;

	public function __construct(Nette\Database\Context $database)
	{
		$this->database = $database;
	}
	
	public function startup()
	{
		parent::startup();
		if (!$this->getUser()->isLoggedIn()) {
	        $this->redirect('Sign:in');
	    }
	    $username = $this->getUser()->getIdentity()->getData()['username'];;
	    $this->rodne_cislo = $this->database->table('Zakaznik')->select('rodne_cislo')->where("username", $username);
	}

	public function renderRezervace(){
		$this->template->today = StrFTime("%Y%m%d %H:%M:%S", Time());
		$this->template->pobyty = $this->database->table('Pobyt')->where(array('rodne_cislo' => $this->rodne_cislo));
	}

	public function renderPlatby(){
		$this->template->platby = $this->database->table('Platba')->where(array('rodne_cislo' => $this->rodne_cislo));
	}

	// TODO mazani pobytu
	public function handleSmazPobyt($id){
		$this->database->table('Pokoj_pobyt')->where("ID_pobytu", $id)->delete();
		$this->database->table('Pobyt_rezervace')->where("ID_pobytu", $id)->delete();
		$this->database->table('Pobyt_bez_rezervace')->where("ID_pobytu", $id)->delete();
	    $this->database->table('Pobyt')->where("ID_pobytu", $id)->delete();
	    $this->flashMessage('Rezervace byla úspěšně zrušena.');
	}

	public function createComponentDatePickForm(){
		$form = new Form;
	    $form->addText('datebegin', 'Datum začátku pobytu *:')
             ->setAttribute('class', 'form_datetime')
             ->setRequired("Vyberte prosím datum zahájení pobytu.");

        $form->addText('dateend', 'Datum konce pobytu *:')
             ->setAttribute('class', 'form_datetime')
             ->setRequired("Vyberte prosím datum ukončení pobytu.");

        $form->addSubmit('send', 'Hledej');
        $form->onSuccess[] = array($this, 'DatePickFormSucceeded');
        return $form;
	}

	public function DatePickFormSucceeded($form, $values){
		if($values->dateend <= $values->datebegin){
			$this->flashMessage('Datum ukončení pobytu nesmí předcházet nebo být ve stejný den zahájení pobytu.', 'success');
    		$this->redirect('this');
		}else{
    		$this->redirect('Klient:VolnePokojeTisk', $values->dateend, $values->datebegin);
		}
	}

	public function actionVolnePokojeTisk($dateend, $datebegin){
		/*$this->template->pobyty = $this->database->table('Pobyt')->where(('$datebegin' < 'datum_od' AND '$dateend' < 'datum_od') 
			OR ('$datebegin' > 'datum_do' AND '$dateend' > 'datum_do'));*/
		$this->template->zabrane_pobyty = $this->database->table('Pobyt')->where
			('(datum_od <= ? AND datum_do >= ?) OR (datum_od <= ? AND datum_do >= ?) OR (datum_od <= ? AND datum_do >= ?) OR (datum_od >= ? AND datum_do <= ?)'
				, $datebegin, $dateend, $datebegin, $datebegin, $dateend, $dateend, $datebegin, $dateend);

		$this->template->pokoje = $this->database->fetchAll('SELECT cislo_pokoje
														FROM Pokoj
														WHERE cislo_pokoje NOT IN(
															SELECT cislo_pokoje
															FROM Pokoj_pobyt
															WHERE ID_pobytu IN(
																SELECT ID_pobytu
																FROM Pobyt
																WHERE (datum_od <= ? AND datum_do >= ?) OR (datum_od <= ? AND datum_do >= ?) OR (datum_od <= ? AND datum_do >= ?) OR (datum_od >= ? AND datum_do <= ?)))
																', $datebegin, $dateend, $datebegin, $datebegin, $dateend, $dateend, $datebegin, $dateend);

		$this->template->dateend = $dateend;
		$this->template->datebegin = $datebegin;
		$this->template->Sluzby = $this->database->table('Sluzba');
		$this->template->vypis_pokoju = $this->database->table('Pokoj');
	}

	public function createComponentPobytMoznostiForm(){
		$Sluzby = $this->template->Sluzby;
		$array = $this->template->pokoje;
		$Pokoje = array();
		foreach ($array as $key => $value) {
			foreach ($value as $value2) {
			array_push($Pokoje, array($value2 => $value2));
			}
		}

		$form = new Form;
		$form->addCheckboxList('typ', 'Výběr služby: ', $Sluzby->fetchPairs('typ', 'typ'));
		$form->addSelect('pokoj', 'Výběr pokoje *: ', $Pokoje);
		$form->addSubmit('send', 'Vytvoř rezervaci');
        $form->onSuccess[] = array($this, 'PobytMoznostiFormSucceeded');
        return $form;
	}

	public function PobytMoznostiFormSucceeded($form, $values){
		$celkova_cena = 0;

		foreach ($values->typ as $hodnotaType) {
      		$celkova_cena = $celkova_cena + $this->database->fetch('SELECT * FROM Sluzba WHERE typ=?', $hodnotaType)['cena'];
		}

		foreach ($this->rodne_cislo as $key => $value) {
			$rodne_cislo = $value;
		}

		$cena_pokoje = $this->database->fetch('SELECT * FROM Pokoj WHERE cislo_pokoje=?', $values->pokoj)['cena'];
		$celkova_cena += $cena_pokoje;

	    $this->database->table('Pobyt')->insert(array(
	        'rodne_cislo' => $rodne_cislo,
	        'datum_od' => $this->template->datebegin,
	        'datum_do' => $this->template->dateend,
	        'celkova_cena' => $celkova_cena,
	    ));

		$maxid = $this->database->fetch('SELECT MAX(ID_pobytu) AS maxid FROM Pobyt')['maxid'];

	    $this->database->table('Pokoj_pobyt')->insert(array(
	        'cislo_pokoje' => $values->pokoj,
	        'ID_pobytu' => $maxid,
	    ));

	    $this->flashMessage('Vaše rezervace byla vložena', 'success');
	    $this->redirect('VolnePokoje');
	}
}
