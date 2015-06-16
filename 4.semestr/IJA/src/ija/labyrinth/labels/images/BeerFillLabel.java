package ija.labyrinth.labels.images;

import javax.swing.*;
import java.awt.*;

/**
 * Label zobrazujici naplnene pivo s procentualnim score
 * Created by xjehla00, xbayer05 on 9. 5. 2015.
 */
public class BeerFillLabel extends JLabel {
    // Nactene ikony daneho labelu
    ImageIcon[] icon = new ImageIcon[11];

    /**
     * Konstruktor + nacteni ikon
     */
    public BeerFillLabel(){
        setBounds(762, 430, 233, 248);
        //setBounds(0, 0, 233, 248);
        setText("");

        for (int i = 0; i < 11; i++){
            icon[i] = new ImageIcon(new ImageIcon("lib/Obr/PivoNapln/PivoNapln" + i + ".png").getImage().getScaledInstance(233, 248, java.awt.Image.SCALE_SMOOTH));
        }
    }

    /**
     * Nastaveni piva na danem indexu
     * @param index = index piva
     */
    public void setBeerFill(int index){
        setIcon(icon[index]);
    }

    /**
     * nastavi text plniciho piva
     * @param procenta = procenta
     */
    public void setBeerText(int procenta){
        setText(procenta + "%");
        setForeground(new Color(0, 0, 0, 255));
        setFont(new Font("Calibri", Font.BOLD, 50));
        setVerticalAlignment(CENTER);
        setHorizontalAlignment(CENTER);
    }
}
