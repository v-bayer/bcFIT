package ija.labyrinth.listeners;

import ija.labyrinth.panels.SelectKryglPanel;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener reagujici na vybrani figurky
 * Created by xjehla00, xbayer05 on 7. 5. 2015.
 */
public class SelectKryglActionsListener implements ActionListener {

    @Override
    public void actionPerformed(ActionEvent e) {
        // Vybrani figurky a odeslani dat serveru
        if(e.getSource() == SelectKryglPanel.getKrygl(0)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(0);
            //System.out.println("0");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(1)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(1);
            //System.out.println("1");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(2)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(2);
            //System.out.println("2");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(3)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(3);
            //System.out.println("3");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(4)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(4);
            //System.out.println("4");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(5)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(5);
            //System.out.println("5");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(6)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(6);
            //System.out.println("6");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(7)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(7);
            //System.out.println("7");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(8)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(8);
            //System.out.println("8");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(9)){
            play.setNewGame();
            SelectKryglPanel.setSelectedKrygl(9);
            //System.out.println("9");
        }
        else if(e.getSource() == SelectKryglPanel.getKrygl(10)){
            play.setSelectPlayers();
            //System.out.println("13");
        }
    }
}
