package ija.labyrinth.buttons;

import ija.labyrinth.listeners.RotateButtonActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko reprezentujici rotaci volneho kamene
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class RotateButton extends JButton {
    /**
     * Konstruktor.
     * Nejsou potreba parametry. Pozice i velikost je fixni.
     */
    public RotateButton(){
        super("");
        setBorder(null);
        setBorderPainted(false);
        setFocusPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        setForeground(new Color(240, 138, 29, 0));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(653, 415, 95, 25);
        setHorizontalAlignment(SwingConstants.LEFT);

        addActionListener(new RotateButtonActionListener());
    }
}
