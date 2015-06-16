package ija.labyrinth.panels;

import ija.labyrinth.buttons.SelectGameButton;

import javax.swing.*;
import java.awt.*;

/**
 * Panel zobrazujici vyber velikosti hry
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class SelectGamePanel extends JPanel {
    // Tlacitka vyberu velikosti hry
    private SelectGameButton zpet = new SelectGameButton("Zpet", 38, 458, 111, 31);             // Tlacitko zpet
    private SelectGameButton pet = new SelectGameButton("pet", 80, 371, 15, 15);                // Tlacitko pro hru 5x5
    private SelectGameButton sedm = new SelectGameButton("sedm", 145, 356, 30, 30);             // Tlacitko pro hru 7x7
    private SelectGameButton devet = new SelectGameButton("devet", 212, 326, 60, 60);           // Tlacitko pro hru 9x9
    private SelectGameButton jedenact = new SelectGameButton("jedenact", 311, 266, 120, 120);   // Tlacitko pro hru 11x11

    private static int gameSize = 7;

    /**
     * Konstruktor
     */
    public SelectGamePanel(){
        setLayout(null);
        // Vypsani tlacitek
        add(zpet);
        add(pet);
        add(sedm);
        add(devet);
        add(jedenact);
    }

    /**
     * Nastavi velikost hry
     * @param size  = valikost
     */
    public static void setGameSize(int size){
        gameSize = size;
    }

    /**
     * Vrati velikost hry
     * @return  = velikost
     */
    public static int getGameSize(){
        return gameSize;
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_NovaHra2.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
