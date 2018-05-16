package ija.labyrinth.buttons;

import ija.labyrinth.listeners.GameMazeButtonActionListeners;
import ija.labyrinth.listeners.GameMenuActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Buttonky v menu ve hre
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class GameMenuSubButton extends JButton {
    /**
     * konstruktor
     * @param text      =  text tlacitka
     * @param locationY = Y-ova souradnice
     */
    public GameMenuSubButton(String text, int locationY){
        super(text);
        // Zpruhlednime tlacitko
        setBorder(null);
        setBorderPainted(false);
        setFocusPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        // Zpruhlednime text tlacitka
        setForeground(new Color(240, 138, 29, 0));
        //setFont(new Font("Calibri", Font.BOLD, 30));

        // Nastavime kurzor, velikost a umisteni tlacitka
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(415, locationY, 203, 40);

        // Nastavime, jaky se ma pouzit Listener
        addActionListener(new GameMenuActionListener());
    }
}
