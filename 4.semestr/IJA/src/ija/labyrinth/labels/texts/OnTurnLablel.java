package ija.labyrinth.labels.texts;

import ija.labyrinth.connections.SetConnection;
import ija.labyrinth.panels.GamePanel;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;

/**
 * Label reprezentujici informace o tahu
 * Created by xjehla00, xbayer05 on 10. 5. 2015.
 */
public class OnTurnLablel extends JLabel {
    ImageIcon icon;

    /**
     * Konstruktor
     */
    public OnTurnLablel(){
        setBounds(698, 364, 300, 30);
        setForeground(new Color(35, 31, 32, 255));
        setFont(new Font("Calibri", Font.BOLD, 24));
        setHorizontalAlignment(SwingConstants.LEFT);
        setVerticalAlignment(SwingConstants.CENTER);
    }

    /**
     * Nastavi kdo je na tahu, popripade co ma hrac udelat
     * @param type  = 1 - tah jineho hrace, 2 - cekani na zasunuti kamene, 3 - cekani na tah figurkou
     */
    public void setActualPlayer(int type){
        switch (type){
            case 1:
                setText("Na tahu je ");
                setVerticalTextPosition(JLabel.CENTER);
                setHorizontalTextPosition(JLabel.LEFT);
                icon = GamePanel.getActualPlayerKryglIcon();
                Image img = icon.getImage();
                BufferedImage bi = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_ARGB);
                Graphics g = bi.createGraphics();
                g.drawImage(img, 0, 10, 30, 30, null);
                icon = new ImageIcon(bi);
                setIcon(icon);
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        SetConnection.listen();
                    }
                });
                break;
            case 2:
                setText("Vlož kámen");
                setIcon(null);
                break;
            case 3:
                setText("Táhni krýglem");
                setIcon(null);
                break;
        }
    }
}
