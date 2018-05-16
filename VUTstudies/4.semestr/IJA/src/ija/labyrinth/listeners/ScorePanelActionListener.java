package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Listener odpojujici se od score
 * Created by xjehla00, xbayer05 on 14. 5. 2015.
 */
public class ScorePanelActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if(e.getActionCommand() == "Zpet"){
            play.setMainMenu();
            SetConnection.closeConnection();
        }
    }
}
