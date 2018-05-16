package ija.labyrinth.listeners;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.frames.MainFrame;
import ija.labyrinth.panels.GamePanel;
import ija.labyrinth.panels.JoinGamePanel;
import ija.labyrinth.play;
import ija.labyrinth.textfields.JoinGameTextField;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

/**
 * reaguje na pripojenise ke hre
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class JoinButtonActionListener implements ActionListener {

    @Override
    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand() == "Připojit ke hře"){
            //System.out.println("Připojit");
            JoinGamePanel.setIpAddress();
            //System.out.println(JoinGameTextField.getIpAddress());
            SetConnection.listenSocket();
            SetConnection.joinGame();
        }
        else if (e.getActionCommand() == ""){
            //System.out.println("13");
            play.setCreateJoin();
        }
        else if (e.getActionCommand() == "Menu"){
            //System.out.println("13");
            play.setMainMenu();
        }
        else if (e.getActionCommand() == "Vytvořit hru"){
            //System.out.println("Vytvořit");
            JoinGamePanel.setIpAddress();
            //System.out.println(JoinGameTextField.getIpAddress());
            SetConnection.listenSocket();
            play.setSelectPlayers();
        }
        else if (e.getActionCommand() == "Nahrát"){
            //System.out.println("Nahrát");
            JoinGamePanel.setIpAddress();
            //System.out.println(JoinGameTextField.getIpAddress());
            SetConnection.listenSocket();
            //play.setLoadGameConfirmPanel();
            SetConnection.loadGame(1, "");
            GamePanel.setNumberOfPlayer(1);
        }
        else if (e.getActionCommand() == "Nejvyšší skóre"){
            //System.out.println("skóre");
            JoinGamePanel.setIpAddress();
            //System.out.println(JoinGameTextField.getIpAddress());
            SetConnection.listenSocket();
            play.setScorePanel();
        }
    }
}
