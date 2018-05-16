package ija.labyrinth.panels;

import ija.labyrinth.buttons.ScoreButton;
import ija.labyrinth.labels.texts.ScoreLabel;

import javax.swing.*;
import java.awt.*;

/**
 * Panel reprezentujici obrazovku pro prohlizeni nejvyssiho score
 * Created by xjehla00, xbayer05 on 14. 5. 2015.
 */
public class ScorePanel extends JPanel {
    private ScoreButton zpet = new ScoreButton();
    // TODO: Pridelat jeste label do ktereho se vypise score

    /**
     * konstruktor
     */
    public ScorePanel(){
        add(zpet);
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_NovaHra1.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
