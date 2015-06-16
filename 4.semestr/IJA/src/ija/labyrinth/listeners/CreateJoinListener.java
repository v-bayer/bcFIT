package ija.labyrinth.listeners;

import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener obsluhujici udalost vytvoreni hry/pripojeni se ke hre
 * Created by xjehla00, xbayer05 on 10. 5. 2015.
 */
public class CreateJoinListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand() == "Vytvořit hru"){
            //System.out.println("Vytvorit");
            play.setCreateGamePanel();
        }
        else if (e.getActionCommand() == "Připojit se"){
            //System.out.println("Připojit");
            play.setJoinGame();
        }
        else if (e.getActionCommand() == ""){
            //System.out.println("Zpet");
            play.setMainMenu();
        }
    }
}
