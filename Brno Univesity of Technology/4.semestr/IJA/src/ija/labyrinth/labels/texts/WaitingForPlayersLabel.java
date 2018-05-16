package ija.labyrinth.labels.texts;

import javax.swing.*;
import java.awt.*;

/**
 * Label popisujici cekani na hrace
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class WaitingForPlayersLabel extends JLabel {
    /**
     * Konstruktor
     */
    public WaitingForPlayersLabel(){
        setText("Čekám na ostatní hráče...");
        setBounds(157, 320, 500, 23);
        setForeground(new Color(240, 138, 29, 255));
        setFont(new Font("Calibri", Font.BOLD, 20));
    }
}
