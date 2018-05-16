package ija.labyrinth.listeners;

import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener reagujici na vyber polozky v hlavnim menu
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class MenuActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand() == "Nová hra"){
            // Prepnuti na vyber poctu hracu
            play.setCreateJoin();
        }
        else if(e.getActionCommand() == "Načíst hru"){
            // Prepnuti na nacteni hry
            //System.out.println(e.getActionCommand());
            play.setLoadGamePanel();
        }
        else if(e.getActionCommand() == "Nejvyšší skóre"){
            // Prepnuti na nejvyssi skore
            //System.out.println(e.getActionCommand());
            //play.setWaitingPanel();
            play.setScoreGamePanel();
        }
        else if(e.getActionCommand() == "Nastavení"){
            // Prepnuti na nastaveni
            //System.out.println(e.getActionCommand());
        }
        else if(e.getActionCommand() == "Konec"){
            // Ukonceni hry
            System.exit(0);
        }
    }
}
