package ija.labyrinth.labels.images;

import javax.swing.*;

/**
 * Label reprezentujici podbarveni startovacich pozic
 * Created by xjehla00, xbayer05 on 9. 5. 2015.
 */
public class StartPositionLabel extends JLabel {
    // Vycet moznych barev startovacich pozic
    public enum colorStartMaze{
        RED,
        GREEN,
        BLUE,
        YELLOW,
    }

    /**
     * Konstruktor
     * @param color     = barva pozice
     * @param locationX = X-ova souradnice Labelu
     * @param locationY = Y-ova souradnice Labelu
     * @param width     = sirka
     * @param height    = vyska
     */
    public StartPositionLabel(colorStartMaze color, int locationX, int locationY, int width, int height){
        setBounds(locationX, locationY, width, height);
        setText("");

        // Vyber pozadi na zaklade barvy
        if (color == colorStartMaze.RED){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeStartRed.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
        }
        else if (color == colorStartMaze.GREEN){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeStartGreen.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
        }
        else if (color == colorStartMaze.BLUE){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeStartBlue.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
        }
        else if (color == colorStartMaze.YELLOW){
            setIcon(new ImageIcon(new ImageIcon("lib/Obr/Maze/MazeStartYellow.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH)));
        }
    }
}
