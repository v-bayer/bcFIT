package ija.labyrinth.listeners;

import ija.labyrinth.panels.SelectPlayersPanel;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener reagujici na vyber hrace
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class SelectPlayersActionListener implements ActionListener {

    @Override
    public void actionPerformed(ActionEvent e) {
        // Vybrani hrace a poslani serveru
        if(e.getActionCommand() == "one"){
            play.setSelectKrygl();
            SelectPlayersPanel.setNumOfPlayers(2);
            //System.out.println("2");
        }
        else if(e.getActionCommand() == "two"){
            play.setSelectKrygl();
            SelectPlayersPanel.setNumOfPlayers(3);
            //System.out.println("3");
        }
        else if(e.getActionCommand() == "three"){
            play.setSelectKrygl();
            SelectPlayersPanel.setNumOfPlayers(4);
            //System.out.println("4");
        }
        else if(e.getActionCommand() == "zpet"){
            // Zobrazeni hlavniho menu
            play.setCreateJoin();
            //System.out.println("13");
        }
    }
}
