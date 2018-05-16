<?php

namespace App\Presenters;

use Nette;
use App\Model;

use Nette\Application\UI\Presenter;

/**
 * Base presenter for all application presenters.
 */
abstract class BasePresenter extends Presenter
{
	private $user;
	protected $username;
	protected $role;

	public function startup(){
		parent::startup();
		$this->user = $this->getUser();
		$this->role = $this->user->getRoles()[0];
		$this->user->setExpiration('20 minutes', TRUE);
	}

	public function beforeRender()
	{
	    parent::beforeRender(); // nezapomeňte volat metodu předka, stejně jako u startup()
	    $this->username = $this->user->getIdentity()->getData()['username'];
	    $klient = array(
	    	'Home' => 'Homepage:',
	        'Volné pokoje' => 'Klient:volnepokoje',
	        'Správa rezervace' => 'Klient:rezervace',
	        'Přehled plateb' => 'Klient:platby',
	    );

	    $recepce = array(
	    	'Home' => 'Homepage:',
	        'Správa rezervací' => 'Zamestnanec:rezervace',
	        'Správa plateb' => 'Zamestnanec:platby',
	        'Správa klientů' => 'Zamestnanec:zakaznici',
	        'Správa služeb' => 'Zamestnanec:sluzby',
	    );

	    $manazer = array(
	    	'Home' => 'Homepage:',
	        'Správa rezervací' => 'Zamestnanec:rezervace',
	        'Správa plateb' => 'Zamestnanec:platby',
	        'Správa klientů' => 'Zamestnanec:zakaznici',
	        'Správa zaměstnanců' => 'Zamestnanec:zamestnanci',
	        'Správa služeb' => 'Zamestnanec:sluzby',
	        'Správa pokojů' => 'Zamestnanec:pokoje',
	    );

	    $role_func = array();
	    switch ($this->role) {
	    	case 'klient':
	    		$role_func = $klient;
	    		break;
	    	
	    	case 'recepce':
	    		$role_func = $recepce;
	    		break;

	    	case 'manazer':
	    		$role_func = $manazer;
	    		break;
	    }
	    $out = array('Odhlásit' => 'Sign:out');
	    $role_func = array_merge($role_func, $out);

	    $this->template->menuItems = $role_func;
	}

	public function handleSignOut()
	{
	    $this->getUser()->logout();
	    $this->redirect('Sign:in');
	}
}
