package ija.labyrinth.buttons;

import ija.labyrinth.listeners.JoinButtonActionListener;
import ija.labyrinth.panels.JoinGamePanel;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko pridani se ke hre
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class JoinGameButton extends JButton {
    /**
     * konstruktor
     * @param text = text tlacitka
     */
    public JoinGameButton(String text){
        super(text);
        setBounds(38, 458, 111, 31);
        if ((text != "") && (!text.equals("Menu"))){
            setBorder(null);
            setBorderPainted(false);
            setBounds(299, 281, 43, 42);
        }
        setContentAreaFilled(false);
        setOpaque(false);
        setFocusPainted(false);
        setForeground(new Color(240, 138, 29, 0));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setHorizontalAlignment(SwingConstants.CENTER);

        // Pridani Listeneru
        addActionListener(new JoinButtonActionListener());
    }
}
