package ija.labyrinth.labels.images;

import ija.labyrinth.panels.GamePanel;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

/**
 * Label reprezentujici volny kamen
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class FreeMazeLabel extends JLabel {
    // Zobrazovana ikona
    static ImageIcon[] icons = new ImageIcon[3];

    ImageIcon icon;

    /**
     * nastavi vsechny mozne ikony
     */
    public static void setIcons(){
        icons[0] = new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeC.png").getImage().getScaledInstance(90, 90, java.awt.Image.SCALE_SMOOTH));
        icons[1] = new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeL.png").getImage().getScaledInstance(90, 90, java.awt.Image.SCALE_SMOOTH));
        icons[2] = new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeF.png").getImage().getScaledInstance(90, 90, java.awt.Image.SCALE_SMOOTH));
    }

    /**
     * Konstruktor
     * @param type      = urcuje, zda je volny kamen typu C, L, nebo F
     * @param numRotate = urcuje jak je natoceny volny kamen
     */
    public FreeMazeLabel(char type, int numRotate){
        setBounds(655, 445, 90, 90);
        setText("");

        setIcons();

        // vyber obrazu kamene podle typu
        switch (type){
            case 'C':
                icon = icons[0];
                break;
            case 'L':
                icon = icons[1];
                break;
            case 'F':
                icon = icons[2];
                break;
        }

        // otoceni kamene
        for (int i = 0; i < numRotate; i++){
            rotate();
        }
    }

    /**
     * prepnuti kamene na vysunuty kamen
     * @param index = index vysunuteho kamene
     */
    public void chooseMaze(int index){
        icon = icons[index];
        setIcon(icon);
    }

    /**
     * Rotace volneho kamene
     */
    public void rotate(){
        int w = icon.getIconWidth();
        int h = icon.getIconHeight();
        int type = BufferedImage.TYPE_INT_ARGB;                             // Nastaveni barevneho spektra
        BufferedImage image = new BufferedImage(h, w, type);
        Graphics2D g2 = image.createGraphics();
        double x = (h - w)/2.0;
        double y = (w - h)/2.0;
        AffineTransform at = AffineTransform.getTranslateInstance(x, y);
        at.rotate(Math.toRadians(90), w/2.0, h/2.0);
        g2.drawImage(icon.getImage(), at, this);
        g2.dispose();
        icon = new ImageIcon(image);

        setIcon(icon);
    }
}
