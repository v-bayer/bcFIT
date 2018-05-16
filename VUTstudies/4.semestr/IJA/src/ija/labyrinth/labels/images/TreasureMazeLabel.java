package ija.labyrinth.labels.images;

import javax.swing.*;

/**
 * Label reprezentujici poklady na hraci plose
 * Created by xjehla00, xbayer05 on 9. 5. 2015.
 */
public class TreasureMazeLabel extends JLabel {
    // Index pokladu
    private int treasure;

    /**
     * Funkce vracejici index pokladu
     * @return = index pokladu
     */
    public int getTreasure(){
        return treasure;
    }

    /**
     * Konstruktor
     * @param numOfTreasure = Index pokladu
     * @param locationX     = X-ova souradnice Labelu
     * @param locationY     = Y-ova souradnice Labelu
     * @param width         = sirka
     * @param height        = vyska
     */
    public TreasureMazeLabel(int numOfTreasure, int locationX, int locationY, int width, int height){
        setBounds(locationX, locationY, width, height);
        setText("");

        // Vybrani pozadi pokladu na zaklade indexu pokladu
        switch (numOfTreasure){
            case 0:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 1:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 2:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 3:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar4.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 4:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar5.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 5:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgGambrinus1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 6:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgHeineken.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 7:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKozel1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 8:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKozel2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 9:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKozel3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 10:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKrusovice.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 11:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgPardal.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 12:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgPlzen.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 13:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgRadegast.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 14:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgStarobrno1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 15:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgStarobrno2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 16:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgStarobrno3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 17:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgSviany1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 18:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgSviany2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 19:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZlatopramen1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 20:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 21:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 22:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 23:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr4.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 24:
                setIcon(null);
                break;
        }
    }
}
