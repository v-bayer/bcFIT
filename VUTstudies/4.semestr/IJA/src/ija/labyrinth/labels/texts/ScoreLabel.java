package ija.labyrinth.labels.texts;

import javax.swing.*;
import java.awt.*;

/**
 * Label reprezentujici skore
 * Created by xjehla00, xbayer05 on 10. 5. 2015.
 */
public class ScoreLabel extends JLabel {
    /**
     * Konstruktor
     * @param locationX = X-ova souradnice
     * @param locationY = Y-ova souradnice
     * @param fontSize  = Velikost pisma
     */
    public ScoreLabel(int locationX, int locationY, int fontSize){
        super("100");
        setBounds(locationX, locationY, 100, fontSize);
        setForeground(new Color(35, 31, 32, 255));
        setFont(new Font("Calibri", Font.BOLD, fontSize));
        setHorizontalAlignment(RIGHT);
    }

    /**
     * Vrati aktualni score
     * @return skore
     */
    public int getScore() {
        return Integer.parseInt(getText());
    }

    /**
     * nastavi skore
     * @param score = skore
     */
    public void setScore(int score){
        setText(Integer.toString(score));
    }
}
