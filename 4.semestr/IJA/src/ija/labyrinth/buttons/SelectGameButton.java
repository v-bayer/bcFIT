package ija.labyrinth.buttons;

import ija.labyrinth.listeners.SelectGameActionsListener;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko reprezentujici vybrani velikosti hraci plochy
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class SelectGameButton extends JButton {
    /**
     * Konstruktor tlacitka
     * @param text      = text, podle ktereho se rozlisuje, na ktere tlacitko bylo kliknuto
     * @param locationX = X-ova souradnice tlacitka
     * @param locationY = Y-ova souradnice tlacitka
     * @param width     = sirka tlacitka
     * @param height    = vyska tlacitka
     */
    public SelectGameButton(String text, int locationX, int locationY, int width, int height){
        super(text);
        //setBorder(null);
        //setBorderPainted(false);
        setFocusPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        setForeground(new Color(240, 138, 29, 0));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(locationX, locationY, width, height);
        setHorizontalAlignment(SwingConstants.LEFT);

        addActionListener(new SelectGameActionsListener());
    }
}
