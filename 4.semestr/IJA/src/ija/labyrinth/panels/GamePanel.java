package ija.labyrinth.panels;

import ija.labyrinth.buttons.*;
import ija.labyrinth.labels.images.*;
import ija.labyrinth.labels.texts.BeerFillProcentaLabel;
import ija.labyrinth.labels.texts.OnTurnLablel;
import ija.labyrinth.labels.texts.ScoreLabel;

import javax.swing.*;
import java.awt.*;

/**
 * Panel zobrazujici hru
 * Created by xjehla00, xbayer05 on 6. 5. 2015.
 */
public class GamePanel extends JPanel {
    private static GameMazeButton[] gameMazeButtons;                // Pole hracich kamenu
    private static InsertMazeButton[] insertMazeButtonsUp;          // Pole vkladacich mist sezdola nahoru
    private static InsertMazeButton[] insertMazeButtonsRight;       // Pole vkladacich mist zprava doleva
    private static InsertMazeButton[] insertMazeButtonsDown;        // Pole vkladacich mist sezhora dolu
    private static InsertMazeButton[] insertMazeButtonsLeft;        // Pole vkladacich mist zleva doprava
    private static RotateButton rotateButton = new RotateButton();  // Tlacitko pro rotovani volneho kamene
    private static GameMenuButton gameMenuButton = new GameMenuButton(); // Tlacitko pro vyvolani menu
    private static GameMenuSubButton[] gameMenuSubButtons = new GameMenuSubButton[3]; // Tlacitka herniho menu

    private static int game_size;                                   // Velikost hraci plochy
    private static int numOfTreasures;                              // Pocet pokladu
    private static int numOfFindedCards = 0;                        // Pocet nalezenych pokladu
    private static int numberOfPlayer;                              // Cislo clienta
    private static int numOfCards;                                  // Pocet hledanych pokladu
    private static int[] treasuresPlayer1;                          // Poklady hrace1
    private static int[] treasuresPlayer2;                          // Poklady hrace2
    private static int[] treasuresPlayer3;                          // Poklady hrace3
    private static int[] treasuresPlayer4;                          // Poklady hrace4
    private static int gameFieldSize = 500;                         // Velikost hraci plochy
    private static int gameFieldPositionX = 65;                     // X-ova souradnice hraci plochy
    private static int gameFieldPositionY = 170;                    // Y-ova souradnice hraci plochy
    private static int gameMazeButtonIndex;                         // Index hraciho kamene
    private static int gameMazeButtonSize;                          // Velikost Maze
    private static int insertButtonsCount;                          // Pocet sipek pro vkladani kamene
    private static int positionTreasure;                            // Umisteni obrazku pokladu vuci kameni
    private static int sizeTreasure;                                // Velikost obrazku pokladu
    private static int movedPositionTreasure;                       // Umisteni posledniho obrazku pokladu
    private static int isOnTurn;                                    // Index hrace na tahu
    private static int rotationFreeMaze = 0;                        // Uchovava informaci o natoceni volneho kamene

    private static FreeMazeLabel freeMazeLabel = new FreeMazeLabel('C', 2);                 // Obrazek volneho kamene
    private static KryglLabel[] kryglLabels;                                                // Pole hracich kostech
    private static TreasureCardLabel treasureCardLabel = new TreasureCardLabel();           // Poklad na zobrazene karte
    private static StartPositionLabel[] startPositionLabels;                                // Pole startovacich pozic
    private static GameMazeLabel[] gameMazeLabels;                                          // Pole kamenu ve hre
    private static TreasureMazeLabel[] treasureMazeLabels;                                  // Pole vsech pokladu na hraci plose
    private static BeerFillLabel beerFillLabel = new BeerFillLabel();                       // Pozadi Pivo Score
    private static BeerFillProcentaLabel beerFillProcentaLabel = new BeerFillProcentaLabel();// Pivo score procenta
    private static ScoreLabel[] score;                                                       // Score pro jednotlive hrace
    private static OnTurnLablel onTurnLablel = new OnTurnLablel();                          // Zobrazuje, kdo je na tahu
    private static KryglPekineseLabel[] kryglPekineseLabels;                                // Figurky na hraci plose
    private static GameMenuLabel gameMenuLabel = new GameMenuLabel();                       // Menu pozadi

    private static boolean moveTurn;        // Urcuje, zdali hrac vsunuje kamen, nebo se posunuje po hraci plose
    private static boolean newgame = true;  // Indikuje, zdali je nova hra

    /**
     * Nastavi newgame na danou hodnotu
     * @param val = hodnota
     */
    public static void setNewgame(boolean val){
        newgame = val;
    }

    /**
     * Nastavi pocet hledanych pokladu na spravnou velikost
     * @param number = spravna velikost
     */
    public static void setNumOfCards(int number){
        numOfCards = number;
    }

    /**
     * Zinicializuje treasures pro hrace
     */
    public static void initTreasuresPlayers() {
        // Nastaveni maximalniho poctu nalezenych karet
        treasuresPlayer1 = new int[numOfCards];
        treasuresPlayer2 = new int[numOfCards];
        treasuresPlayer3 = new int[numOfCards];
        treasuresPlayer4 = new int[numOfCards];

        // Inicializace vsech ulovku
        inintTreasuresPlayer();
    }

