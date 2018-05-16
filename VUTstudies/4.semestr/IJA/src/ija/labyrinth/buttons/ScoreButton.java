package ija.labyrinth.buttons;

import ija.labyrinth.listeners.ScorePanelActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Buttonek na vraceni ze score zpet na menu
 * Created by xjehla00, xbayer05 on 14. 5. 2015.
 */
public class ScoreButton extends JButton {
    /**
     * konstruktor
     */
    public ScoreButton(){
        super("");
        setBounds(38, 458, 111, 31);
        setContentAreaFilled(false);
        setOpaque(false);
        setFocusPainted(false);
        setForeground(new Color(240, 138, 29, 0));
        setFont(new Font("Calibri", Font.BOLD, 30));
        setCursor(new Cursor(Cursor.HAND_CURSOR));
        setHorizontalAlignment(SwingConstants.CENTER);

        addActionListener(new ScorePanelActionListener());
    }
}
