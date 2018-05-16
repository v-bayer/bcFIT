package ija.labyrinth.labels.images;

import javax.swing.*;

/**
 * Label reprezentujici figurku
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class KryglLabel extends JLabel {
    /**
     * Konstruktor
     * @param krygl     = identifikator obrazku
     * @param locationX = X-ova pozice labelu
     * @param locationY = Y-ova pozice labelu
     * @param width     = sirka labelu
     * @param height    = vyska labelu
     */
    public KryglLabel(int krygl,int locationX, int locationY, int width, int height){
        setBounds(locationX, locationY, width, height);
        setText("");

        // Vyber obrazku figurky na zaklade promenne krygl
        switch (krygl){
            case 0:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 1:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 2:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 3:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer4.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 4:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer5.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 5:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer6.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 6:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer7.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 7:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer8.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 8:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer9.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
            case 9:
                setIcon(new ImageIcon(new ImageIcon("lib/Obr/Krigly/MazePlayer10.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
                break;
        }
    }
}
