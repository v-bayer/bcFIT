package ija.labyrinth.panels;

import ija.labyrinth.buttons.SelectPlayersButton;
import ija.labyrinth.labels.images.MenuArrowLabel;

import javax.swing.*;
import java.awt.*;

/**
 * Panel pro zobrazeni vyberu hracu
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class SelectPlayersPanel extends JPanel {
    private SelectPlayersButton jeden = new SelectPlayersButton("one", 280);    // Tlacitko pro dva hrace
    private SelectPlayersButton dva = new SelectPlayersButton("two", 320);      // Tlacitko pro tri hrace
    private SelectPlayersButton tri = new SelectPlayersButton("three", 360);    // Tlacitko pro ctyri hrace
    private SelectPlayersButton zpet = new SelectPlayersButton("zpet", 0);      // Tlacitko zpet

    private MenuArrowLabel[] menuArrowLabel = new MenuArrowLabel[3];            // Sipky k tlacitkum

    private static int numOfPlayers;                                            // Pocet hracu

    /**
     * Ziskani poctu hracu
     * @return = Pocet hracu
     */
    public static int getNumOfPlayers(){
        return numOfPlayers;
    }

    /**
     * Nastaveni poctu hracu
     * @param Count = pocet hracu
     */
    public static void setNumOfPlayers(int Count){
        numOfPlayers = Count;
    }

    /**
     * Konstruktor
     */
    public SelectPlayersPanel(){
        setLayout(null);
        // Vypsani tlacitek
        add(jeden);
        add(dva);
        add(tri);
        zpet.setBounds(38, 458, 111, 31);
        add(zpet);

        // Vykresleni sipek
        for (int i = 0; i < 3; i++){
            menuArrowLabel[i] = new MenuArrowLabel(38, 290 + i * 40);
            add(menuArrowLabel[i]);
        }
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_PocetHracu.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
