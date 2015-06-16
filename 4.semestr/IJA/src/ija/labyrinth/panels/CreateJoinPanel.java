package ija.labyrinth.panels;

import ija.labyrinth.buttons.CreateJoinButton;
import ija.labyrinth.labels.images.MenuArrowLabel;

import javax.swing.*;
import java.awt.*;

/**
 * Obrazovka pro vytvoreni/pripojeni ke hre
 * Created by xjehla00, xbayer05 on 10. 5. 2015.
 */
public class CreateJoinPanel extends JPanel {
    private CreateJoinButton host = new CreateJoinButton("Vytvořit hru", 86, 275, 200, 35);  // Tlacitko pro hostovani hry
    private CreateJoinButton join = new CreateJoinButton("Připojit se", 86, 320, 200, 35);   // Tlacitko pro pripojeni se ke hre
    private CreateJoinButton back = new CreateJoinButton("", 38, 458, 111, 31);

    private MenuArrowLabel[] menuArrowLabel = new MenuArrowLabel[2];            // Sipky ukazujici na tlacitka

    /**
     * Konstruktor
     */
    public CreateJoinPanel(){
        setLayout(null);
        add(host);
        add(join);
        add(back);

        // Vykresleni sipek
        for (int i = 0; i < 2; i++){
            menuArrowLabel[i] = new MenuArrowLabel(38, 285 + i * 45);
            add(menuArrowLabel[i]);
        }
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_NovaHra1.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
