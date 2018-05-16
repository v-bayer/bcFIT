<?php

namespace App\Presenters;

use Nette;
use App\Model;


class HomepagePresenter extends BasePresenter
{
	private $database;

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
	}

	public function renderDefault(){
		//$this->template->zakaznik = $this->database->table('zakaznik')->where(array('username' => $this->username));
		$userData = $this->database->table('users')->get($this->username);
		$this->template->zakaznik = $userData->related('zakaznik');
		$this->template->zamestnanec = $userData->related('zamestnanec');
	    $this->template->role = $this->role;
	}



}
