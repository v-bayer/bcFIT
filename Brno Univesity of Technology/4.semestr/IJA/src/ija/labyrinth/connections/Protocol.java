package ija.labyrinth.connections;

import ija.labyrinth.frames.MainFrame;
import ija.labyrinth.panels.*;
import ija.labyrinth.play;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Protokoly pro komunikaci se serverem
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class Protocol {
    /**
     * hlavicky protokolu
     */
    public enum HEADERS{
        JOINGAME,
        READYCHECK,
        PLAYGAME,
        NEWGAME,
        WIN,
        EXIT,
        LOADGAME,
        NEWLOAD,
        CHECKREPEAT,
        END,
    }

    private boolean joinGame = false;
    private boolean readyCheck = false;
    private boolean checkRepeat = false;
    private boolean moveTurn = false;
    private boolean end = false;
    private boolean noReadyCheck = false;


    private boolean okState = false;

    private static int indexPlayer;
    private static String colorPlayer;
    private static int freeMazeTreasure = -1;

    private static int krigl = 0;
    private static List<Integer> noKrigl = new ArrayList<Integer>();
    private static Scanner scanner;
    private static boolean next = false;

    HEADERS head;

    public static int getIndexPlayer(){
        return indexPlayer;
    }

    public static List<Integer> getNoKrigl(){
        return noKrigl;
    }

    /**
     * Nainicializuje puvodni hodnoty
     */
    public void initFlags(){
        joinGame = false;
        readyCheck = false;
        checkRepeat = false;
        end = false;
        okState = false;
        krigl = 0;
        noKrigl = new ArrayList<>();
        next = false;
        noReadyCheck = false;
    }

    /**
     * vrati stav, jestli ma probehnout readycheck
     * @return = stav
     */
    public boolean getNoReadyCheck (){
        return noReadyCheck;
    }


    /**
     * Zjisteni, zda je nastaven parametr end
     * @return  = boolean end
     */
    public boolean getEnd(){
        return end;
    }

    /**
     * Rozpoznani hlavicky
     * @param line  = hlavicka
     */
    public void ParseLine(String line){
        if (line.equals("JOIN GAME")) head = HEADERS.JOINGAME;
        else if (line.equals("READY CHECK")) head = HEADERS.READYCHECK;
        else if (line.equals("PLAY GAME")) head = HEADERS.PLAYGAME;
        else if (line.equals("NEW GAME")) head = HEADERS.NEWGAME;
        else if (line.equals("TURN")) head = HEADERS.READYCHECK;
        else if (line.equals("WIN")) head = HEADERS.WIN;
        else if (line.equals("EXIT")) head = HEADERS.EXIT;
        else if (line.equals("LOAD GAME")) head = HEADERS.LOADGAME;
        else if (line.equals("NEW LOAD")) head = HEADERS.NEWLOAD;
    }

    /**
     * rozsifrovava zpravu ze serveru
     * @param line  = radek zpravy
     * @param i     = index radku
     * @return      = vraceni noveho indexu
     */
    public int FinalM(String line, int i){
        int winPlayer = 0;
        switch (head){
            case NEWLOAD:
                if (i == 2) {
                    SelectPlayersPanel.setNumOfPlayers(Integer.parseInt(line));
                    GamePanel.setNumOfKryglLabel(SelectPlayersPanel.getNumOfPlayers());
                    MainFrame.setNumberOfPekineseLabels();
                    MainFrame.initScoreLabel();
                    indexPlayer = GamePanel.getNumberOfPlayer();
                    GamePanel.setNewgame(false);
                }
                if (i == 3){
                    GamePanel.setGameSize(Integer.parseInt(line));
                    if (GamePanel.getGameSize() == 5) {
                        GamePanel.setNumOfCards(12 / SelectPlayersPanel.getNumOfPlayers());
                    }
                    else {
                        GamePanel.setNumOfCards(24 / SelectPlayersPanel.getNumOfPlayers());
                    }
                    GamePanel.initTreasuresPlayers();

                    String[] udaje;

                    int[] intUdaje;

                    int[] poklady;

                    for (int j = 0; j < SelectPlayersPanel.getNumOfPlayers(); j++){
                        line = SetConnection.getLine();

                        udaje = line.split(";");

                        intUdaje = new int[udaje.length];

                        for (int k = 0; k < udaje.length; k++){
                            intUdaje[k] = Integer.parseInt(udaje[k]);
                        }

                        SelectKryglPanel.setSelectedKrygl(intUdaje[1]);

                        if (indexPlayer == intUdaje[0]){
                            GamePanel.setKryglLabelSelf(intUdaje[1]);
                            GamePanel.setWantedTreasure(intUdaje[4]);
                        }
                        else {
                            GamePanel.setKryglLabelOther(intUdaje[0] - 1, intUdaje[1]);
                        }

                        MainFrame.setDefaultPekineseLabel(intUdaje[0] - 1);

                        GamePanel.setKryglPekineseLabel(intUdaje[0] - 1, intUdaje[2], intUdaje[3]);
                        GamePanel.setScore(intUdaje[0], intUdaje[5]);

                        // TODO: Nastavit vsem hracum jejich poklady

                        poklady = new int[intUdaje.length - 6];

                        for (int k = 6; k < intUdaje.length; k++) {
                            poklady[k - 6] = intUdaje[k];
                        }

                        GamePanel.setTreasuresToPlayer(intUdaje[0], poklady);

                        i++;
                    }
                }
                else if (i == (4 + SelectPlayersPanel.getNumOfPlayers())){
                    String[] udaje;

                    int[] intUdaje;

                    udaje = line.split(";");

                    intUdaje = new int[udaje.length];

                    for (int k = 0; k < udaje.length; k++){
                        intUdaje[k] = Integer.parseInt(udaje[k]);
                    }

                    // Zjisteni volneho kamene
                    GamePanel.setFreeMazeLabel(intUdaje[2] - 1, intUdaje[4]);

                    if (intUdaje[3] != -1){
                        GamePanel.setTreasureMazes(intUdaje[3], 655 + (45 - GamePanel.getSizeTreasure() / 2), 445 + (45 - GamePanel.getSizeTreasure() / 2));
                    }

                    if (intUdaje[1] == 1){
                        GamePanel.setMoveTurn(false);
                    }
                    else{
                        GamePanel.setMoveTurn(true);
                    }

                    GamePanel.setIsOnTurn(intUdaje[0]);
                }
                else if (i == (5 + SelectPlayersPanel.getNumOfPlayers())){
                    play.setGame();
                    MainFrame.printTreasuresLabel();
                    GamePanel.setNewMazeType(line.split(";"));
                }
                else if (i == (6 + SelectPlayersPanel.getNumOfPlayers())){
                    GamePanel.putTreasureMaze(line.split(","));
                }
                break;
            case JOINGAME:
                if (i == 2 && line.equals("ok")) {
                    okState = true;
                }
                else if (i == 2 && line.equals("nok")) {
                    end = true;
                }
                if (okState) {
                    if (i == 3) {
                        indexPlayer = Integer.parseInt(line);
                        GamePanel.setNumberOfPlayer(indexPlayer);
                    }
                    if (i == 4) {
                        scanner = new Scanner(line);
                        noKrigl = new ArrayList<Integer>();
                        while (scanner.hasNextInt()) {
                            noKrigl.add(scanner.nextInt());
                        }
                        if ((noKrigl.size() == 1) && (noKrigl.get(0) == -1)){
                            noReadyCheck = true;
                        }
                        else {
                            JoinSelectKryglPanel.setAlowedKrygl();
                        }
                        okState = false;
                    }
                }
                break;
            case READYCHECK:
                if (i == 2 && line.equals("ok")) {
                    okState = true;
                    play.setWaitingPanel();
                }
                if ((i == 3) && (!okState)) {
                    scanner = new Scanner(line);
                    noKrigl = new ArrayList<Integer>();
                    while (scanner.hasNextInt()) {
                        noKrigl.add(scanner.nextInt());
                    }
                    JoinSelectKryglPanel.setAlowedKrygl();
                }
                break;
            case NEWGAME:
                if (i == 2) {
                    SelectPlayersPanel.setNumOfPlayers(Integer.parseInt(line));
                    GamePanel.setNumOfKryglLabel(SelectPlayersPanel.getNumOfPlayers());
                }
                if (i == 3){
                    GamePanel.setGameSize(Integer.parseInt(line));

                    String[] udaje;

                    int[] intUdaje;

                    for (int j = 0; j < SelectPlayersPanel.getNumOfPlayers(); j++){
                        line = SetConnection.getLine();

                        udaje = line.split(";");

                        intUdaje = new int[udaje.length];

                        for (int k = 0; k < udaje.length; k++){
                            intUdaje[k] = Integer.parseInt(udaje[k]);
                        }

                        if (indexPlayer == intUdaje[0]){
                            GamePanel.setKryglLabelSelf(intUdaje[1]);
                            GamePanel.setWantedTreasure(intUdaje[4]);
                        }
                        else {
                            GamePanel.setKryglLabelOther(intUdaje[0] - 1, intUdaje[1]);
                        }
                        i++;
                        //System.out.println(i + " PRIJMAM: " + line);
                    }
                    MainFrame.initPekineese();
                }
                else if (i == (4 + SelectPlayersPanel.getNumOfPlayers())){
                    String[] udaje;

                    int[] intUdaje;

                    udaje = line.split(";");

                    intUdaje = new int[udaje.length];

                    for (int k = 0; k < udaje.length; k++){
                        intUdaje[k] = Integer.parseInt(udaje[k]);
                    }

                    // Zjisteni volneho kamene
                    GamePanel.setFreeMazeLabel(intUdaje[4] - 1, intUdaje[6]);

                    if (intUdaje[5] != -1){
                        GamePanel.setTreasureMazes(intUdaje[5], 655 + (45 - GamePanel.getSizeTreasure() / 2), 445 + (45 - GamePanel.getSizeTreasure() / 2));
                    }
                }
                else if (i == (5 + SelectPlayersPanel.getNumOfPlayers())){
                    play.setGame();
                    GamePanel.setNewMazeType(line.split(";"));
                }
                else if (i == (6 + SelectPlayersPanel.getNumOfPlayers())){
                    GamePanel.putTreasureMaze(line.split(","));
                }
                break;
            case PLAYGAME:
                // Udaje o hracich
                if (i == 3){

                    String[] udaje;

                    int[] intUdaje;

                    for (int j = 0; j < SelectPlayersPanel.getNumOfPlayers(); j++){
                        line = SetConnection.getLine();

                        udaje = line.split(";");

                        intUdaje = new int[udaje.length];

                        for (int k = 0; k < udaje.length; k++){
                            intUdaje[k] = Integer.parseInt(udaje[k]);
                        }

                        if (indexPlayer == intUdaje[0]){
                            GamePanel.setWantedTreasure(intUdaje[4]);
                        }

                        GamePanel.setKryglPekineseLabel(intUdaje[0] - 1, intUdaje[2], intUdaje[3]);

                        i++;
                        //System.out.println(i + " PRIJMAM: " + line);
                    }
                }
                // Udaje o jednotlivych zmenach (ktery hrac je na tahu, jestli byl nalezen poklad, jaky je stav hry a
                // udaje o volnem kameni)
                else if (i == (4 + SelectPlayersPanel.getNumOfPlayers())){
                    String[] udaje;

                    int[] intUdaje;

                    udaje = line.split(";");

                    intUdaje = new int[udaje.length];

                    for (int k = 0; k < udaje.length; k++){
                        intUdaje[k] = Integer.parseInt(udaje[k]);
                    }

                    // Zjisteni volneho kamene
                    GamePanel.setFreeMazeLabel(intUdaje[4] - 1, intUdaje[6]);

                    if (intUdaje[5] != -1){
                        freeMazeTreasure = intUdaje[5];
                    }

                    if ((intUdaje[3] == 1)){
                        if(intUdaje[2] == -1){
                            MainFrame.setOnTurn(0, -1);
                        }
                        else {
                            MainFrame.setOnTurn(100, intUdaje[2]);
                        }
                    }

                    if ((GamePanel.getIsOnTurn() == intUdaje[0]) && (intUdaje[0] == indexPlayer) && (intUdaje[3] == 1)){
                        GamePanel.setMoveTurn(false);
                    }
                    else if ((GamePanel.getIsOnTurn() == intUdaje[0]) && (intUdaje[0] == indexPlayer) && (intUdaje[3] == 2)){
                        GamePanel.setMoveTurn(true);
                        moveTurn = true;
                    }

                    GamePanel.setActiveButtons();
                }
                // Info o kamenech na hraci plose
                else if (i == (5 + SelectPlayersPanel.getNumOfPlayers())){
                    GamePanel.setNewMazeType(line.split(";"));
                }
                // Info o pokladech na kamenech
                else if (i == (6 + SelectPlayersPanel.getNumOfPlayers())){
                    GamePanel.putTreasureMaze(line.split(","));
                    if (freeMazeTreasure != -1){
                        GamePanel.setTreasureMazes(freeMazeTreasure, 655 + (45 - GamePanel.getSizeTreasure() / 2), 445 + (45 - GamePanel.getSizeTreasure() / 2));
                        freeMazeTreasure = -1;
                    }
                }
                // Info o tlacitkach, na ktere se da kliknout (posuv figurky)
                else if (i == (7 + SelectPlayersPanel.getNumOfPlayers()) && (moveTurn)){
                    String[] udaje = line.split(",");

                    int[] intUdaje = new int[udaje.length];

                    for (int k = 0; k < udaje.length; k++){
                        intUdaje[k] = Integer.parseInt(udaje[k]);
                    }

                    GamePanel.setClickableButtons(intUdaje);
                    moveTurn = false;
                }
                break;
            case WIN:
                switch (indexPlayer){
                    case 1:
                        winPlayer = (Integer.parseInt(line));
                        break;
                    case 2:
                        switch (SelectPlayersPanel.getNumOfPlayers()){
                            case 2:
                                winPlayer = (Integer.parseInt(line) + 1);
                                break;
                            case 3:
                                if ((Integer.parseInt(line)) == 3){
                                    winPlayer = 2;
                                }
                                else{
                                    winPlayer = (Integer.parseInt(line) + 2);
                                }
                                break;
                            case 4:
                                if ((Integer.parseInt(line)) == 1){
                                    winPlayer = 4;
                                }
                                else{
                                    winPlayer = (Integer.parseInt(line) + 1);
                                }
                                break;
                        }
                        break;
                    case 3:
                        switch (SelectPlayersPanel.getNumOfPlayers()){
                            case 3:
                                winPlayer = (Integer.parseInt(line) + 1);
                                break;
                            case 4:
                                if ((Integer.parseInt(line)) == 4){
                                    winPlayer = 2;
                                }
                                else{
                                    winPlayer = (Integer.parseInt(line) + 2);
                                }
                                break;
                        }
                        break;
                    case 4:
                        winPlayer = (Integer.parseInt(line) + 1);
                        break;
                }

                JOptionPane.showMessageDialog(play.getMenuFrame(), "Hráč číslo " + winPlayer + " vyhrál hru.", "Win Game", JOptionPane.INFORMATION_MESSAGE);
                SetConnection.closeConnection();
                System.exit(0);
                break;
            case EXIT:
                switch (indexPlayer){
                    case 1:
                        winPlayer = (Integer.parseInt(line));
                        break;
                    case 2:
                        switch (SelectPlayersPanel.getNumOfPlayers()){
                            case 2:
                                winPlayer = (Integer.parseInt(line) + 1);
                                break;
                            case 3:
                                if ((Integer.parseInt(line)) == 3){
                                    winPlayer = 2;
                                }
                                else{
                                    winPlayer = (Integer.parseInt(line) + 2);
                                }
                                break;
                            case 4:
                                if ((Integer.parseInt(line)) == 1){
                                    winPlayer = 4;
                                }
                                else{
                                    winPlayer = (Integer.parseInt(line) + 1);
                                }
                                break;
                        }
                        break;
                    case 3:
                        switch (SelectPlayersPanel.getNumOfPlayers()){
                            case 3:
                                winPlayer = (Integer.parseInt(line) + 1);
                                break;
                            case 4:
                                if ((Integer.parseInt(line)) == 4){
                                    winPlayer = 2;
                                }
                                else{
                                    winPlayer = (Integer.parseInt(line) + 2);
                                }
                                break;
                        }
                        break;
                    case 4:
                        winPlayer = (Integer.parseInt(line) + 1);
                        break;
                }

                JOptionPane.showMessageDialog(play.getMenuFrame(), "Hráč číslo " + winPlayer + " se odpojil.", "Exit Game", JOptionPane.INFORMATION_MESSAGE);
                SetConnection.closeConnection();
                System.exit(0);
                break;
            case LOADGAME:
                if (i == 2){
                    while (!line.equals("end")){

                        // TODO: přidat nějaký pitomý obsah do combobox
                        if (line.equals("nok")){
                            end = true;
                        }
                        else if (!line.equals("end")){
                            LoadGameConfirmPanel.addItemToCombobox(line);
                        }

                        line = SetConnection.getLine();
                        i++;
                        System.out.println(i + " PRIJMAM: " + line);
                    }
                }
                break;
        }
        return i;
    }
}