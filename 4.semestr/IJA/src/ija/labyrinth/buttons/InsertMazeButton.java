package ija.labyrinth.buttons;

import ija.labyrinth.listeners.InsertMazeActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Tlacitko reprezentujici sipky, ktere vsunuji volny kamen na hraci plochu
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class InsertMazeButton extends JButton {
    /**
     * Vycet smeru, kterym je natocen kamen
     */
    public enum Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT,
    }

    /**
     * Konstruktor tlacitka
     * @param smer      = natoceni sipky
     * @param locationX = X-ova souradnice sipky
     * @param locationY = Y-ova souradnice sipky
     * @param width     = sirka tlacitka
     * @param height    = vyska tlacitka
     */
    public InsertMazeButton(Direction smer, int locationX, int locationY, int width, int height){
        // Nastaveni vlastnosti tlacitka
        setBorder(null);
        setBorderPainted(false);
        setFocusPainted(false);
        setContentAreaFilled(false);
        setOpaque(false);
        setForeground(new Color(240, 138, 29, 0));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setBounds(locationX, locationY, width, height);

        // Podle smeru natoceni vybereme, kterou sipku pouzijeme
        if (smer == Direction.UP){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Triangle/TriangleUp.png").getImage().getScaledInstance(22, 19, java.awt.Image.SCALE_SMOOTH)));
            setHorizontalAlignment(SwingConstants.CENTER);
            setVerticalAlignment(SwingConstants.TOP);
        }
        else if (smer == Direction.RIGHT){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Triangle/TriangleRight.png").getImage().getScaledInstance(22, 19, java.awt.Image.SCALE_SMOOTH)));
            setHorizontalAlignment(SwingConstants.RIGHT);
            setVerticalAlignment(SwingConstants.CENTER);
        }
        else if (smer == Direction.DOWN){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Triangle/TriangleDown.png").getImage().getScaledInstance(22, 19, java.awt.Image.SCALE_SMOOTH)));
            setHorizontalAlignment(SwingConstants.CENTER);
            setVerticalAlignment(SwingConstants.BOTTOM);
        }
        else if (smer == Direction.LEFT){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Triangle/TriangleLeft.png").getImage().getScaledInstance(22, 19, java.awt.Image.SCALE_SMOOTH)));
            setHorizontalAlignment(SwingConstants.LEFT);
            setVerticalAlignment(SwingConstants.CENTER);
        }

        // Pridani Listeneru
        addActionListener(new InsertMazeActionListener());
    }
}
