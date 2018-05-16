package ija.labyrinth.frames;

import ija.labyrinth.panels.*;

import javax.swing.*;
import java.awt.*;

/**
 * Hlavni frame
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class MainFrame extends JFrame {
    /**
     * Panely potrebne pro hru
     * menuPanel           = Panel zobrazujici menu
     * selectGamePanel     = Panel zobrazujici vyber velikosti hry
     * selectKryglPanel    = Panel zobrazujici vyber figurky
     * gamePanel           = Panel zobrazujici herni okno
     * playersPanel        = Panel zobrazujici vyber poctu hracu
     */
    private MenuPanel menuPanel = new MenuPanel();
    private SelectGamePanel selectGamePanel = new SelectGamePanel();
    private SelectKryglPanel selectKryglPanel = new SelectKryglPanel();
    private static GamePanel gamePanel = new GamePanel();
    private SelectPlayersPanel playersPanel = new SelectPlayersPanel();
    private CreateJoinPanel createJoinPanel = new CreateJoinPanel();
    private JoinGamePanel createGamePanel = new JoinGamePanel(0);
    private static JoinGamePanel joinGamePanel = new JoinGamePanel(1);
    private JoinGamePanel loadGamePanel = new JoinGamePanel(2);
    private JoinGamePanel scoreGamePanel = new JoinGamePanel(3);
    private WaitingForPlayersPanel waitingForPlayersPanel = new WaitingForPlayersPanel();
    private JoinSelectKryglPanel joinSelectKryglPanel = new JoinSelectKryglPanel();
    private LoadGameConfirmPanel loadGameConfirmPanel = new LoadGameConfirmPanel();
    private ScorePanel scorePanel = new ScorePanel();

    /**
     * Nastavuje velikost a umisteni Frame pro menu
     */
    private void setMenuPanelSize(){
        invalidate();
        validate();
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setSize(800, 600);
        setBounds(screenSize.width / 2 - getWidth() / 2, screenSize.height / 2 - getHeight() / 2, 800, 600);
    }

    public static void setIpAddress(){
        joinGamePanel.setIpAddress();
    }

    /**
     * Nastavuje velikost a umisteni Frame pro hru
     */
    private void setGamePanelSize(){
        invalidate();
        validate();
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        setSize(1030, 720);
        setBounds(screenSize.width / 2 - getWidth() / 2, screenSize.height / 2 - getHeight() / 2 - 20, 1030, 720);
    }

    /**
     * Nastavi novou cistou hraci plochu
     */
    public void setNewGamePanel(){
        gamePanel = new GamePanel();
    }

    /**
     * Nastavuje jako aktivni panel cekani na hrace
     */
    public void setWaitingForPlayersPanel(){
        setContentPane(waitingForPlayersPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel Vyberu hry na nacteni
     */
    public void setLoadGameConfirmPanel(){
        setContentPane(loadGameConfirmPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavi jako aktivni obrazovku na koukani na skore
     */
    public void setScorePanel(){
        setContentPane(scorePanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje obrazovku na prihlaseni na load game
     */
    public void setLoadGamePanel(){
        setContentPane(loadGamePanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje obrazovku na prihlaseni na score
     */
    public void setScoreGamePanel(){
        setContentPane(scoreGamePanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel vyberu kryglu pro join uzivatele
     */
    public void setJoinSelectKryglPanel(){
        setContentPane(joinSelectKryglPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel vytvoreni hry (Zadani IP)
     */
    public void setCreateGamePanel(){
        setContentPane(createGamePanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel pro pripojeni ke hre
     */
    public void setJoinGamePanel(){
        setContentPane(joinGamePanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel pro vyber hostovani, ci pripojeni ke hre
     */
    public void setCreateJoinPanel(){
        setContentPane(createJoinPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel panel pro vyber hracu
     */
    public void setSelectPlayersPanel(){
        setContentPane(playersPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel panel pro hlavni menu
     */
    public void setMenuPanel(){
        setContentPane(menuPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel panel pro vyber velikosti hry
     */
    public void setSelectGamePanel(){
        setContentPane(selectGamePanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel panel pro vyber figurky
     */
    public void setSelectKryglPanel(){
        setContentPane(selectKryglPanel);
        setMenuPanelSize();
    }

    /**
     * Nastavuje jako aktivni panel panel pro hraci plochu
     */
    public void setGamePanel() {
        setContentPane(gamePanel);
        setGamePanelSize();
        gamePanel.createGame();

        invalidate();
        validate();
    }

    /**
     * Konstruktor
     */
    public MainFrame(){
        // Nastaveni popisku aplikace
        setTitle("Pivní Labyrinth");

        // Nastaveni otravne ikonky
        setIconImage(new ImageIcon("lib/Obr/UI/UI.jpg").getImage());

        // Zakazani rozsirovani frame
        setResizable(false);

        // Nastaveni exitu a vyktresleni frame
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
    }

    /**
     * inicializace pincliku
     */
    public static void initPekineese(){
        gamePanel.initKryglPekineseLabels();
    }

    /**
     * Inicializuje figurky pri loadgame
     * @param indexPlayer = index hrace
     */
    public static void setDefaultPekineseLabel(int indexPlayer){
        gamePanel.setDefaultPekineseLabel(indexPlayer);
    }

    /**
     * Vytiskne poklady
     */
    public static void printTreasuresLabel(){
        gamePanel.printTreasuresPlayer();
    }

    /**
     * Nainicializuje scoreLabel
     */
    public static void initScoreLabel(){
        gamePanel.initScoreLabel();
    }

    /**
     * Nastavi pocet figurek
     */
    public static void setNumberOfPekineseLabels(){
        gamePanel.setNumberOfPekineseLabels();
    }

    /**
     * Nastavi hrace na tahu
     * @param addScore  = score, ktere mu ma pridat
     * @param findedCard= karta, kterou nasel
     */
    public static void setOnTurn(int addScore, int findedCard){
        gamePanel.setIsOnTurn(addScore, findedCard);
    }
}
