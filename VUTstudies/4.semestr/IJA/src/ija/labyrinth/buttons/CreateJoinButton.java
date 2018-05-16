package ija.labyrinth.buttons;

import ija.labyrinth.listeners.CreateJoinListener;

import javax.swing.*;
import java.awt.*;

/**
 * Buttonek pro vytvoreni/pripojeni se ke hre
 * Created by xjehla00, xbayer05 on 10. 5. 2015.
 */
public class CreateJoinButton extends JButton {
    /**
     * konstruktor
     * @param text      = text tlacitka
     * @param locationX = X-ova souradnice
     * @param locationY = Y-ova souradnice
     * @param width     = sirka
     * @param height    = vyska
     */
    public CreateJoinButton(String text, int locationX, int locationY, int width, int height){
        super(text);
        // Zpruhlednime tlacitko
        if (text != ""){
            setBorder(null);
            setBorderPainted(false);
        }
        setFocusPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        // Zpruhlednime text tlacitka
        setForeground(new Color(240, 138, 29, 255));
        setFont(new Font("Calibri", Font.BOLD, 30));

        // Nastavime kurzor, velikost a umisteni tlacitka
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(locationX, locationY, width, height);
        //setBounds(86, locationY, 200, 35);

        // Nastavime umisteni obsahu
        setHorizontalAlignment(SwingConstants.LEFT);

        // Nastavime, jaky se ma pouzit Listener
        addActionListener(new CreateJoinListener());
    }
}
