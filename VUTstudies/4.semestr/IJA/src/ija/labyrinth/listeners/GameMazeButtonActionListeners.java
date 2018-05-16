package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.panels.GamePanel;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener reagujici na kliknuti na Maze
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class GameMazeButtonActionListeners implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        //GamePanel.setMoveTurn(false);
        // TODO: Nastavit ze serveru aktivniho hrace
        //GamePanel.setActiveButtons();
        for (int i = 0; i < GamePanel.getGameSize() * GamePanel.getGameSize(); i++){
            if (e.getSource() == GamePanel.getGameMazeButton(i)){
                // odeslani souradnic kliku serveru
                //System.out.println(((int)(i / GamePanel.getGameSize())) + " " + (i % GamePanel.getGameSize()));
                SetConnection.turn(0, ((int)(i / GamePanel.getGameSize())), (i % GamePanel.getGameSize()));
            }
        }
    }
}
