package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.panels.LoadGameConfirmPanel;
import ija.labyrinth.panels.SelectKryglPanel;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Created by Antonín on 21. 5. 2015.
 */
public class LoadGameConfirmActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand() == "Vybrat"){
            SetConnection.loadGame2(2, LoadGameConfirmPanel.getSelItemFromCombobox());
        }
        else if (e.getActionCommand() == ""){
            //System.out.println("13");
            play.setMainMenu();
        }
    }
}
