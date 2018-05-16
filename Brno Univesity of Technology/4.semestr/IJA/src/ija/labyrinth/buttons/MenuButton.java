package ija.labyrinth.buttons;

import ija.labyrinth.listeners.*;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko reprezentujici nabidku v menu
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class MenuButton extends JButton {
    /**
     * Konstruktor tlacitka
     * @param text      = Zobrazovany text
     * @param locationY = umisteni v Y-ove ose X-ova neni treba jsou stejne
     */
    public MenuButton(String text, int locationY){
        super(text);
        setBorder(null);
        setBorderPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        setFocusPainted(false);
        setForeground(new Color(240, 138, 29, 255));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(86, locationY, 200, 35);
        setHorizontalAlignment(SwingConstants.LEFT);
        //Image image = new ImageIcon("lib/Obr/labyrinth.jpg").getImage();
        //imageUpdate(image, 0, 0, 0, 200, 35);

        // Pridani Listeneru
        addActionListener(new MenuActionListener());
    }
}
