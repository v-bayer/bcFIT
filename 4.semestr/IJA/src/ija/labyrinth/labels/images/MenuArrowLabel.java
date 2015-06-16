package ija.labyrinth.labels.images;

import javax.swing.*;

/**
 * Label reprezentujici sipky vyberu v menu
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class MenuArrowLabel extends JLabel {
    /**
     * Kontruktor
     * @param positionX = X-ova pozice Labelu
     * @param positionY = Y-ova pozice Labelu
     */
    public MenuArrowLabel(int positionX, int positionY){
        setBounds(positionX, positionY, 7, 14);
        setText("");

        setIcon(new ImageIcon(new ImageIcon("lib/Obr/Triangle/TriangleMenu.png").getImage().getScaledInstance(7, 14, java.awt.Image.SCALE_SMOOTH)));
    }
}
