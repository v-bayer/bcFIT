package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.panels.JoinSelectKryglPanel;
import ija.labyrinth.panels.SelectKryglPanel;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * reaguje na vyber kryglu pri join do hry
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class JoinSelectKryglActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        // Vybrani figurky a odeslani dat serveru
        if(e.getSource() == JoinSelectKryglPanel.getKrygl(0)){
            JoinSelectKryglPanel.setSelectedKrygl(0);
            //System.out.println("0");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(1)){
            JoinSelectKryglPanel.setSelectedKrygl(1);
            //System.out.println("1");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(2)){
            JoinSelectKryglPanel.setSelectedKrygl(2);
            //System.out.println("2");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(3)){
            JoinSelectKryglPanel.setSelectedKrygl(3);
            //System.out.println("3");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(4)){
            JoinSelectKryglPanel.setSelectedKrygl(4);
            //System.out.println("4");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(5)){
            JoinSelectKryglPanel.setSelectedKrygl(5);
            //System.out.println("5");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(6)){
            JoinSelectKryglPanel.setSelectedKrygl(6);
            //System.out.println("6");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(7)){
            JoinSelectKryglPanel.setSelectedKrygl(7);
            //System.out.println("7");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(8)){
            JoinSelectKryglPanel.setSelectedKrygl(8);
            //System.out.println("8");
        }
        else if(e.getSource() == JoinSelectKryglPanel.getKrygl(9)){
            JoinSelectKryglPanel.setSelectedKrygl(9);
            //System.out.println("9");
        }

        SetConnection.joinGameSelectKrygl();
    }
}
