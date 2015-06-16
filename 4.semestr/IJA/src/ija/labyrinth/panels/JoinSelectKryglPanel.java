package ija.labyrinth.panels;

import ija.labyrinth.buttons.JoinSelectKryglButton;
import ija.labyrinth.buttons.SelectKryglButton;
import ija.labyrinth.connections.Protocol;
import ija.labyrinth.labels.images.MenuArrowLabel;
import ija.labyrinth.listeners.JoinSelectKryglActionListener;

import javax.swing.*;
import java.awt.*;

/**
 * Obrazovka pro vyber kryglu pri prihlasovani do hry
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class JoinSelectKryglPanel extends JPanel {
    // Tlacitka vyberu kryglu
    private static JoinSelectKryglButton[] krygly = new JoinSelectKryglButton[10];

    // Sipky ukazujici na krygly
    private MenuArrowLabel[] menuArrowLabel = new MenuArrowLabel[2];

    private static int selectedKrygl = 0;

    /**
     * Konstruktor
     */
    public JoinSelectKryglPanel(){
        setLayout(null);
        // Vykresleni kryglu
        for (int i = 0; i < 10; i++){
            krygly[i] = new JoinSelectKryglButton(i, 60 + i * 52 - (int)(i / 5) * 5 * 52, 218 + (int)(i / 5) * 52, 50, 50);
            add(krygly[i]);
        }

        // Vykresleni sipek
        for (int i = 0; i < 2; i++){
            menuArrowLabel[i] = new MenuArrowLabel(38, 240 + i * 45);
            add(menuArrowLabel[i]);
        }
    }

    /**
     * Nastavi vybrany krygl na vybrany krygl
     * @param choosenKrygl  = index vybraneho kryglu
     */
    public static void setSelectedKrygl(int choosenKrygl){
        selectedKrygl = choosenKrygl;
    }

    /**
     * Vrati vybrany krygl
     * @return  = index vybraneho kryglu
     */
    public static int getSelectedKrygl(){
        return selectedKrygl;
    }

    /**
     * Ziskani vybraneho kriglu
     * @param index = index vybraneho kryglu
     * @return      = krygl
     */
    public static JoinSelectKryglButton getKrygl(int index){
        return krygly[index];
    }

    /**
     * Nastavi krygly na neklikaci
     */
    public static void setAlowedKrygl(){
        for (Integer indexKrygl : Protocol.getNoKrigl()){
            krygly[indexKrygl].setEnabled(false);
        }
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
