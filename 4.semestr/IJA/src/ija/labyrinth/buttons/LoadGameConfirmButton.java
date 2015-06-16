package ija.labyrinth.buttons;

import ija.labyrinth.listeners.LoadGameConfirmActionListener;
import ija.labyrinth.panels.LoadGameConfirmPanel;

import javax.swing.*;
import java.awt.*;

/**
 * Created by Antonín on 21. 5. 2015.
 */
public class LoadGameConfirmButton extends JButton {

    /**
     * Konstruktor
     * @param text = obsah tlacitka
     */
    public LoadGameConfirmButton(String text){
        super(text);
        setBounds(38, 458, 111, 31);
        if (text != ""){
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

        addActionListener(new LoadGameConfirmActionListener());
    }
}
