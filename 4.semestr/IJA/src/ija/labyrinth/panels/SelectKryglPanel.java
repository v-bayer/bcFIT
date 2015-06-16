package ija.labyrinth.panels;

import ija.labyrinth.buttons.SelectKryglButton;
import ija.labyrinth.labels.images.MenuArrowLabel;

import javax.swing.*;
import java.awt.*;

/**
 * Panel pro zobrazeni vyberu figurek
 * Created by xjehla00, xbayer05 on 7. 5. 2015.
 */
public class SelectKryglPanel extends JPanel {
    // Tlacitka vyberu kryglu
    private static SelectKryglButton[] krygly = new SelectKryglButton[11];

    // Sipky ukazujici na krygly
    private MenuArrowLabel[] menuArrowLabel = new MenuArrowLabel[2];

    private static int selectedKrygl = 0;

    /**
     * Konstruktor
     */
    public SelectKryglPanel(){
        setLayout(null);
        // Vykresleni kryglu
        for (int i = 0; i < 10; i++){
            krygly[i] = new SelectKryglButton(i, 60 + i * 52 - (int)(i / 5) * 5 * 52, 218 + (int)(i / 5) * 52, 50, 50);
            add(krygly[i]);
        }
        krygly[10] = new SelectKryglButton(10, 38, 458, 111, 31);
        add(krygly[10]);

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
    public static SelectKryglButton getKrygl(int index){
        return krygly[index];
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_NovaHra1.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
