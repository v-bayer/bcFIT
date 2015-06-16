package ija.labyrinth.buttons;

import ija.labyrinth.listeners.GameMenuActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Buttonky v menu
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class GameMenuButton extends JButton {
    /**
     * konstruktor
     */
    public GameMenuButton (){
        super("Menu");
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
        setBounds(1000, 608, 24, 82);

        // Nastavime umisteni obsahu
        setHorizontalAlignment(SwingConstants.LEFT);

        // Nastavime listener
        addActionListener(new GameMenuActionListener());
    }
}
