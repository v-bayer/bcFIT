package ija.labyrinth.panels;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.labels.texts.WaitingForPlayersLabel;

import javax.swing.*;
import java.awt.*;
import java.util.Set;

/**
 * Panel reprezentujici cekani na hrace
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class WaitingForPlayersPanel extends JPanel {
    private static WaitingForPlayersLabel waitingForPlayersLabel = new WaitingForPlayersLabel();

    /**
     * Konstruktor
     */
    public WaitingForPlayersPanel(){
        setLayout(null);
        add(waitingForPlayersLabel);
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
