package ija.labyrinth.buttons;

import javax.swing.*;
import java.awt.*;

/**
 * Buttonek slouzici k nacteni hry a nebo k vraceni se do menu
 * Created by xjehla00, xbayer05 on 14. 5. 2015.
 */
public class LoadGameButton extends JButton {
    /**
     * konstruktor
     * @param text = text tlacitka
     */
    public LoadGameButton(String text){
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
    }
}