    /**
     * Nastavi danemu hraci vsechny jeho jiz nalezene poklady
     * @param indexPlayer   = index hrace
     * @param treasures     = jeho poklady
     */
    public static void setTreasuresToPlayer(int indexPlayer, int[] treasures){
        switch (indexPlayer){
            case 1:
                for (int i = 0; i < treasures.length; i++) {
                    treasuresPlayer1[i] = treasures[i];
                    if (indexPlayer == numberOfPlayer){
                        numOfFindedCards++;
                        setPercentBeer();
                    }
                }
                break;
            case 2:
                for (int i = 0; i < treasures.length; i++) {
                    treasuresPlayer2[i] = treasures[i];
                    if (indexPlayer == numberOfPlayer){
                        numOfFindedCards++;
                        setPercentBeer();
                    }
                }
                break;
            case 3:
                for (int i = 0; i < treasures.length; i++) {
                    treasuresPlayer3[i] = treasures[i];
                    if (indexPlayer == numberOfPlayer){
                        numOfFindedCards++;
                        setPercentBeer();
                    }
                }
                break;
            case 4:
                for (int i = 0; i < treasures.length; i++) {
                    treasuresPlayer4[i] = treasures[i];
                    if (indexPlayer == numberOfPlayer){
                        numOfFindedCards++;
                        setPercentBeer();
                    }
                }
                break;
        }
    }

    /**
     * Nastavi cislo hraci
     * @param number    = Cislo hrace
     */
    public static void setNumberOfPlayer(int number){
        numberOfPlayer = number;
    }

    /**
     * Nastavi rotaci volneho kamene
     * @param rotation = stupen rotace
     */
    public static void setRotationFreeMaze(int rotation){
        rotationFreeMaze = rotation;
    }

    /**
     * Vrati hodnotu rotace volneho kamene
     * @return = stupen rotace
     */
    public static int getRotationFreeMaze(){
        return rotationFreeMaze;
    }

    /**
     * Nastavi nove rozmisteni kamenu na hraci pose
     * @param types = pole typu kamenu a jejich natoceni
     */
    public static void setNewMazeType(String[] types){
        String[] strVal;
        for (int i = 0; i < gameMazeLabels.length; i++) {
            strVal = types[i].split(",");
            switch (Integer.parseInt(strVal[0])){
                case 1:
                    gameMazeLabels[i].setMaze('C', Integer.parseInt(strVal[1]));
                    break;
                case 2:
                    gameMazeLabels[i].setMaze('L', Integer.parseInt(strVal[1]));
                    break;
                case 3:
                    gameMazeLabels[i].setMaze('F', Integer.parseInt(strVal[1]));
                    break;
            }
        }
    }

    /**
     * Nastavuje klikatelna pouze ta tlacitka, na ktera se muze hrac pohnout
     * @param hodnoty = hodnoty techto policek
     */
    public static void setClickableButtons(int[] hodnoty){
        for (int i = 0; i < game_size * game_size; i++) {
            if (hodnoty[i] == 1){
                gameMazeLabels[i].setEnabled(true);
                gameMazeButtons[i].setEnabled(true);
            }
            else {
                gameMazeLabels[i].setEnabled(false);
                gameMazeButtons[i].setEnabled(false);
            }
        }
    }

    /**
     * Vrati velikost obrazku pokladu
     * @return  = velikost
     */
    public static int getSizeTreasure(){
        return sizeTreasure;
    }

    /**
     * Ziska cislo hrace
     * @return  = cislo hrace
     */
    public static int getNumberOfPlayer(){
        return numberOfPlayer;
    }

    /**
     * Prida nove nalezeny poklad do seznamu nalezenych pokladu
     * @param player        = Hrac, ktery poklad nasel
     * @param treasureIndex = Index, ktery poklad byl nalezen
     */
    public static void setTreasuresPlayer(int player, int treasureIndex){
        if (player == numberOfPlayer){
            numOfFindedCards++;
            setPercentBeer();
        }
        switch (player){
            case 1:
                for (int i = 0; i < numOfCards; i++){
                    if(treasuresPlayer1[i] == -1){
                        treasuresPlayer1[i] = treasureIndex;
                        break;
                    }
                }
                break;
            case 2:
                for (int i = 0; i < numOfCards; i++){
                    if(treasuresPlayer2[i] == -1){
                        treasuresPlayer2[i] = treasureIndex;
                        break;
                    }
                }
                break;
            case 3:
                for (int i = 0; i < numOfCards; i++){
                    if(treasuresPlayer3[i] == -1){
                        treasuresPlayer3[i] = treasureIndex;
                        break;
                    }
                }
                break;
            case 4:
                for (int i = 0; i < numOfCards; i++){
                    if(treasuresPlayer4[i] == -1){
                        treasuresPlayer4[i] = treasureIndex;
                        break;
                    }
                }
                break;
        }
    }

    /**
     * inicializuje hracum poklady na -1
     */
    public static void inintTreasuresPlayer(){
        //System.out.println(numOfCards);
        for (int i = 0; i < numOfCards; i++){
            treasuresPlayer1[i] = -1;
            treasuresPlayer2[i] = -1;
            treasuresPlayer3[i] = -1;
            treasuresPlayer4[i] = -1;
        }
    }

    /**
     * Vrati obrazek volneho kamene
     * @return = obrazek volneho kamene
     */
    public static FreeMazeLabel getFreeMazeLabel(){
        return freeMazeLabel;
    }

    /**
     * nastavi volny kamen
     * @param type      = typ kamene
     * @param rotation  = rotace kamene
     */
    public static void setFreeMazeLabel(int type, int rotation){
        freeMazeLabel.chooseMaze(type);

        for (int i = 0; i < rotation; i++){
            freeMazeLabel.rotate();
        }

        rotationFreeMaze = rotation;
    }

    /**
     * Vrati velikost hraci plochy
     * @return = velikost hraci plochy
     */
    public static int getGameSize(){
        return game_size;
    }

    /**
     * Nastavi velikost hraci plochy
     * @param gameSize  = velikost plochy
     */
    public static void setGameSize(int gameSize){
        game_size = gameSize;
    }

    /**
     * Vrati tlacitko pro vkladani nahoru
     * @param index = poradi tlacitka zleva
     * @return      = tlacitko
     */
    public static InsertMazeButton getUpButton(int index){
        return insertMazeButtonsUp[index];
    }

