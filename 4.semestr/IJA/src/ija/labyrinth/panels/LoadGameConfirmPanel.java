package ija.labyrinth.panels;

import ija.labyrinth.buttons.LoadGameConfirmButton;
import ija.labyrinth.comboboxes.LoadGameComboBox;

import javax.swing.*;
import java.awt.*;

/**
 * Panel, ktery zobrazi hry k nacteni
 * Created by xjehla00, xbayer05 on 14. 5. 2015.
 */
public class LoadGameConfirmPanel extends JPanel {
    private LoadGameConfirmButton select = new LoadGameConfirmButton("Vybrat");
    private LoadGameConfirmButton zpet = new LoadGameConfirmButton("");
    private static LoadGameComboBox games = new LoadGameComboBox();

    /**
     * konstruktor
     */
    public LoadGameConfirmPanel(){
        setLayout(null);
        add(select);
        add(zpet);
        add(games);
    }

    /**
     * vyprazdni ComboBox
     */
    public static void initComboBox(){
        while (games.getItemCount() != 0){
            games.remove(0);
        }
    }

    /**
     * Prida item do ComboBoxu
     * @param item = nazev itemu
     */
    public static void addItemToCombobox(String item){
        games.addItem(item);
    }

    /**
     * Vrati vybrany item z ComboBoxu
     * @return = item
     */
    public static String getSelItemFromCombobox(){
        return (String) games.getSelectedItem();
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_load.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
