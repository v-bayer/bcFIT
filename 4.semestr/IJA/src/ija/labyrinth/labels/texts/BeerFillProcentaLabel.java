package ija.labyrinth.labels.texts;

import javax.swing.*;
import java.awt.*;

/**
 * Label reprezentujici text na plnicim se kryglu
 * Created by xjehla00, xbayer05 on 10. 5. 2015.
 */
public class BeerFillProcentaLabel extends JLabel {
    /**
     * konstruktor
     */
    public BeerFillProcentaLabel(){
        setBounds(762, 430, 180, 248);
        setForeground(new Color(35, 31, 32, 255));
        setFont(new Font("Calibri", Font.BOLD, 50));
        setVerticalAlignment(SwingConstants.CENTER);
        setHorizontalAlignment(SwingConstants.CENTER);
    }
}
