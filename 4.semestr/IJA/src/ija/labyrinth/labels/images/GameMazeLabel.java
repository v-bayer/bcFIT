package ija.labyrinth.labels.images;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

/**
 * Label reprezentujici pozadi hraciho kamene
 * Created by xjehla00, xbayer05 on 9. 5. 2015.
 */
public class GameMazeLabel extends JLabel {
    // Ikona hraciho kamene
    ImageIcon[] icon = new ImageIcon[12];
    ImageIcon showedIcon;

    /**
     * Nastaveni kamene (vykresovaneho pozadi a natoceni sceny)
     * @param type      = Typ vykreslovaneho kamene
     * @param numRotate = pocet otoceni vpravo
     */
    public void setMaze(char type, int numRotate){
        // Vybrani pozadi
        switch (type){
            case 'C':
                setIcon(icon[numRotate]);
                break;
            case 'L':
                setIcon(icon[numRotate + 4]);
                break;
            case 'F':
                setIcon(icon[numRotate + 8]);
                break;
        }
    }

    /**
     * Otaci obrazek v Labelu
     */
    private void rotate(){
        int w = showedIcon.getIconWidth();
        int h = showedIcon.getIconHeight();
        int type = BufferedImage.TYPE_INT_ARGB;  // other options, see api
        BufferedImage image = new BufferedImage(h, w, type);
        Graphics2D g2 = image.createGraphics();
        double x = (h - w)/2.0;
        double y = (w - h)/2.0;
        AffineTransform at = AffineTransform.getTranslateInstance(x, y);
        at.rotate(Math.toRadians(90), w/2.0, h/2.0);
        g2.drawImage(showedIcon.getImage(), at, this);
        g2.dispose();
        showedIcon = new ImageIcon(image);

        setIcon(showedIcon);
    }

    /**
     * Konstruktor
     * @param locationX = X-ova pozice labelu
     * @param locationY = Y-ova pozice labelu
     * @param width     = sirka labelu
     * @param height    = vyska labelu
     */
    public GameMazeLabel(int locationX, int locationY, int width, int height){
        setBounds(locationX, locationY, width, height);
        setText("");

        showedIcon = new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeC.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[0] = showedIcon;
        rotate();
        icon[1] = showedIcon;
        rotate();
        icon[2] = showedIcon;
        rotate();
        icon[3] = showedIcon;
        showedIcon = new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeL.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[4] = showedIcon;
        rotate();
        icon[5] = showedIcon;
        rotate();
        icon[6] = showedIcon;
        rotate();
        icon[7] = showedIcon;
        showedIcon = new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeF.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[8] = showedIcon;
        rotate();
        icon[9] = showedIcon;
        rotate();
        icon[10] = showedIcon;
        rotate();
        icon[11] = showedIcon;
    }
}
