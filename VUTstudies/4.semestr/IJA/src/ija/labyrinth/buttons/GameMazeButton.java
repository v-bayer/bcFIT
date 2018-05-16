package ija.labyrinth.buttons;

import ija.labyrinth.listeners.GameMazeButtonActionListeners;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko reprezentujici kameny na hraci plose
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class GameMazeButton extends JButton {
    /**
     * konstruktor
     * @param locationX = X-ova souradnice
     * @param locationY = Y-ova souradnice
     * @param width     = sirka
     * @param height    = vyska
     */
    public GameMazeButton(int locationX, int locationY, int width, int height){
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
        setBounds(locationX, locationY, width, height);

        // Nastavime umisteni obsahu
        setHorizontalAlignment(SwingConstants.LEFT);

        // Nastavime, jaky se ma pouzit Listener
        addActionListener(new GameMazeButtonActionListeners());
    }
}
