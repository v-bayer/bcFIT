package ija.labyrinth.labels.images;

import ija.labyrinth.panels.GamePanel;

import javax.swing.*;

/**
 * Label reprezentujici poklad na odkryte karte
 * Created by xjehla00, xbayer05 on 9. 5. 2015.
 */
public class TreasureCardLabel extends JLabel {
    // Set vsech pokladu
    ImageIcon[] icon = new ImageIcon[25];

    /**
     * Konstruktor
     */
    public TreasureCardLabel() {
        int width = 70;
        int height = 70;
        setBounds(660, 575, width, height);
        setText("");

        // Nacteni vsech pokladu do pole
        icon[0] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[1] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[2] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[3] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar4.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[4] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgBudvar5.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[5] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgGambrinus1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[6] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgHeineken.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[7] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKozel1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[8] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKozel2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[9] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKozel3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[10] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgKrusovice.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[11] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgPardal.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[12] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgPlzen.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[13] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgRadegast.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[14] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgStarobrno1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[15] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgStarobrno2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[16] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgStarobrno3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[17] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgSviany1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[18] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgSviany2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[19] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZlatopramen1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[20] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr1.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[21] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr2.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[22] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr3.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));
        icon[23] = new ImageIcon(new ImageIcon("lib/Obr/Piva/MazeImgZubr4.png").getImage().getScaledInstance(width, height, java.awt.Image.SCALE_SMOOTH));

        icon[24] = null;
    }

    /**
     * Nastaveni aktualniho odkryteho pokladu
     * @param index = idex v poli pokladu
     */
    public void chooseTreasureCardBeer(int index){
        setIcon(icon[index]);
    }
}
