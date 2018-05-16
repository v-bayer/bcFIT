package ija.labyrinth.labels.images;

import ija.labyrinth.panels.GamePanel;

import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;

/**
 * label reprezentujici figurku
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class KryglPekineseLabel extends JLabel {
    ImageIcon icon;

    /**
     * konstruktor
     * @param player    = hrac kteremu se nastavi figurka
     * @param positionX = X-ova pozice
     * @param positionY = Y-ova pozice
     * @param widht     = sirka figurky
     * @param height    = vyska figurky
     */
    public KryglPekineseLabel(int player, int positionX, int positionY, int widht, int height){
        setBounds(positionX, positionY, widht, height);
        icon = GamePanel.getPlayerKryglIcon(player);
        Image img = icon.getImage();
        BufferedImage bi = new BufferedImage(icon.getIconWidth(), icon.getIconHeight(), BufferedImage.TYPE_INT_ARGB);
        Graphics g = bi.createGraphics();
        if (player == GamePanel.getNumberOfPlayer() - 1){
            g.drawImage(img, 0, 50 - widht / 2, widht, height, null);
        }
        else {
            g.drawImage(img, 0, 50 - widht / 2 - 25, widht, height, null);
        }
        icon = new ImageIcon(bi);
        setIcon(icon);
    }

    /**
     * nastavi pozici figurce
     * @param positionX = X-ova pozice
     * @param positionY = Y-ova pozice
     */
    public void setPosition(int positionX, int positionY){
        setLocation(positionX,positionY);
    }
}
