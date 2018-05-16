package ija.labyrinth.textfields;

import javax.swing.*;
import java.awt.*;

/**
 * Nastaveni IP adresy serveru pro pripojeni
 * Created by xjehla00, xbayer05 on 11. 5. 2015.
 */
public class JoinGameTextField extends JTextField {
    private static String ipAddress;

    /**
     * Konstruktor
     */
    public JoinGameTextField(){
        setBorder(null);
        //setText("192.168.173.40");
        setText("localhost");
        setOpaque(false);
        setBounds(87, 281, 214, 42);
        setForeground(new Color(240, 138, 29, 255));
        setFont(new Font("Calibri", Font.BOLD, 20));
        setHorizontalAlignment(SwingConstants.CENTER);
    }

    /**
     * nastavi ip adresu na tu ze zadavaciho pole
     */
    public void setIpAddress(){
        ipAddress = getText();
    }

    /**
     * vrati nastavenou IP adresu
     * @return = ip adresa
     */
    public static String getIpAddress(){
        return ipAddress;
    }
}
