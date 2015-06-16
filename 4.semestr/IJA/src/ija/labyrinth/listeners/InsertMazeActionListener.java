package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.panels.GamePanel;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener reagujici na vlozeni kamene
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class InsertMazeActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        //GamePanel.setMoveTurn(true);
        //GamePanel.setActiveButtons();
        for (int i = 0; i < GamePanel.getGameSize() / 2; i++){
            // odeslani souradnic kliku serveru
            if (e.getSource() == GamePanel.getUpButton(i)){
                //System.out.println((GamePanel.getGameSize() - 1) + " " + (i * 2 + 1));
                SetConnection.turn(1, (GamePanel.getGameSize() - 1), (i * 2 + 1));
            }
            else if (e.getSource() == GamePanel.getRightButton(i)){
                //System.out.println((i * 2 + 1) + " " + "0");
                SetConnection.turn(1, (i * 2 + 1), 0);
            }
            else if (e.getSource() == GamePanel.getDownButton(i)){
                //System.out.println("0" + " " + (i * 2 + 1));
                SetConnection.turn(1, 0, (i * 2 + 1));
            }
            else if (e.getSource() == GamePanel.getLeftButton(i)){
                //System.out.println((i * 2 + 1) + " " + (GamePanel.getGameSize() - 1));
                SetConnection.turn(1, (i * 2 + 1), (GamePanel.getGameSize() - 1));
            }
        }
    }
}