    /**
     * Vrati tlacitko pro vkladani vpravo
     * @param index = poradi tlacitka zeshora
     * @return      = tlacitko
     */
    public static InsertMazeButton getRightButton(int index){
        return insertMazeButtonsRight[index];
    }

    /**
     * Vrati tlacitko pro vkladani dolu
     * @param index = poradi tlacitka zleva
     * @return      = tlacitko
     */
    public static InsertMazeButton getDownButton(int index){
        return insertMazeButtonsDown[index];
    }

    /**
     * Vrati tlacitko pro vkladani vlevo
     * @param index = poradi tlacitka seshora
     * @return      = tlacitko
     */
    public static InsertMazeButton getLeftButton(int index){
        return insertMazeButtonsLeft[index];
    }

    /**
     * Vrati hraci kamen
     * @param index = poradi hraciho kamene
     * @return      = hraci kamen
     */
    public static GameMazeButton getGameMazeButton(int index){
        return gameMazeButtons[index];
    }

    /**
     * Nastavi Layout na null
     */
    public GamePanel(){
        setLayout(null);
    }

    /**
     * Nastavime pevne hraci kameny
     */
    private static void setStaticMaze(){
        int gameFieldSize = 500;                            // Velikost hraci plochy
        int gameFieldPositionX = 65;                        // X-ova souradnice hraci plochy
        int gameFieldPositionY = 170;                       // Y-ova souradnice hraci plochy
        int gameMazeButtonSize = gameFieldSize / game_size; // Velikost Maze

        // Nastaveni rohovych/startovacich kamenu
        (gameMazeLabels[0] = new GameMazeLabel(gameFieldPositionX, gameFieldPositionY, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('C', 2);
        (gameMazeLabels[(game_size - 1)] = new GameMazeLabel(gameFieldPositionX + (game_size - 1) * gameMazeButtonSize, gameFieldPositionY, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('C', 3);
        (gameMazeLabels[(game_size - 1) * game_size] = new GameMazeLabel(gameFieldPositionX, gameFieldPositionY + (game_size - 1) * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('C', 1);
        (gameMazeLabels[(game_size - 1) * game_size + (game_size - 1)] = new GameMazeLabel(gameFieldPositionX + (game_size - 1) * gameMazeButtonSize, gameFieldPositionY + (game_size - 1) * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('C', 0);

        //new GameMazeButton(gameFieldPositionX + x * gameMazeButtonSize, gameFieldPositionY + y * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)

        numOfTreasures = 24;
        // Nastaveni ostatnich kamenu pro hru 5x5
        if (game_size == 5){
            // Naplneni ostatnich fixnich kamenu + rotace
            (gameMazeLabels[2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size] = new GameMazeLabel(gameFieldPositionX, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[2 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[2 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[4 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            numOfTreasures = 12;
        }
        // Nastaveni ostatnich kamenu pro hru 7x7
        else if (game_size == 7){
            // Naplneni ostatnich fixnich kamenu + rotace
            (gameMazeLabels[0 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[0 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);

            (gameMazeLabels[2 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[2 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[4 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);

            (gameMazeLabels[2 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[4 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[4 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);

            (gameMazeLabels[4 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[6 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[6 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
        }
        // Nastaveni ostatnich kamenu pro hru 9x9
        else if (game_size == 9){
            // Naplneni ostatnich fixnich kamenu + rotace
            (gameMazeLabels[0 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[0 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[0 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);

            (gameMazeLabels[2 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[4 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[6 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[2 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[4 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);

            (gameMazeLabels[2 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[4 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[4 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[6 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[6 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);

            (gameMazeLabels[8 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[8 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[8 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[6 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[6 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[4 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
        }
        // Nastaveni ostatnich kamenu pro hru 11x11
        else{
            // Naplneni ostatnich fixnich kamenu + rotace
            (gameMazeLabels[0 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[0 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[0 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[0 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 0 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[2 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);
            (gameMazeLabels[4 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 2);

            (gameMazeLabels[2 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[4 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[6 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[8 * game_size + 0] = new GameMazeLabel(gameFieldPositionX + 0 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[2 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[4 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[6 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);
            (gameMazeLabels[4 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 1);

            (gameMazeLabels[2 * game_size + 10] = new GameMazeLabel(gameFieldPositionX + 10 * gameMazeButtonSize, gameFieldPositionY + 2 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[4 * game_size + 10] = new GameMazeLabel(gameFieldPositionX + 10 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[6 * game_size + 10] = new GameMazeLabel(gameFieldPositionX + 10 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[8 * game_size + 10] = new GameMazeLabel(gameFieldPositionX + 10 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[4 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 4 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[6 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[8 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);
            (gameMazeLabels[6 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 3);

            (gameMazeLabels[10 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 10 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[10 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 10 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[10 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 10 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[10 * game_size + 8] = new GameMazeLabel(gameFieldPositionX + 8 * gameMazeButtonSize, gameFieldPositionY + 10 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[8 * game_size + 2] = new GameMazeLabel(gameFieldPositionX + 2 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[8 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[8 * game_size + 6] = new GameMazeLabel(gameFieldPositionX + 6 * gameMazeButtonSize, gameFieldPositionY + 8 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
            (gameMazeLabels[6 * game_size + 4] = new GameMazeLabel(gameFieldPositionX + 4 * gameMazeButtonSize, gameFieldPositionY + 6 * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('F', 0);
        }
        treasureMazeLabels = new TreasureMazeLabel[numOfTreasures];
    }

    /**
     * Vytvoreni komplet cele herni plochy
     */
    public void createGame(){
        add(gameMenuSubButtons[0] = new GameMenuSubButton("HlavniMenu", 335));
        add(gameMenuSubButtons[1] = new GameMenuSubButton("Ulozit", 293));
        add(gameMenuSubButtons[2] = new GameMenuSubButton("Zpet", 377));

        // Pridava menu label, ktery prekryje vse
        add(gameMenuLabel);
        setGameMenuUnvisible();

        // vykresleni menu tlacitka
        add(gameMenuButton);

        gameMazeButtonSize = gameFieldSize / game_size;     // Velikost Maze
        insertButtonsCount = game_size / 2;                 // Pocet sipek pro vkladani kamene

        sizeTreasure = gameMazeButtonSize / 2;                          // Velikost obrazku pokladu
        positionTreasure = sizeTreasure / 2;                            // Umisteni obrazku pokladu vuci kameni
        movedPositionTreasure = gameMazeButtonSize * (game_size - 1);   // Umisteni posledniho obrazku pokladu

        // Vytvoreni seznamu hracich kamenu a sipek
        gameMazeButtons = new GameMazeButton[game_size * game_size];
        insertMazeButtonsUp = new InsertMazeButton[insertButtonsCount];
        insertMazeButtonsRight = new InsertMazeButton[insertButtonsCount];
        insertMazeButtonsDown = new InsertMazeButton[insertButtonsCount];
        insertMazeButtonsLeft = new InsertMazeButton[insertButtonsCount];

        // Naplneni seznamu hracich kamenu novymi kameny
        for (int y = 0; y < game_size; y++){
            for (int x = 0; x < game_size; x++){
                add(gameMazeButtons[y * game_size + x] = new GameMazeButton(gameFieldPositionX + x * gameMazeButtonSize, gameFieldPositionY + y * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2));
            }
        }

        // Vytvoreni seznamu pozadi pro hraci kameny
        gameMazeLabels = new GameMazeLabel[game_size * game_size];

        // Vykresleni fixnich kamenu
        setStaticMaze();

        // Vykresleni figurek u hracu
        // TODO: smazat
        //setKryglLabels();
        for (int i = 0; i < SelectPlayersPanel.getNumOfPlayers(); i++) {
            add(kryglLabels[i]);
        }

        // inicializace hracich figurek na hraci plose
        if (newgame){
            initKryglPekineseLabels();
        }
        else {
            for (int i = 0; i < SelectPlayersPanel.getNumOfPlayers(); i++) {
                add(kryglPekineseLabels[i]);
            }
        }

        // Vykresleni startovacich pozic
        startPositionLabels = new StartPositionLabel[SelectPlayersPanel.getNumOfPlayers()];
        setStartPositionLabels();

        // Inicializace a vykresleni kamenu pokladu
        initTreasureMazes(sizeTreasure, sizeTreasure);
        //putTreasureMaze();

        // Vykresleni vkladacich sipek
        for (int i = 0; i < insertButtonsCount; i++){
            add(insertMazeButtonsUp[i] = new InsertMazeButton(InsertMazeButton.Direction.UP, gameFieldPositionX + gameMazeButtonSize + (i) * gameMazeButtonSize * 2, gameFieldPositionY + gameMazeButtonSize * game_size, gameMazeButtonSize - 2, gameMazeButtonSize / 2));
            add(insertMazeButtonsRight[i] = new InsertMazeButton(InsertMazeButton.Direction.RIGHT, gameFieldPositionX - gameMazeButtonSize / 2, gameFieldPositionY + gameMazeButtonSize + (i) * gameMazeButtonSize * 2, gameMazeButtonSize / 2, gameMazeButtonSize - 2));
            add(insertMazeButtonsDown[i] = new InsertMazeButton(InsertMazeButton.Direction.DOWN, gameFieldPositionX + gameMazeButtonSize + (i) * gameMazeButtonSize * 2, gameFieldPositionY - gameMazeButtonSize / 2, gameMazeButtonSize - 2, gameMazeButtonSize / 2));
            add(insertMazeButtonsLeft[i] = new InsertMazeButton(InsertMazeButton.Direction.LEFT, gameFieldPositionX + gameMazeButtonSize * game_size, gameFieldPositionY + gameMazeButtonSize + (i) * gameMazeButtonSize * 2, gameMazeButtonSize / 2, gameMazeButtonSize - 2));
        }

        numOfCards = numOfTreasures / SelectPlayersPanel.getNumOfPlayers(); // Zjisti pocet hledanych pokladu

        if (newgame){
            initTreasuresPlayers();
        }
        // TODO

        // Vykresleni volneho kamene a rotovaciho tlacitka
        add(freeMazeLabel);
        add(rotateButton);

        // Vykresleni hledaneho pokladu
        add(treasureCardLabel);
        // TODO: smazat
        //treasureCardLabel.chooseTreasureCardBeer(5);

        // Vypocet a zobrazeni procentualniho piva
        setPercentBeer();
        add(beerFillProcentaLabel);
        add(beerFillLabel);

        // vykresleni ostatnich kamenu
        setGameMazeLabels();

        // nastaveni poctu score Labelu
        if (newgame){
            initScoreLabel();
        }

        // Inicializace hrace na tahu
        isOnTurn = 1;

        // Inicializace hrace na tahu
        add(onTurnLablel);
        if (numberOfPlayer == isOnTurn){
            onTurnLablel.setActualPlayer(2);
            enableInsertButtons();
            disableMazeButtons();
        }
        else {
            onTurnLablel.setActualPlayer(1);
            disableInsertButtons();
            disableMazeButtons();
            disableRotateButton();
        }
    }

    /**
     * nastavuje hledany poklad
     * @param index = index pokladu
     */
    public static void setWantedTreasure(int index){
        treasureCardLabel.chooseTreasureCardBeer(index);
    }

    /**
     * nastavi hodnotu moveturn na zadanou hodnotu
     * @param value = hodnota
     */
    public static void setMoveTurn(boolean value){
        moveTurn = value;
    }

    /**
     * Nastavi aktivni tlacitka
     */
    public static void setActiveButtons(){
        if ((numberOfPlayer == isOnTurn) && (!moveTurn)){
            onTurnLablel.setActualPlayer(2);
            enableInsertButtons();
            enableRotateButton();
            enableMenuButton();
            disableMazeButtons();
        }
        else if ((numberOfPlayer == isOnTurn) && (moveTurn)){
            onTurnLablel.setActualPlayer(3);
            disableInsertButtons();
            enableMazeButtons();
            disableRotateButton();
            enableMenuButton();
        }
        else {
            onTurnLablel.setActualPlayer(1);
            disableInsertButtons();
            disableMazeButtons();
            disableRotateButton();
            enableGameMazeLabels();
            enableMenuButton();
        }
    }

    /**
     * povoli obrazky hracich kamenu
     */
    public static void enableGameMazeLabels(){
        for (int i = 0; i < game_size * game_size; i++) {
            gameMazeLabels[i].setEnabled(true);
        }
    }

    /**
     * Zviditelni pozadi a tlacitka menu
     */
    public static void setGameMenuVisible(){
        gameMenuLabel.setVisible(true);
        gameMenuSubButtons[0].setVisible(true);
        gameMenuSubButtons[1].setVisible(true);
        gameMenuSubButtons[2].setVisible(true);
    }

    /**
     * Zneviditelni pozadi a tlacitka menu
     */
    public static void setGameMenuUnvisible(){
        gameMenuLabel.setVisible(false);
        gameMenuSubButtons[0].setVisible(false);
        gameMenuSubButtons[1].setVisible(false);
        gameMenuSubButtons[2].setVisible(false);
    }

    /**
     * Zakaze klikat na Menu tlacitko
     */
    public static void disableMenuButton(){
        gameMenuButton.setVisible(false);
    }

    /**
     * Povoli klikat na Menu tlacitko
     */
    public static void enableMenuButton(){
        gameMenuButton.setVisible(true);
    }

    /**
     * Zakaze klikat na rotovaci tlacitko
     */
    public static void disableRotateButton(){
        rotateButton.setEnabled(false);
    }

    /**
     * Povoli klikat na rotovaci tlacitko
     */
    public static void enableRotateButton(){
        rotateButton.setEnabled(true);
    }

    /**
     * Zakaze klikani na vkladaci tlacitka
     */
    public static void disableInsertButtons(){
        for (int i = 0; i < game_size / 2; i++) {
            insertMazeButtonsUp[i].setEnabled(false);
            insertMazeButtonsDown[i].setEnabled(false);
            insertMazeButtonsLeft[i].setEnabled(false);
            insertMazeButtonsRight[i].setEnabled(false);
        }
    }

    /**
     * Zakaze klikani na Maze
     */
    public static void disableMazeButtons(){
        for (int i = 0; i < game_size * game_size; i++) {
            gameMazeButtons[i].setEnabled(false);
        }
    }

    /**
     * povoli klikani na vkladaci tlacitka
     */
    public static void enableInsertButtons(){
        for (int i = 0; i < game_size / 2; i++) {
            insertMazeButtonsUp[i].setEnabled(true);
            insertMazeButtonsDown[i].setEnabled(true);
            insertMazeButtonsLeft[i].setEnabled(true);
            insertMazeButtonsRight[i].setEnabled(true);
        }
    }

    /**
     * povoli klikani na Maze
     */
    public static void enableMazeButtons(){
        for (int i = 0; i < game_size * game_size; i++) {
            gameMazeButtons[i].setEnabled(true);
        }
    }

    /**
     * povoli klikat na tlacitko na urcitych souradnicich
     * @param coordinateX   = X-ova souradnice tlacitka
     * @param coordinateY   = Y-ova souradnice tlacitka
     */
    public static void enableMazebutton(int coordinateX, int coordinateY){
        gameMazeButtons[coordinateY * game_size + coordinateX].setEnabled(true);
    }

    /**
     * Vrati ikonu vybraneho hrace
     * @param index = poradi hrace
     * @return      = ikona daneho hrace
     */
    public static ImageIcon getPlayerKryglIcon(int index){
        return (ImageIcon) kryglLabels[index].getIcon();
    }

    /**
     * Vrati ikonu figurky aktualniho hrace
     * @return  = ikona hrace
     */
    public static ImageIcon getActualPlayerKryglIcon(){
        return getPlayerKryglIcon(isOnTurn - 1);
    }

    /**
     * Vraci index hrace na tahu
     * @return  = index
     */
    public static int getIsOnTurn(){
        return isOnTurn;
    }

    /**
     * Nastavi pri loadgame hrace na tahu
     * @param index = index hrace a tahu
     */
    public static void setIsOnTurn(int index){
        isOnTurn = index;
    }

    /**
     * nastavi jako aktivniho dalsiho hrace a pricte body
     * @param addScore skore k pridani
     * @param findedCard nalezena karta
     */
    public void setIsOnTurn(int addScore, int findedCard){
        setScore(isOnTurn, (score[isOnTurn - 1].getScore() - 25 + addScore));

        if (findedCard != -1){
            setTreasuresPlayer(isOnTurn, findedCard);

            printTreasuresPlayer();
        }

        isOnTurn = isOnTurn % SelectPlayersPanel.getNumOfPlayers() + 1;
    }

    /**
     * Nastavi hraci nove score
     * @param player    = Hrac, kteremu se meni score
     * @param newScore  = Nove score
     */
    public static void setScore(int player, int newScore){
        score[player - 1].setScore(newScore);
    }

    /**
     * Ziska score hrace na danem indexu
     * @param player    = index hrace
     * @return          = jeho score
     */
    public static int getScore(int player){
        return score[player - 1].getScore();
    }

    /**
     * Inicializace score popisku
     */
    public void initScoreLabel(){
        // Vykresleni score a inicializace
        int numberOfPlayers = SelectPlayersPanel.getNumOfPlayers();
        int indexPlayer = numberOfPlayer - 1;
        score = new ScoreLabel[numberOfPlayers];
        add(score[indexPlayer] = new ScoreLabel(658, 246, 24));
        for (int i = numberOfPlayer; i < numberOfPlayers + indexPlayer; i++){
            add(score[i % numberOfPlayers] = new ScoreLabel(203 + (i - numberOfPlayer) * 325, 122, 19));
        }
    }

    /**
     * Nastavi novou pozici figurce
     * @param index     = index hrace/figurky
     * @param positionX = X-ova souradnice
     * @param positionY = Y-ova souradnice
     */
    public static void setKryglPekineseLabel(int index, int positionX, int positionY){
        kryglPekineseLabels[index].setPosition(gameFieldPositionX + positionTreasure + positionX * (gameFieldSize / game_size), gameFieldPositionY + positionTreasure + positionY * (gameFieldSize / game_size));
    }

    /**
     * Nastavi pocet figurek
     */
    public void setNumberOfPekineseLabels(){
        kryglPekineseLabels = new KryglPekineseLabel[SelectPlayersPanel.getNumOfPlayers()];
    }

    /**
     * Nastavi figurky pri loadgame na nic
     * @param indexplayer index hrace
     */
    public void setDefaultPekineseLabel(int indexplayer){
        gameMazeButtonSize = gameFieldSize / game_size;                 // Velikost Maze
        sizeTreasure = gameMazeButtonSize / 2;                          // Velikost obrazku pokladu
        positionTreasure = sizeTreasure / 2;                            // Umisteni obrazku pokladu vuci kameni
        movedPositionTreasure = gameMazeButtonSize * (game_size - 1);   // Umisteni posledniho obrazku pokladu
        kryglPekineseLabels[indexplayer] = new KryglPekineseLabel(indexplayer, movedPositionTreasure, movedPositionTreasure, sizeTreasure, sizeTreasure);
    }

    /**
     * Inicializuje figurky na puvodni pozice
     */
    public void initKryglPekineseLabels(){
        // Vykresleni figurek u hracu a nastaveni figurek na HP
        int numberOfPlayers = SelectPlayersPanel.getNumOfPlayers();
        int indexPlayer = numberOfPlayer - 1;
        kryglPekineseLabels = new KryglPekineseLabel[numberOfPlayers];
        switch (numberOfPlayer){
            case 1:
                add(kryglPekineseLabels[indexPlayer] = new KryglPekineseLabel(indexPlayer, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                switch (numberOfPlayers){
                    case 4:
                        add(kryglPekineseLabels[indexPlayer + 3] = new KryglPekineseLabel(indexPlayer + 3, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                    case 3:
                        add(kryglPekineseLabels[indexPlayer + 2] = new KryglPekineseLabel(indexPlayer + 2, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                    case 2:
                        add(kryglPekineseLabels[indexPlayer + 1] = new KryglPekineseLabel(indexPlayer + 1, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        break;
                }
                break;
            case 2:
                add(kryglPekineseLabels[indexPlayer] = new KryglPekineseLabel(indexPlayer, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                switch (numberOfPlayers){
                    case 2:
                        add(kryglPekineseLabels[(indexPlayer + 1) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 1) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                    case 3:
                        add(kryglPekineseLabels[(indexPlayer + 1) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 1) % numberOfPlayers, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        add(kryglPekineseLabels[(indexPlayer + 2) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 2) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                    case 4:
                        add(kryglPekineseLabels[(indexPlayer + 1) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 1) % numberOfPlayers, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        add(kryglPekineseLabels[(indexPlayer + 2) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 2) % numberOfPlayers, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(kryglPekineseLabels[(indexPlayer + 3) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 3) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                }
                break;
            case 3:
                add(kryglPekineseLabels[indexPlayer] = new KryglPekineseLabel(indexPlayer, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                switch (numberOfPlayers){
                    case 3:
                        add(kryglPekineseLabels[(indexPlayer + 1) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 1) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(kryglPekineseLabels[(indexPlayer + 2) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 2) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        break;
                    case 4:
                        add(kryglPekineseLabels[(indexPlayer + 1) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 1) % numberOfPlayers, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(kryglPekineseLabels[(indexPlayer + 2) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 2) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(kryglPekineseLabels[(indexPlayer + 3) % numberOfPlayers] = new KryglPekineseLabel((indexPlayer + 3) % numberOfPlayers, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        break;
                }
                break;
            case 4:
                add(kryglPekineseLabels[indexPlayer] = new KryglPekineseLabel(indexPlayer, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                add(kryglPekineseLabels[(indexPlayer + 1) % 4] = new KryglPekineseLabel((indexPlayer + 1) % 4, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                add(kryglPekineseLabels[(indexPlayer + 2) % 4] = new KryglPekineseLabel((indexPlayer + 2) % 4, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                add(kryglPekineseLabels[(indexPlayer + 3) % 4] = new KryglPekineseLabel((indexPlayer + 3) % 4, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                break;
        }
    }

    /**
     * Nastavi pocet kryglu
     * @param number    = pocet
     */
    public static void setNumOfKryglLabel(int number){
        kryglLabels = new KryglLabel[number];
    }

    /**
     * Nastavi figurku aktualnimu hraci
     * @param indexKrygl    = index figurky
     */
    public static void setKryglLabelSelf(int indexKrygl){
        kryglLabels[numberOfPlayer - 1] = new KryglLabel(indexKrygl, 860, 180, 125, 100);
    }

    /**
     * Nastavi figurku jinemu hraci
     * @param indexPlayer   = index hrace
     * @param indexKrygl    = index figurky
     */
    public static void setKryglLabelOther(int indexPlayer, int indexKrygl){
        int numberOfPlayers = SelectPlayersPanel.getNumOfPlayers();

        if(indexPlayer - numberOfPlayer < 0){
            kryglLabels[indexPlayer] = new KryglLabel(indexKrygl, 250 + (numberOfPlayers + indexPlayer - numberOfPlayer) * 325, 30, 55, 50);
        }
        else {
            kryglLabels[indexPlayer] = new KryglLabel(indexKrygl, 250 + (indexPlayer - numberOfPlayer) * 325, 30, 55, 50);
        }
    }

    /**
     * Nastavi figurky hracum
     */
    public void setKryglLabels(){
        // Vykresleni figurek u hracu
        int numberOfPlayers = SelectPlayersPanel.getNumOfPlayers();
        int indexPlayer = numberOfPlayer - 1;
        kryglLabels = new KryglLabel[numberOfPlayers];
        add(kryglLabels[indexPlayer] = new KryglLabel(indexPlayer, 860, 180, 125, 100));
        for (int i = numberOfPlayer; i < numberOfPlayers + indexPlayer; i++){
            add(kryglLabels[i % numberOfPlayers] = new KryglLabel((i % numberOfPlayers), 250 + (i - numberOfPlayer) * 325, 30, 55, 50));
        }
    }

    /**
     * Nastavi procenta na pivu
     */
    public static void setPercentBeer(){
        beerFillLabel.setBeerFill((int) (10 - numOfFindedCards * 10 / numOfCards));
        beerFillProcentaLabel.setText((int)(numOfFindedCards * 100 / numOfCards) + "%");
    }

    /**
     * Nastavi ostatni kameny
     * Vykresli vsechny kameny
     */
    public void setGameMazeLabels(){
        // Vykresleni ostatnich kamenu
        for (int y = 0; y < game_size; y++){
            for (int x = 0; x < game_size; x++){
                gameMazeButtonIndex = y * game_size + x;
                if (gameMazeLabels[gameMazeButtonIndex] != null){
                    add(gameMazeLabels[gameMazeButtonIndex]);
                }
                else {
                    (gameMazeLabels[gameMazeButtonIndex] = new GameMazeLabel(gameFieldPositionX + x * gameMazeButtonSize, gameFieldPositionY + y * gameMazeButtonSize, gameMazeButtonSize - 2, gameMazeButtonSize - 2)).setMaze('L', 0);
                    add(gameMazeLabels[gameMazeButtonIndex]);
                }
            }
        }
    }

    /**
     * Nastavi startovaci pozice do hry podle toho, jaky se diva hrac
     */
    public void setStartPositionLabels(){
        // Pridani startovacich pozic do hry
        switch (numberOfPlayer){
            case 1:
                add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.BLUE, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                switch (SelectPlayersPanel.getNumOfPlayers()){
                    case 4:
                        add(startPositionLabels[3] = new StartPositionLabel(StartPositionLabel.colorStartMaze.YELLOW, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                    case 3:
                        add(startPositionLabels[2] = new StartPositionLabel(StartPositionLabel.colorStartMaze.RED, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                    case 2:
                        add(startPositionLabels[1] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        break;
                }
                break;
            case 2:
                add(startPositionLabels[1] = new StartPositionLabel(StartPositionLabel.colorStartMaze.BLUE, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                switch (SelectPlayersPanel.getNumOfPlayers()){
                    case 2:
                        add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                    case 3:
                        add(startPositionLabels[2] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.RED, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                    case 4:
                        add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.YELLOW, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(startPositionLabels[2] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        add(startPositionLabels[3] = new StartPositionLabel(StartPositionLabel.colorStartMaze.RED, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                }
                break;
            case 3:
                add(startPositionLabels[2] = new StartPositionLabel(StartPositionLabel.colorStartMaze.BLUE, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                switch (SelectPlayersPanel.getNumOfPlayers()){
                    case 3:
                        add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(startPositionLabels[1] = new StartPositionLabel(StartPositionLabel.colorStartMaze.RED, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        break;
                    case 4:
                        add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.RED, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        add(startPositionLabels[1] = new StartPositionLabel(StartPositionLabel.colorStartMaze.YELLOW, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                        add(startPositionLabels[3] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                        break;
                }
                break;
            case 4:
                add(startPositionLabels[0] = new StartPositionLabel(StartPositionLabel.colorStartMaze.GREEN, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                add(startPositionLabels[1] = new StartPositionLabel(StartPositionLabel.colorStartMaze.RED, gameFieldPositionX + positionTreasure + movedPositionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                add(startPositionLabels[2] = new StartPositionLabel(StartPositionLabel.colorStartMaze.YELLOW, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure, sizeTreasure, sizeTreasure));
                add(startPositionLabels[3] = new StartPositionLabel(StartPositionLabel.colorStartMaze.BLUE, gameFieldPositionX + positionTreasure, gameFieldPositionY + positionTreasure + movedPositionTreasure, sizeTreasure, sizeTreasure));
                break;
        }
    }

    /**
     * Nahraje a vykresli vsechny poklady
     * @param width     = sirka pokladu
     * @param height    = vyska pokladu
     */
    public void initTreasureMazes(int width, int height){
        for (int i = 0; i < numOfTreasures; i++){
            add(treasureMazeLabels[i] = new TreasureMazeLabel(i, 0, 0, width, height));
        }
    }

    /**
     * Nastaveni pokladu na hracim poli
     * @param index     = Index pokladu v originalnim seznamu pokladu
     * @param locationX = X-ova pozice pokladu
     * @param locationY = Y-ova pozice pokladu
     */
    public static void setTreasureMazes(int index, int locationX, int locationY){
        treasureMazeLabels[index].setLocation(locationX, locationY);
    }

    /**
     * zajisti spravne vykresleni ulovku na spravne pozice
     * @param player1 = prvni seznam ulovku pro hrace co se diva
     * @param player2 = druhy seznam ulovku pro hrace cislo 2 na herni obrazovce
     * @param player3 = treti seznam ulovku pro hrace cislo 3 na herni obrazovce
     * @param player4 = ctvrty seznam ulovku pro hrace cislo 4 na herni obrazovce
     */
    public void printTreasuresPlayerWhile(int[] player1, int[] player2, int[] player3, int[] player4){
        int getTreasureSize = 27;                           // Velikost ikon ulovku
        int indexTreasure = 0;

        while (indexTreasure < numOfCards) {
            if (player1[indexTreasure] != -1){
                add(new TreasureMazeLabel(player1[indexTreasure], 680 + indexTreasure * getTreasureSize, 300, getTreasureSize, getTreasureSize));
                //setTreasureMazes(player1[indexTreasure], 680 + indexTreasure * getTreasureSize, 300, getTreasureSize, getTreasureSize);
            }
            if (player2[indexTreasure] != -1){
                if (indexTreasure < 6){
                    add(new TreasureMazeLabel(player2[indexTreasure], 75 + indexTreasure * getTreasureSize, 71, getTreasureSize, getTreasureSize));
                    //setTreasureMazes(player2[indexTreasure], 75 + indexTreasure * getTreasureSize, 71, getTreasureSize, getTreasureSize);
                }
                else {
                    add(new TreasureMazeLabel(player2[indexTreasure], 75 + (indexTreasure - 6) * getTreasureSize, 71 + getTreasureSize, getTreasureSize, getTreasureSize));
                    //setTreasureMazes(player2[indexTreasure], 75 + (indexTreasure - 6) * getTreasureSize, 71 + getTreasureSize, getTreasureSize, getTreasureSize);
                }
            }
            if (player3[indexTreasure] != -1){
                if (indexTreasure < 6){
                    add(new TreasureMazeLabel(player3[indexTreasure], 402 + indexTreasure * getTreasureSize, 71, getTreasureSize, getTreasureSize));
                    //setTreasureMazes(player3[indexTreasure], 402 + indexTreasure * getTreasureSize, 71, getTreasureSize, getTreasureSize);
                }
                else {
                    add(new TreasureMazeLabel(player3[indexTreasure], 402 + (indexTreasure - 6) * getTreasureSize, 71 + getTreasureSize, getTreasureSize, getTreasureSize));
                    //setTreasureMazes(player3[indexTreasure], 402 + (indexTreasure - 6) * getTreasureSize, 71 + getTreasureSize, getTreasureSize, getTreasureSize);
                }
            }
            if (player4[indexTreasure] != -1){
                if (indexTreasure < 6){
                    add(new TreasureMazeLabel(player4[indexTreasure], 725 + indexTreasure * getTreasureSize, 71, getTreasureSize, getTreasureSize));
                    //setTreasureMazes(player4[indexTreasure], 725 + indexTreasure * getTreasureSize, 71, getTreasureSize, getTreasureSize);
                }
                else {
                    add(new TreasureMazeLabel(player4[indexTreasure], 725 + (indexTreasure - 6) * getTreasureSize, 71 + getTreasureSize, getTreasureSize, getTreasureSize));
                    //setTreasureMazes(player4[indexTreasure], 725 + (indexTreasure - 6) * getTreasureSize, 71 + getTreasureSize, getTreasureSize, getTreasureSize);
                }
            }
            indexTreasure++;
        }
    }

    /**
     * Vytiskne ulovky
     */
    public void printTreasuresPlayer(){
        switch (numberOfPlayer){
            case 1:
                printTreasuresPlayerWhile(treasuresPlayer1, treasuresPlayer2, treasuresPlayer3, treasuresPlayer4);
                break;
            case 2:
                switch (SelectPlayersPanel.getNumOfPlayers()){
                    case 2:
                        printTreasuresPlayerWhile(treasuresPlayer2, treasuresPlayer1, treasuresPlayer3, treasuresPlayer4);
                        break;
                    case 3:
                        printTreasuresPlayerWhile(treasuresPlayer2, treasuresPlayer3, treasuresPlayer1, treasuresPlayer4);
                        break;
                    case 4:
                        printTreasuresPlayerWhile(treasuresPlayer2, treasuresPlayer3, treasuresPlayer4, treasuresPlayer1);
                        break;
                }
                break;
            case 3:
                switch (SelectPlayersPanel.getNumOfPlayers()){
                    case 3:
                        printTreasuresPlayerWhile(treasuresPlayer3, treasuresPlayer1, treasuresPlayer2, treasuresPlayer4);
                        break;
                    case 4:
                        printTreasuresPlayerWhile(treasuresPlayer3, treasuresPlayer4, treasuresPlayer1, treasuresPlayer2);
                        break;
                }
                break;
            case 4:
                printTreasuresPlayerWhile(treasuresPlayer4, treasuresPlayer1, treasuresPlayer2, treasuresPlayer3);
                break;
        }
    }

    /**
     * Vypise poklady na dana policka
     * @param positions = pozice pokladu
     */
    public static void putTreasureMaze(String[] positions){
        int gameMazeButtonSize = gameFieldSize / game_size; // Velikost Maze

        int sizeTreasure = gameMazeButtonSize / 2;          // Velikost obrazku pokladu
        int positionTreasure = sizeTreasure / 2;            // Umisteni obrazku pokladu vuci kameni

        for (int i = 0; i < numOfTreasures; i++) {
            setTreasureMazes(i, gameFieldPositionX + positionTreasure + (Integer.parseInt(positions[i]) % game_size) * gameMazeButtonSize, gameFieldPositionY + positionTreasure + (Integer.parseInt(positions[i]) / game_size) * gameMazeButtonSize);
        }
    }

    @Override
    protected void paintComponent(Graphics g){
        super.paintComponent(g);

        Image image = new ImageIcon("lib/Obr/UI/UI_Game.jpg").getImage();

        g.drawImage(image, 0, 0, 1024, 690, null);
    }
}
