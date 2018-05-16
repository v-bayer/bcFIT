package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.labels.images.GameMazeLabel;
import ija.labyrinth.panels.GamePanel;
import ija.labyrinth.panels.SelectGamePanel;
import ija.labyrinth.panels.SelectKryglPanel;
import ija.labyrinth.panels.SelectPlayersPanel;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Set;

/**
 * Listener reagujici na vybrani hry
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class SelectGameActionsListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand() == "Zpet"){
            // Zobrazeni vyberu hracu
            play.setSelectKrygl();
            //System.out.println("13");
            return;
        }
        else if(e.getActionCommand() == "pet"){
            // Nastaveni hry na pet + odeslani dat serveru
            SelectGamePanel.setGameSize(5);
            //play.setGame(5);
            //System.out.println("5");
        }
        else if(e.getActionCommand() == "sedm"){
            // Nastaveni hry na sedm + odeslani dat serveru
            SelectGamePanel.setGameSize(7);
            //play.setGame(7);
            //System.out.println("7");
        }
        else if(e.getActionCommand() == "devet"){
            // Nastaveni hry na devet + odeslani dat serveru
            SelectGamePanel.setGameSize(9);
            //play.setGame(9);
            //System.out.println("9");
        }
        else if(e.getActionCommand() == "jedenact"){
            // Nastaveni hry na jedenact + odeslani dat serveru
            SelectGamePanel.setGameSize(11);
            //play.setGame(11);
            //System.out.println("11");
        }

        // TODO: send data to server
        SetConnection.addLine("CREATE GAME");
        SetConnection.addLine(Integer.toString(SelectPlayersPanel.getNumOfPlayers()));
        SetConnection.addLine(Integer.toString(SelectKryglPanel.getSelectedKrygl()));
        SetConnection.addLine(Integer.toString(SelectGamePanel.getGameSize()));
        SetConnection.createGame();
    }
}
