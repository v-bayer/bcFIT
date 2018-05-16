package ija.labyrinth.buttons;

import ija.labyrinth.listeners.SelectKryglActionsListener;

import javax.swing.*;

/**
 * Tlacitko reprezentujici figurky pri vyberu
 * Created by xjehla00, xbayer05 on 7. 5. 2015.
 */
public class SelectKryglButton extends GameMazeButton {
    /**
     * konstruktor
     * @param order     = poradi (ktery obrazek se ma nahrat) take se pouziva pri odesilani vyberu na server
     * @param locationX = X-ova souradnice tlacitka
     * @param locationY = Y-ova souradnice tlacitka
     * @param width     = sirka tlacitka
     * @param height    = vyska tlacitka
     */
    public SelectKryglButton(int order, int locationX, int locationY, int width, int height) {
        super(locationX, locationY, width, height);

        // Podle hodnoty order, se nastavi pozadi
        switch (order){
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
            case 10:
                // tato hodnota reprezentuje tlacitko zpet
                setIcon(null);
                setBorder(new JButton("nic").getBorder());
                setBorderPainted(true);
                break;
        }

        addActionListener(new SelectKryglActionsListener());
    }
}
