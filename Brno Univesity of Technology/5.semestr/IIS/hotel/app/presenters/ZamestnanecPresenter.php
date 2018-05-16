<?php

namespace App\Presenters;
use Nette;
use App\Model;
use Nette\Application\UI\Form;
use Nette\Security;

class ZamestnanecPresenter extends BasePresenter
{
	private $database;
	private $ID_zamestnance;
	protected $id_pobytu;
	protected $typ_registrace;

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
	    $username = $this->getUser()->getIdentity()->getData()['username'];
	    //$this->ID_zamestnance = $this->database->table('Zamestnanec')->select('cislo_zamestnance')->where("username", $username);
	    $this->ID_zamestnance = $this->database->fetch('SELECT cislo_zamestnance FROM Zamestnanec WHERE username=?', $username)->cislo_zamestnance;
	}

	public function renderRezervace(){
		$this->template->today = StrFTime("%Y%m%d %H:%M:%S", Time());
		$this->template->pobyty = $this->database->table('Pobyt');
	}

	public function handleSmazPobyt($id){
		$this->database->table('Pokoj_pobyt')->where("ID_pobytu", $id)->delete();
		$this->database->table('Pobyt_rezervace')->where("ID_pobytu", $id)->delete();
		$this->database->table('Pobyt_bez_rezervace')->where("ID_pobytu", $id)->delete();
	    $this->database->table('Pobyt')->where("ID_pobytu", $id)->delete();
	    $this->flashMessage('Rezervace byla úspěšně zrušena.');
	}

	public function actionUpravRezervaci($id){
		$post = $this->database->table('Pobyt')->get($id);
	    if (!$post) {
	        $this->error('Příspěvek nebyl nalezen');
	    }
	    $this['upravRezervaciForm']->setDefaults($post->toArray());
	}	

	public function createComponentUpravRezervaciForm(){
		$rodne_cisla = $this->database->table('Zakaznik')->fetchPairs('rodne_cislo', 'rodne_cislo');
		$pobyt = $this->database->fetch('SELECT * FROM Pobyt WHERE ID_pobytu=?', $this->getParameter('id'));

		$form = new Form;
	    $form->addText('datebegin', 'Datum začátku pobytu *:')
	    	 ->setDefaultValue($pobyt->datum_od)
             ->setAttribute('class', 'form_datetime')
             ->setRequired("Vyberte prosím datum zahájení pobytu.");

        $form->addText('dateend', 'Datum konce pobytu *:')
        	 ->setDefaultValue($pobyt->datum_do)
             ->setAttribute('class', 'form_datetime')
             ->setRequired("Vyberte prosím datum ukončení pobytu.");

        $form->addText('cena', 'Cena *:')
        	 ->setDefaultValue($pobyt->celkova_cena)
        	 ->setRequired("Zadejte prosím cenu pobytu.")
        	 ->addRule(Form::INTEGER, "Cena musí být celočíselného formátu.");

       	$form->addSelect('rodne_cislo', 'Rodné číslo *:', $rodne_cisla)
        	 ->setRequired("Zadejte prosím rodné číslo.")
        	 ->setPrompt('- Vyberte RČ -')
         	 ->addRule(Form::FILLED, 'Vyberte prosím rodné číslo.');

        $form->addSubmit('send', 'Hotovo');
        $form->onSuccess[] = array($this, 'UpravRezervaciFormSucceeded');
        return $form;
	}

	public function UpravRezervaciFormSucceeded($form, $values){
		$id = $this->getParameter('id');

		if($values->dateend <= $values->datebegin){
			$this->flashMessage('Datum ukončení pobytu nesmí předcházet nebo být ve stejný den zahájení pobytu.', 'success');
    		$this->redirect('upravrezervaci:');
    	}elseif (count($this->database->table('Zakaznik')->where("rodne_cislo", $values->rodne_cislo)) == 0) {
    		$this->flashMessage('Zakaznik s uvedeným rodným číslem se nenachází v databázi.', 'success');
    		$this->redirect('UpravRezervaci');
		}else{
			$this->database->table('Pobyt')->where(array('ID_pobytu' => $id))->update(array(
	        'rodne_cislo' => $values->rodne_cislo,
	        'datum_od' => $values->datebegin,
	        'datum_do' => $values->dateend,
	        'celkova_cena'	=>	$values->cena,
	    	));
	    	$this->flashMessage('Vami vybrany pobyt byl upraven', 'success');
    		$this->redirect('Zamestnanec:Rezervace');
		}
	}

	public function createComponentNovaRezervaceForm(){
		$rodne_cisla = $this->database->table('Zakaznik')->fetchPairs('rodne_cislo', 'rodne_cislo');

		$form = new Form;
	    $form->addText('datebegin', 'Datum začátku pobytu *:')
             ->setAttribute('class', 'form_datetime')
             ->setRequired("Vyberte prosím datum zahájení pobytu.");

        $form->addText('dateend', 'Datum konce pobytu *:')
             ->setAttribute('class', 'form_datetime')
             ->setRequired("Vyberte prosím datum ukončení pobytu.");

        $form->addText('cena', 'Cena *:')
        	 ->setRequired("Zadejte prosím cenu pobytu.")
        	 ->addRule(Form::INTEGER, "Cena musí být celočíselného formátu.");

       	$form->addSelect('rodne_cislo', 'Rodné číslo *:', $rodne_cisla)
        	 ->setRequired("Zadejte prosím rodné číslo.")
        	 ->setPrompt('- Vyberte RČ -')
         	 ->addRule(Form::FILLED, 'Vyberte prosím rodné číslo.');

        $form->addSubmit('send', 'Hotovo');
        $form->onSuccess[] = array($this, 'NovaRezervaceFormSucceeded');
        return $form;
	}

	public function NovaRezervaceFormSucceeded($form, $values){
		if($values->dateend <= $values->datebegin){
			$this->flashMessage('Datum ukončení pobytu nesmí předcházet nebo být ve stejný den zahájení pobytu.', 'success');
    		$this->redirect('upravrezervaci:');
    	}elseif (count($this->database->table('Zakaznik')->where("rodne_cislo", $values->rodne_cislo)) == 0) {
    		$this->flashMessage('Zakaznik s uvedeným rodným číslem se nenachází v databázi.', 'success');
    		$this->redirect('UpravRezervaci');
		}else{
			$this->database->table('Pobyt')->insert(array(
	        'rodne_cislo' => $values->rodne_cislo,
	        'datum_od' => $values->datebegin,
	        'datum_do' => $values->dateend,
	        'celkova_cena'	=>	$values->cena,
	    	));
	    	$this->flashMessage('Vaše rezervace byla vložena', 'success');
    		$this->redirect('Zamestnanec:Rezervace');
		}
	}

	public function renderPlatby(){
		$this->template->platby = $this->database->table('Platba');
	}

	public function handleSmazPlatbu($id){
	    $this->database->table('Platba')->where("ID_platby", $id)->delete();
	    $this->flashMessage('Platba byla úspěšně zrušena.');
	}

	public function actionUpravPlatbu($id){
		$post = $this->database->table('Platba')->get($id);
	    if (!$post) {
	        $this->error('Příspěvek nebyl nalezen');
	    }
	    $post = $post->toArray();
	    $this['upravRezervaciForm']->setDefaults($post);
	}


	public function createComponentUpravPlatbuForm(){
		$rodne_cisla = $this->database->table('Zakaznik')->fetchPairs('rodne_cislo', 'rodne_cislo');
		$pobyt = $this->database->fetch('SELECT * FROM Platba WHERE ID_platby=?', $this->getParameter('id'));

		$form = new Form;
        $form->addText('cena', 'Výše platby *:')
        	 ->setDefaultValue($pobyt->cena)
        	 ->setRequired("Zadejte prosím cenu pobytu.")
        	 ->addRule(Form::INTEGER, "Cena musí být celočíselného formátu.");

       	$form->addSelect('rodne_cislo', 'Rodné číslo *:', $rodne_cisla)
        	 ->setRequired("Zadejte prosím rodné číslo.")
        	 ->setPrompt('- Vyberte RČ -')
         	 ->addRule(Form::FILLED, 'Vyberte prosím rodné číslo.');

        $form->addSelect('typ', 'Typ platby', array(
		    'hotovost' => 'Hotovost',
		    'kartou' => 'Kartou',
		    'faktura' => 'Faktura',
		    'převodem' => 'Převodem',
			))
        	->setPrompt('- Vyberte typ platby -');

        $form->addSubmit('send', 'Hotovo');
        $form->onSuccess[] = array($this, 'UpravPlatbuFormSucceeded');
        return $form;
	}

	public function UpravPlatbuFormSucceeded($form, $values){
		$id = $this->getParameter('id');

		if (count($this->database->table('Zakaznik')->where("rodne_cislo", $values->rodne_cislo)) == 0) {
    		$this->flashMessage('Zakaznik s uvedeným rodným číslem se nenachází v databázi.', 'success');
    		$this->redirect('UpravPlatbu');
		}else{
			$this->database->table('Platba')->where(array('ID_platby' => $id))->update(array(
	        'rodne_cislo' => $values->rodne_cislo,
	        'datum' => StrFTime("%Y%m%d", Time()),
	        'cena' => $values->cena,
	        'typ_platby' =>	$values->typ,
	        'cislo_zamestnance' =>	$this->ID_zamestnance,
	    	));
	    	$this->flashMessage('Vami vybrana platba byla upravena', 'success');
    		$this->redirect('Zamestnanec:Platby');
		}
	}

	public function createComponentNovaPlatbaForm(){
		$rodne_cisla = $this->database->table('Zakaznik')->fetchPairs('rodne_cislo', 'rodne_cislo');

		$form = new Form;
        $form->addText('cena', 'Výše platby *:')
        	 ->setRequired("Zadejte prosím cenu pobytu.")
        	 ->addRule(Form::INTEGER, "Cena musí být celočíselného formátu.");

       	$form->addSelect('rodne_cislo', 'Rodné číslo *:', $rodne_cisla)
        	 ->setRequired("Zadejte prosím rodné číslo.")
        	 ->setPrompt('- Vyberte RČ -')
         	 ->addRule(Form::FILLED, 'Vyberte prosím rodné číslo.');

        $form->addSelect('typ', 'Typ platby', array(
		    'hotovost' => 'Hotovost',
		    'kartou' => 'Kartou',
		    'faktura' => 'Faktura',
		    'převodem' => 'Převodem',
			))
        	->setPrompt('- Vyberte typ platby -');

        $form->addSubmit('send', 'Hotovo');
        $form->onSuccess[] = array($this, 'NovaPlatbaFormSucceeded');
        return $form;
	}

	public function NovaPlatbaFormSucceeded($form, $values){

		if (count($this->database->table('Zakaznik')->where("rodne_cislo", $values->rodne_cislo)) == 0) {
    		$this->flashMessage('Zakaznik s uvedeným rodným číslem se nenachází v databázi.', 'success');
    		$this->redirect('UpravPlatbu');
		}else{
			$this->database->table('Platba')->insert(array(
	        'rodne_cislo' => $values->rodne_cislo,
	        'datum' => StrFTime("%Y%m%d", Time()),
	        'cena' => $values->cena,
	        'typ_platby' =>	$values->typ,
	        'cislo_zamestnance' =>	$this->ID_zamestnance,
	    	));
	    	$this->flashMessage('Byla vytvorena nova platba', 'success');
    		$this->redirect('Zamestnanec:Platby');
		}
	}

	public function renderZakaznici(){
		$this->template->Zakaznici = $this->database->table('Zakaznik');
	}

	public function handleSmazZakaznika($id){
		$username = $this->database->fetch('SELECT * FROM Zakaznik WHERE rodne_cislo=?', $id)->username;
	    $this->database->table('Zakaznik')->where("rodne_cislo", $id)->delete();
	    $this->database->table('Users')->where("username", $username)->delete();
	    $this->flashMessage('Zákaznik byl úspěšně smazán.');
	}

	public function actionUpravZakaznika($id){
		$post = $this->database->table('Zakaznik')->get($id);
	    if (!$post) {
	        $this->error('Příspěvek nebyl nalezen');
	    }
	    $post = $post->toArray();
	    $this['upravRezervaciForm']->setDefaults($post);
	}

	public function actionRegistrace($who){
		$this->template->who = $who;
	}

	public function createComponentRegistraceForm(){

		$form = new Form;
	    $form->addText('username', 'Přihlašovací jméno *:')
             ->setRequired("Zadejte prosím cenu přihlašovací jmnéno.");

        $form->addPassword('password', 'Heslo: *', 20)
                ->setOption('description', 'Alespoň 4 znaky')
                ->addRule(Form::FILLED, 'Vyplňte Vaše heslo')
                ->addRule(Form::MIN_LENGTH, 'Heslo musí mít alespoň %d znaky.', 4);
        $form->addPassword('password2', 'Heslo znovu: *', 20)
                ->addConditionOn($form['password'], Form::VALID)
                ->addRule(Form::FILLED, 'Heslo znovu')
                ->addRule(Form::EQUAL, 'Hesla se neshodují.', $form['password']);

        if($this->template->who == 'zamestnanec'){
	        if($this->user->identity->role == 'manazer'){
		        $form->addSelect('role', 'Práva uživatele: *', array(
				    'manazer' => 'Manažer',
				    'recepce' => 'Zaměstnanec',
					))
		        	->setPrompt('- Vyberte práva uživatele -');
		    }
		}else{
			$form->addSelect('role', 'Práva uživatele: *', array(
				 'klient' => 'Zákazník',
				  ))
		         ->setPrompt('- Vyberte práva uživatele -');
		}

        $form->addSubmit('send', 'Registrovat');
        $form->onSuccess[] = array($this, 'RegistraceFormSucceeded');
        return $form;
	}

	public function RegistraceFormSucceeded($form, $values){
		if(count($this->database->table('Users')->where("username", $values->username)) > 0) {
    		$this->flashMessage('Uživatel s tímto jménem již existuje, zadejte prosím jiné uživatelské jméno.', 'success');
    		$this->redirect('Registrace');
    	}

    	$heslo = Security\Passwords::hash($values->password);

		$this->database->table('Users')->insert(array(
	    'username' => $values->username,
	    'password' => $heslo,
	    'role' => $values->role,
	   	));

	    $this->flashMessage('Vaše registrace proběhla úspěšně', 'success');
	    $this->redirect('Zamestnanec:RegistraceNext', $values->username, 'nova');
    }

    public function actionRegistraceNext($username, $typ_registrace, $koho){
    	$this->template->koho = $koho;
    	$pole = array('username' => $username, 'typ_registrace' => $typ_registrace);
    	$this->template->typ_registrace = $typ_registrace;
    	$this->template->role = $this->user->identity->role;
    	$this['registraceZakaznikForm']->setDefaults($pole);
    }

    public function createComponentRegistraceZakaznikForm(){
    	$Zakaznik = array('rodne_cislo' => '', 'jmeno' => '', 'prijmeni' => '', 'ulice' => '', 
    		'popisne_cislo' => '', 'mesto' => '', 'PSC' => '');

    	if($this->getParameter('typ_registrace') == 'uprav'){
    		$Zakaznik = $this->database->fetch('SELECT * FROM Zakaznik WHERE username=?', $this->getParameter('username'));
    	}

		$form = new Form;
	    $form->addText('rodne_cislo', 'Rodné číslo *:')
	    	 ->addRule(Form::INTEGER, 'Rodné číslo musí být číslo.')
	    	 ->addRule(Form::LENGTH, 'Rodné číslo musí mít délku %d znaků', 10)
	    	 ->setDefaultValue($Zakaznik['rodne_cislo'])
             ->setRequired("Zadejte prosím správné rodné číslo.");
        $form->addText('jmeno', 'Jméno *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Jméno nesmí být delší než %d znaků', 20)
	    	 ->setDefaultValue($Zakaznik['jmeno'])
             ->setRequired("Zadejte prosím jméno.");
        $form->addText('prijmeni', 'Přijmení *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Přijmení nesmí být delší než %d znaků', 30)
	    	 ->setDefaultValue($Zakaznik['prijmeni'])
             ->setRequired("Zadejte prosím příjmení.");
        $form->addText('ulice', 'Ulice *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Název ulice nesmí být delší než %d znaků', 20)
	    	 ->setDefaultValue($Zakaznik['ulice']);
        $form->addText('popisne_cislo', 'Popisné číslo *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Popisné číslo nesmí být delší než %d znaků', 9)
	    	 ->setDefaultValue($Zakaznik['popisne_cislo'])
	    	 ->addRule(Form::INTEGER, 'Popisné číslo musí být číslo.');
        $form->addText('mesto', 'Město *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Název města nesmí být delší než %d znaků', 20)
	    	 ->setDefaultValue($Zakaznik['mesto']);
        $form->addText('PSC', 'PSČ *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Směrovací číslo nesmí být delší než %d znaků', 6)
	    	 ->setDefaultValue($Zakaznik['PSC'])
	    	 ->addRule(Form::INTEGER, 'Směrovací číslo musí být číslo.');

        $form->addSubmit('send', 'Registrovat');
        $form->onSuccess[] = array($this, 'RegistraceZakaznikFormSucceeded');
        return $form;
	}

	public function RegistraceZakaznikFormSucceeded($form, $values){
		$username = $this->getParameter('username');

		if($this->getParameter('typ_registrace') == 'nova'){
			$this->database->table('Zakaznik')->insert(array(
			'username' => $username,
		    'rodne_cislo' => $values->rodne_cislo,
		    'jmeno' => $values->jmeno,
		    'prijmeni' => $values->prijmeni,
		    'ulice' => $values->ulice,
		    'popisne_cislo' => $values->popisne_cislo,
		    'mesto' => $values->mesto,
		    'PSC' => $values->PSC,
		   	));
		}else{
			$this->database->table('Zakaznik')->where(array('username' => $username))->update(array(
			'username' => $username,
		    'rodne_cislo' => $values->rodne_cislo,
		    'jmeno' => $values->jmeno,
		    'prijmeni' => $values->prijmeni,
		    'ulice' => $values->ulice,
		    'popisne_cislo' => $values->popisne_cislo,
		    'mesto' => $values->mesto,
		    'PSC' => $values->PSC,
		   	));
		}

	    $this->flashMessage('Vaše registrace proběhla úspěšně', 'success');
	    $this->redirect('Zamestnanec:Zakaznici');
    }

    public function createComponentRegistraceZamestnanecForm(){
    	$Zamestnanec = array('jmeno' => '', 'prijmeni' => '');

    	if($this->getParameter('typ_registrace') == 'uprav'){
    		$Zamestnanec = $this->database->fetch('SELECT * FROM Zamestnanec WHERE username=?', $this->getParameter('username'));
    	}

		$form = new Form;
        $form->addText('jmeno', 'Jméno *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Jméno nesmí být delší než %d znaků', 20)
	    	 ->setDefaultValue($Zamestnanec['jmeno'])
             ->setRequired("Zadejte prosím jméno.");
        $form->addText('prijmeni', 'Přijmení *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Přijmení nesmí být delší než %d znaků', 30)
	    	 ->setDefaultValue($Zamestnanec['prijmeni'])
             ->setRequired("Zadejte prosím příjmení.");

        $form->addSubmit('send', 'Registrovat');
        $form->onSuccess[] = array($this, 'RegistraceZamestnanecFormSucceeded');
        return $form;
	}

	public function RegistraceZamestnanecFormSucceeded($form, $values){
		$username = $this->getParameter('username');

		if($this->getParameter('typ_registrace') == 'nova'){
			$this->database->table('Zamestnanec')->insert(array(
			'username' => $username,
		    'jmeno' => $values->jmeno,
		    'prijmeni' => $values->prijmeni,
		   	));
		}else{
			$this->database->table('Zamestnanec')->where(array('username' => $username))->update(array(
			'username' => $username,
		    'jmeno' => $values->jmeno,
		    'prijmeni' => $values->prijmeni,
		   	));
		}

	    $this->flashMessage('Vaše registrace proběhla úspěšně', 'success');
	    $this->redirect('Zamestnanec:Zamestnanci');
    }

    public function renderZamestnanci(){
		$this->template->Zamestnanci = $this->database->table('Zamestnanec');
	}

	public function handleSmazZamestnance($id){
		$username = $this->database->fetch('SELECT * FROM Zamestnanec WHERE cislo_zamestnance=?', $id)->username;
	    $this->database->table('Zamestnanec')->where("cislo_zamestnance", $id)->delete();
	    $this->database->table('Users')->where("username", $username)->delete();
	    $this->flashMessage('Zákaznik byl úspěšně smazán.');
	}

	public function actionUpravZamestnance($id){
		$post = $this->database->table('Zakaznik')->get($id);
	    if (!$post) {
	        $this->error('Příspěvek nebyl nalezen');
	    }
	    $post = $post->toArray();
	    $this['upravRezervaciForm']->setDefaults($post);
	}

    public function renderSluzby(){
    	$role = $this->user->identity->role;
    	$this->template->role = $role;
    	if($role == 'manazer'){
			$this->template->sluzby = $this->database->table('Sluzba');
		}else if($role == 'recepce'){
			$this->template->sluzby = $this->database->table('Sluzba')->where('cislo_zamestnance', $this->ID_zamestnance);
		}
	}

	public function handleSmazSluzbu($id){
	    $this->database->table('Sluzba')->where("ID_sluzby", $id)->delete();
	    $this->database->table('Zamestnanec_sluzba')->where("username", $username)->delete();
	    $this->flashMessage('Služba byla úspěšně smazána.');
	}

	public function actionUpravSluzbu($id, $typ_sluzby){
		$this->template->role = $this->user->identity->role;
		$pole = array('id' => $id, 'typ_sluzby' => $typ_sluzby);
	    $this['novaSluzbaForm']->setDefaults($pole);
	}

	public function createComponentNovaSluzbaForm(){
		$Sluzba = array('typ' => '', 'cena' => '', 'cislo_zamestnance' => '');

		if($this->user->identity->role == 'recepce'){
			$Zamestnanci = $this->database->fetch('SELECT * FROM Zamestnanec WHERE cislo_zamestnance=?', $this->ID_zamestnance)->prijmeni;
			$Zamestnanci = array($Zamestnanci);
		}else{
			$Zamestnanci = $this->database->table('Zamestnanec')->fetchPairs('prijmeni', 'prijmeni');
		}

    	if($this->getParameter('typ_sluzby') == 'uprav'){
    		$Sluzba = $this->database->fetch('SELECT * FROM Sluzba WHERE ID_sluzby=?', $this->getParameter('id'));
    	}

		$form = new Form;
	    $form->addText('typ', 'Název služby *:')
	    	 ->addRule(Form::MAX_LENGTH, 'Název služby nesmí přesáhnout délku %d znaků', 30)
	    	 ->setDefaultValue($Sluzba['typ'])
             ->setRequired("Zadejte prosím název služby.");
        $form->addText('cena', 'Cena *:')
	    	 ->setDefaultValue($Sluzba['cena'])
	    	 ->addRule(Form::INTEGER, 'Cena musí být číslo.')
             ->setRequired("Zadejte prosím cenu služby.");
       	$form->addSelect('prijmeni_zamestnance', 'Zodpovědný zaměstnanec:', $Zamestnanci)
        	 ->setPrompt('- Žádný -');


        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'NovaSluzbaSucceeded');
        return $form;
	}

	public function NovaSluzbaSucceeded($form, $values){
		$id = $this->getParameter('id');
		$cislo_zamestnance = $this->database->fetch('SELECT * FROM Zamestnanec WHERE prijmeni=?', $values->prijmeni_zamestnance)['cislo_zamestnance'];

		if($this->getParameter('typ_sluzby') == 'nova'){
			$this->database->table('Sluzba')->insert(array(
			'typ' => $values->typ,
		    'cena' => $values->cena,
		    'cislo_zamestnance' => $cislo_zamestnance,
		   	));
		}else{
			$this->database->table('Sluzba')->where(array('ID_sluzby' => $id))->update(array(
			'typ' => $values->typ,
		    'cena' => $values->cena,
		    'cislo_zamestnance' => $cislo_zamestnance,
		   	));
		}

	    $this->flashMessage('Údaje služby byly uloženy', 'success');
	    $this->redirect('Zamestnanec:sluzby');
    }

    public function renderPokoje(){
		$this->template->Pokoje = $this->database->table('Pokoj');
	}

	public function handleSmazPokoj($id){
	    $this->database->table('Pokoj')->where("cislo_pokoje", $id)->delete();
	    $this->database->table('Pokoj_pobyt')->where("cislo_pokoje", $id)->delete();
	    $this->flashMessage('Pokoj byl úspěšně smazána.');
	}

	public function actionUpravPokoj($id, $typ_upravy){
		$this->template->role = $this->user->identity->role;
		$pole = array('id' => $id, 'typ_upravy' => $typ_upravy);
	    $this['novyPokojForm']->setDefaults($pole);
	}

	public function createComponentNovyPokojForm(){
		$Pokoj = array('cislo_pokoje' => '', 'cena' => '', 'ID_pobytu' => '', 'popis' => '');

    	if($this->getParameter('typ_upravy') == 'uprav'){
    		$Pokoj = $this->database->fetch('SELECT * FROM Pokoj WHERE cislo_pokoje=?', $this->getParameter('id'));
    	}

		$form = new Form;
	    $form->addText('cislo_pokoje', 'Číslo pokoje *:')
	    	 ->setDefaultValue($Pokoj['cislo_pokoje'])
	    	 ->addRule(Form::INTEGER, 'Číslo pokoje musí být číslo.')
	    	 ->addRule(Form::MAX_LENGTH, 'Číslo pokoje nesmí přesáhnout délku %d znaků', 4)
             ->setRequired("Zadejte prosím číslo pokoje.");
        $form->addText('cena', 'Cena *:')
	    	 ->setDefaultValue($Pokoj['cena'])
	    	 ->addRule(Form::INTEGER, 'Cena musí být číslo.')
             ->setRequired("Zadejte prosím cenu služby.");
        $form->addTextArea('popis', 'Stručný popis:')
	    	 ->setDefaultValue($Pokoj['popis'])
	    	 ->addRule(Form::MAX_LENGTH, 'Text popisu je příliš rozshálý (max 5% znaků).', 500);
       	/*$form->addSelect('prijmeni_zamestnance', 'Zodpovědný zaměstnanec:', $Zamestnanci)
        	 ->setPrompt('- Žádný -');*/


        $form->addSubmit('send', 'Uložit');
        $form->onSuccess[] = array($this, 'NovyPokojSucceeded');
        return $form;
	}

	public function NovyPokojSucceeded($form, $values){
		$id = $this->getParameter('id');

		if($this->getParameter('typ_upravy') == 'nova'){
			$this->database->table('Pokoj')->insert(array(
			'cislo_pokoje' => $values->cislo_pokoje,
		    'cena' => $values->cena,
		    'popis' => $values->popis,
		   	));
		}else{
			$this->database->table('Pokoj')->where(array('cislo_pokoje' => $id))->update(array(
			'cislo_pokoje' => $values->cislo_pokoje,
		    'cena' => $values->cena,
		    'popis' => $values->popis,
		   	));
		}

	    $this->flashMessage('Údaje pokoje byly uloženy', 'success');
	    $this->redirect('Zamestnanec:pokoje');
    }

}
