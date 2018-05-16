package ija.labyrinth.buttons;

import ija.labyrinth.listeners.SelectPlayersActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko reprezentujici tlacitka pro vyber poctu hracu
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class SelectPlayersButton extends JButton{
    /**
     * konstruktor
     * @param text      = textova hodnota tlacitka - pouzivame pro rozliseni tlacitek
     * @param locationY = Y-ove umisteni tlacitka. X-ova neni treba je stejna
     */
    public SelectPlayersButton(String text, int locationY){
        super(text);
        // pokud je hodnota textu zpet, chceme vykreslit ramecek
        if (text != "zpet"){
            setBorder(null);
            setBorderPainted(false);
        }
        setFocusPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        setForeground(new Color(240, 138, 29, 0));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(75, locationY, 275, 38);
        setHorizontalAlignment(SwingConstants.LEFT);

        addActionListener(new SelectPlayersActionListener());
    }
}
