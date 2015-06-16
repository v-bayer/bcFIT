package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.frames.MainFrame;
import ija.labyrinth.panels.GamePanel;
import ija.labyrinth.play;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * Reaguje na tlacitka v hernim menu
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class GameMenuActionListener implements ActionListener {
    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand() == "Menu"){
            GamePanel.disableInsertButtons();
            GamePanel.disableMazeButtons();
            GamePanel.disableMenuButton();
            GamePanel.disableRotateButton();
            GamePanel.setGameMenuVisible();
            //System.out.println("Menu");
        }
        else if (e.getActionCommand() == "HlavniMenu"){
            GamePanel.enableInsertButtons();
            GamePanel.enableMazeButtons();
            GamePanel.enableMenuButton();
            GamePanel.enableRotateButton();
            GamePanel.setGameMenuUnvisible();
            System.exit(0);
            play.setNewGamePanel();
            play.setMainMenu();
            //System.out.println("Hlavní menu");
            SetConnection.exitGame();
        }
        else if (e.getActionCommand() == "Ulozit"){
            //System.out.println("Uložit");
            SetConnection.saveGame();
        }
        else if (e.getActionCommand() == "Zpet"){
            GamePanel.setGameMenuUnvisible();
            GamePanel.setActiveButtons();
            //System.out.println("Zpět");
        }
    }
}
