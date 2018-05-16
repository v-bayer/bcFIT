package ija.labyrinth.labels.images;

import ija.labyrinth.buttons.GameMenuButton;

import javax.swing.*;

/**
 * Label reprezentujici menu ve hre
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class GameMenuLabel extends JLabel {
    /**
     * konstruktor
     */
    public GameMenuLabel(){
        setBounds(361, 125, 442, 488);
        setText("");

        setIcon(new ImageIcon(new ImageIcon("lib/Obr/UI/UI_menu.png").getImage().getScaledInstance(442, 488, java.awt.Image.SCALE_SMOOTH)));
    }
}
