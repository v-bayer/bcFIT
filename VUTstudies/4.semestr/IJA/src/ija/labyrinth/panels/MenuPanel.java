package ija.labyrinth.panels;

import ija.labyrinth.buttons.MenuButton;
import ija.labyrinth.labels.images.MenuArrowLabel;

import javax.swing.*;
import java.awt.*;

/**
 * Panel zobrazujici hlavni menu
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class MenuPanel extends JPanel {
    private MenuButton newGame = new MenuButton("Nová hra", 230 + 30);       // Tlacitko pro novou hru
    private MenuButton loadGame = new MenuButton("Načíst hru", 275 + 30);    // Tlacitko pro nacteni hry
    //private MenuButton score = new MenuButton("Nejvyšší skóre", 320 + 30);   // Tlacitko pro zobrazeni nejvyssiho skore
    //private MenuButton settings = new MenuButton("Nastavení", 365);     // Tlacitko pro zobrazeni nastaveni
    //private MenuButton exitGame = new MenuButton("Konec", 410);         // Tlacitko pro ukonceni aplikace
    private MenuButton exitGame = new MenuButton("Konec", 320 + 30);         // Tlacitko pro ukonceni aplikace

    private MenuArrowLabel[] menuArrowLabel = new MenuArrowLabel[3];    // Sipky ukazujici na tlacitka

    /**
     * Konstruktor
     */
    public MenuPanel(){
        setLayout(null);
        // Pridani tlacitek
        add(newGame);
        add(loadGame);
        //add(score);
        //add(settings);
        add(exitGame);

        // Vykresleni sipek
        for (int i = 0; i < 3; i++){
            menuArrowLabel[i] = new MenuArrowLabel(38, 240 + i * 45 + 30);
            add(menuArrowLabel[i]);
        }
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
