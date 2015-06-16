package ija.labyrinth;
import ija.labyrinth.frames.*;

/**
 * Testovaci trida funkcnosti klienta / spousteci trida pro klienta
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class play {
    // Ramec pro GUI
    static MainFrame menuFrame = new MainFrame();

    // Trida pri startu programu nacte menu
    public static void main(String[] args){
        menuFrame.setMenuPanel();
    }

    // Nastaveni hlavniho menu (napriklad po klinuti na tlacitko menu)
    public static void setMainMenu(){
        menuFrame.setMenuPanel();
    }

    // Nastavi obrazovku pro vyber poctu hracu
    public static void setSelectPlayers(){
        menuFrame.setSelectPlayersPanel();
    }

    // Nastavi obrazovku pro vyber velikosti hry
    public static void setNewGame() {
        menuFrame.setSelectGamePanel();
    }

    // Nastavi obrazovku pro vyber hraci figurky
    public static void setSelectKrygl() {
        menuFrame.setSelectKryglPanel();
    }

    // Nastavi herni obrazovku
    public static void setGame(){
        menuFrame.setGamePanel();
    }

    // Nastavi obrazovku pro pripojeni nebo hostovani hry
    public static void setCreateJoin(){
        menuFrame.setCreateJoinPanel();
    }

    // Nastavi obrazovku na pripojeni ke hre
    public static void setJoinGame(){
        menuFrame.setJoinGamePanel();
    }

    // Nastavi prihlasovani na load game
    public static void setLoadGamePanel(){
        menuFrame.setLoadGamePanel();
    }

    // Nastavi prihlasovani na score
    public static void setScoreGamePanel() {
        menuFrame.setScoreGamePanel();
    }

    // Nastavi jako aktivni plochu pro vyber ulozene hry
    public static void setLoadGameConfirmPanel() {
        menuFrame.setLoadGameConfirmPanel();
    }

    // Nastavi jako aktivni obrazovku na koukani na score
    public static void setScorePanel(){
        menuFrame.setScorePanel();
    }

    // Nastavi obrazovku na cekani na hrace
    public static void setWaitingPanel(){
        menuFrame.setWaitingForPlayersPanel();
    }

    // Nastavi obrazovku na cekani na hrace
    public static void setCreateGamePanel(){
        menuFrame.setCreateGamePanel();
    }

    // Nastavi obrazovku na vyber kryglu pro join hrace
    public static void setJoinSelKrygl(){
        menuFrame.setJoinSelectKryglPanel();
    }

    // Nastavime novou hraci plochu
    public static void setNewGamePanel() {
        menuFrame.setNewGamePanel();
    }

    // Vrati hlavni frame
    public static MainFrame getMenuFrame(){
        return menuFrame;
    }
}
