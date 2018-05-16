package ija.labyrinth.panels;

import ija.labyrinth.buttons.JoinGameButton;
import ija.labyrinth.textfields.JoinGameTextField;

import javax.swing.*;
import java.awt.*;

/**
 * Panel, ktery reprezentuje obrazovku pro prihlasovani se na server
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class JoinGamePanel extends JPanel {
    private JoinGameButton joinGameButton = new JoinGameButton("Připojit ke hře");
    private JoinGameButton createGameButton = new JoinGameButton("Vytvořit hru");
    private JoinGameButton loadGameButton = new JoinGameButton("Nahrát");
    private JoinGameButton scoreGameButton = new JoinGameButton("Nejvyšší skóre");
    private JoinGameButton back = new JoinGameButton("");
    private JoinGameButton backToMenu = new JoinGameButton("Menu");
    private static JoinGameTextField textField;

    /**
     * konstruktor
     * @param state = zdali se jedna o vytvoreni hry, pripojeni, ziskani score, nebo nacteni hry
     */
    public JoinGamePanel(int state){
        setLayout(null);
        textField = new JoinGameTextField();
        add(joinGameButton);
        add(createGameButton);
        add(loadGameButton);
        add(scoreGameButton);
        add(textField);
        add(back);
        add(backToMenu);

        if (state == 0){        // Nastavi se na vytvareni hry
            createGameButton.setVisible(true);
            joinGameButton.setVisible(false);
            loadGameButton.setVisible(false);
            scoreGameButton.setVisible(false);
            back.setVisible(true);
            backToMenu.setVisible(false);
        }
        else if (state == 1){   // Nastavi se na prihlaseni do hry
            createGameButton.setVisible(false);
            joinGameButton.setVisible(true);
            loadGameButton.setVisible(false);
            scoreGameButton.setVisible(false);
            back.setVisible(true);
            backToMenu.setVisible(false);
        }
        else if (state == 2){   // Nastavi se na load game
            createGameButton.setVisible(false);
            joinGameButton.setVisible(false);
            loadGameButton.setVisible(true);
            scoreGameButton.setVisible(false);
            back.setVisible(false);
            backToMenu.setVisible(true);
        }
        else if (state == 3){   // Nastavi se na zobrazeni skore
            createGameButton.setVisible(false);
            joinGameButton.setVisible(false);
            loadGameButton.setVisible(false);
            scoreGameButton.setVisible(true);
            back.setVisible(false);
            backToMenu.setVisible(true);
        }
    }

    /**
     * nastavi ip adresu na text v textfieldu
     */
    public static void setIpAddress(){
        textField.setIpAddress();
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_IP.jpg").getImage();

        g.drawImage(image, 0, 0, null);
    }
}
