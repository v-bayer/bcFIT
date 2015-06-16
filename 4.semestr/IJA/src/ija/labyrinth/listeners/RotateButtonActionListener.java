package ija.labyrinth.listeners;

import ija.labyrinth.panels.GamePanel;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener reagujici na rotaci volneho kamene
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class RotateButtonActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        // Rotace volneho kamene
        GamePanel.getFreeMazeLabel().rotate();
        GamePanel.setRotationFreeMaze((GamePanel.getRotationFreeMaze() + 1) % 4);
        //System.out.println(-1);
    }
}
