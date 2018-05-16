package ija.labyrinth.board;

/**
 * Vytvoreni herniho pole
 * @author xbayer05, xjehla00
 */

import ija.labyrinth.treasure.CardPack;
import ija.labyrinth.treasure.Treasure;

import java.util.ArrayList;
import java.util.List;
import java.util.Random;

/**
 * Vytvoreni herniho pole
 * @author xbayer05, xjehla00
 */
public class MazeBoard {
    private int boardSize;
    private MazeField[][] fields;
    private MazeCard freeCard = null;
    private CardPack pack = null;
    private CardPack treasurePack;

    /**
     * Vytvori hraci desku
     * @param n = velikost hraci desky
     */
    public MazeBoard(int n){
        this.boardSize = n;
        this.fields = new MazeField[n][n];

        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                this.fields[i][j] = new MazeField(i+1, j+1);
    }

    /**
     * Vytvori herni desku o velikosti "n"
     * @param n velikost
     * @return hraci deska
     */
    public static MazeBoard createMazeBoard(int n) {
        return new MazeBoard(n);
    }

    /**
     * Vytvoreni herni desky s vlozenim
     * jednotlivych kamenu
     */
    public void newGame(){
        Random type = new Random();
        int valueRandom;

        int numC = 0;
        int numL = 0;
        int numF = 0;

        if (boardSize == 5){
            // Nastaveni poctu pokladu a vytvoreni pokladu
            MazeCard.setTreasureCount(12);
            //Treasure.createSet(12);

            this.treasurePack = new CardPack(12, 12);

            // Vytvori balicek pokladu
            this.pack = new CardPack(12, 12);

            // Naplneni rohovych kamenu a stredoveho kamenu + rotace
            fields[0][0].putCard(MazeCard.create180('C'));
            fields[0][4].putCard(MazeCard.createLeft('C'));
            fields[4][0].putCard(MazeCard.createRight('C'));
            fields[4][4].putCard(MazeCard.create('C'));

            // Naplneni ostatnich fixnich kamenu + rotace
            fields[0][2].putCard(MazeCard.create180('F'));
            fields[2][0].putCard(MazeCard.createRight('F'));
            fields[2][4].putCard(MazeCard.createLeft('F'));
            fields[2][2].putCard(MazeCard.create('F'));
            fields[4][2].putCard(MazeCard.create('F'));

            /**
            * Priradime pocty jednotlivych kamenu.
            * Format:
            * Celkovy pocet kamenu ve hre - fixni kameny + kameny navic
            */
            numC = 8 - 4 + 0;
            numL = 8 - 0 + 1;
            numF = 8 - 5 + 1;
        }
        else if (boardSize == 7){
            // Nastaveni poctu pokladu a vytvoreni pokladu
            MazeCard.setTreasureCount(24);
            //Treasure.createSet(24);

            this.treasurePack = new CardPack(24, 24);
            // Vytvori balicek pokladu
            this.pack = new CardPack(24, 24);

            // Naplneni rohovych kamenu + rotace
            fields[0][0].putCard(MazeCard.create180('C'));
            fields[0][6].putCard(MazeCard.createLeft('C'));
            fields[6][0].putCard(MazeCard.createRight('C'));
            fields[6][6].putCard(MazeCard.create('C'));

            // Naplneni ostatnich fixnich kamenu + rotace
            fields[0][2].putCard(MazeCard.create180('F'));
            fields[0][4].putCard(MazeCard.create180('F'));
            fields[2][4].putCard(MazeCard.create180('F'));
            fields[2][0].putCard(MazeCard.createRight('F'));
            fields[2][2].putCard(MazeCard.createRight('F'));
            fields[4][0].putCard(MazeCard.createRight('F'));
            fields[2][6].putCard(MazeCard.createLeft('F'));
            fields[4][6].putCard(MazeCard.createLeft('F'));
            fields[4][4].putCard(MazeCard.createLeft('F'));
            fields[4][2].putCard(MazeCard.create('F'));
            fields[6][2].putCard(MazeCard.create('F'));
            fields[6][4].putCard(MazeCard.create('F'));

            /**
             * Priradime pocty jednotlivych kamenu.
             * Format:
             * Celkovy pocet kamenu ve hre - fixni kameny + kameny navic
             */
            numC = 16 - 4  + 0;
            numL = 16 - 0  + 1;
            numF = 16 - 12 + 1;
        }
        else if (boardSize == 9){
            // Nastaveni poctu pokladu a vytvoreni pokladu
            MazeCard.setTreasureCount(24);
            //Treasure.createSet(24);

            this.treasurePack = new CardPack(24, 24);

            // Vytvori balicek pokladu
            this.pack = new CardPack(24, 24);

            // Naplneni rohovych kamenu + rotace
            fields[0][0].putCard(MazeCard.create180('C'));
            fields[0][8].putCard(MazeCard.createLeft('C'));
            fields[8][0].putCard(MazeCard.createRight('C'));
            fields[8][8].putCard(MazeCard.create('C'));

            // Naplneni ostatnich fixnich kamenu + rotace
            fields[0][2].putCard(MazeCard.create180('F'));
            fields[0][4].putCard(MazeCard.create180('F'));
            fields[0][6].putCard(MazeCard.create180('F'));
            fields[2][4].putCard(MazeCard.create180('F'));
            fields[2][6].putCard(MazeCard.create180('F'));
            fields[2][0].putCard(MazeCard.createRight('F'));
            fields[4][0].putCard(MazeCard.createRight('F'));
            fields[6][0].putCard(MazeCard.createRight('F'));
            fields[2][2].putCard(MazeCard.createRight('F'));
            fields[4][2].putCard(MazeCard.createRight('F'));
            fields[2][8].putCard(MazeCard.createLeft('F'));
            fields[4][6].putCard(MazeCard.createLeft('F'));
            fields[4][8].putCard(MazeCard.createLeft('F'));
            fields[6][6].putCard(MazeCard.createLeft('F'));
            fields[6][8].putCard(MazeCard.createLeft('F'));
            fields[8][2].putCard(MazeCard.create('F'));
            fields[8][4].putCard(MazeCard.create('F'));
            fields[8][6].putCard(MazeCard.create('F'));
            fields[6][2].putCard(MazeCard.create('F'));
            fields[6][4].putCard(MazeCard.create('F'));
            fields[4][4].putCard(MazeCard.create('F'));

            /**
             * Priradime pocty jednotlivych kamenu.
             * Format:
             * Celkovy pocet kamenu ve hre - fixni kameny + kameny navic
             */
            numC = 27 - 4  + 1;
            numL = 27 - 0  + 0;
            numF = 27 - 21 + 0;
        }
        else{
            // Nastaveni poctu pokladu a vytvoreni pokladu
            MazeCard.setTreasureCount(24);
            //Treasure.createSet(24);

            this.treasurePack = new CardPack(24, 24);

            // Vytvori balicek pokladu
            this.pack = new CardPack(24, 24);

            // Naplneni rohovych kamenu + rotace
            fields[0][0].putCard(MazeCard.create180('C'));
            fields[0][10].putCard(MazeCard.createLeft('C'));
            fields[10][0].putCard(MazeCard.createRight('C'));
            fields[10][10].putCard(MazeCard.create('C'));

            // Naplneni ostatnich fixnich kamenu + rotace
            fields[0][2].putCard(MazeCard.create180('F'));
            fields[0][4].putCard(MazeCard.create180('F'));
            fields[0][6].putCard(MazeCard.create180('F'));
            fields[0][8].putCard(MazeCard.create180('F'));
            fields[2][4].putCard(MazeCard.create180('F'));
            fields[2][6].putCard(MazeCard.create180('F'));
            fields[2][8].putCard(MazeCard.create180('F'));
            fields[4][6].putCard(MazeCard.create180('F'));
            fields[2][0].putCard(MazeCard.createRight('F'));
            fields[4][0].putCard(MazeCard.createRight('F'));
            fields[6][0].putCard(MazeCard.createRight('F'));
            fields[8][0].putCard(MazeCard.createRight('F'));
            fields[2][2].putCard(MazeCard.createRight('F'));
            fields[4][2].putCard(MazeCard.createRight('F'));
            fields[6][2].putCard(MazeCard.createRight('F'));
            fields[4][4].putCard(MazeCard.createRight('F'));
            fields[2][10].putCard(MazeCard.createLeft('F'));
            fields[4][10].putCard(MazeCard.createLeft('F'));
            fields[6][10].putCard(MazeCard.createLeft('F'));
            fields[8][10].putCard(MazeCard.createLeft('F'));
            fields[4][8].putCard(MazeCard.createLeft('F'));
            fields[6][8].putCard(MazeCard.createLeft('F'));
            fields[8][8].putCard(MazeCard.createLeft('F'));
            fields[6][6].putCard(MazeCard.createLeft('F'));
            fields[10][2].putCard(MazeCard.create('F'));
            fields[10][4].putCard(MazeCard.create('F'));
            fields[10][6].putCard(MazeCard.create('F'));
            fields[10][8].putCard(MazeCard.create('F'));
            fields[8][2].putCard(MazeCard.create('F'));
            fields[8][4].putCard(MazeCard.create('F'));
            fields[8][6].putCard(MazeCard.create('F'));
            fields[6][4].putCard(MazeCard.create('F'));

            /**
             * Priradime pocty jednotlivych kamenu.
             * Format:
             * Celkovy pocet kamenu ve hre - fixni kameny + kameny navic
             */
            numC = 40 - 4  + 0;
            numL = 40 - 0  + 1;
            numF = 40 - 32 + 1;
        }

        int FreeMazeCount = numC + numL + numF;

        for(int i = 0; i < boardSize; i++)
            for(int j = 0; j < boardSize; j++) {
                if((i % 2 == 0) && (j % 2 == 0)){
                    continue;
                }
                valueRandom = type.nextInt(FreeMazeCount - 1);

                if ((valueRandom >= 0) && (valueRandom < numC)){
                    fields[i][j].putCard(MazeCard.createRotate('C', type.nextInt(4)));
                    numC--;
                    FreeMazeCount--;
                }
                else if ((valueRandom >= numC) && (valueRandom < numC + numL)){
                    fields[i][j].putCard(MazeCard.createRotate('L', type.nextInt(4)));
                    numL--;
                    FreeMazeCount--;
                }
                else if ((valueRandom >= numC + numL) && (valueRandom < numC + numL + numF)){
                    fields[i][j].putCard(MazeCard.createRotate('F', type.nextInt(4)));
                    numF--;
                    FreeMazeCount--;
                }
                else{
                    fields[i][j].putCard(MazeCard.createRotate('C', type.nextInt(4)));
                }
            }
        if (numC > 0)
            freeCard = MazeCard.create('C');
        else if (numL > 0)
            freeCard = MazeCard.create('L');
        else
            freeCard = MazeCard.create('F');
    }

    /**
     * Nastavi volny kamen
     * @param mazeType typ
     * @param mazeTreasure poklad
     * @param mazeRotation rotace
     */
    public void setFreeCard(char mazeType, int mazeTreasure, int mazeRotation){
        freeCard = MazeCard.createRotate(mazeType, mazeRotation);
        freeCard.setTreasure(mazeTreasure);
    }

    /**
     * Vlozi a zrotuje kamen na dane policko
     * @param x x-souradnice
     * @param y y-souradnice
     * @param mazeTypeChar typ kamene
     * @param mazeRotate rotace kamene
     */
    public void insertMaze(int x, int y, char mazeTypeChar, int mazeRotate){
        if (mazeRotate == 0)
            fields[x][y].putCard(MazeCard.create(mazeTypeChar));
        else if (mazeRotate == 1)
            fields[x][y].putCard(MazeCard.createRight(mazeTypeChar));
        else if (mazeRotate == 2)
            fields[x][y].putCard(MazeCard.create180(mazeTypeChar));
        else if (mazeRotate == 3)
            fields[x][y].putCard(MazeCard.createLeft(mazeTypeChar));
    }

    /**
     * Vrati policko na zadane pozici
     * @param row =  radek
     * @param col sloupec
     * @return policko
     */
    public MazeField get(int row, int col){
        if(row-1 >= boardSize || col-1 >= boardSize || row-1 < 0 || col-1 < 0){
            return null;
        }
        return fields[row-1][col-1];
    }

    public void setMazeCardToPosition(int row, int col, char type, int rot){
        fields[row][col].putCard(MazeCard.createRotate(type, rot));
    }

    /**
     * Vrati velikost balicku karet
     * @return = velikost
     */
    public int getSizeCardPack(){
        return treasurePack.size();
    }

    /**
     * zamicha balicek karet - stare
     */
    public void shuffleCardPack(){
        treasurePack.shuffle();
    }

    /**
     * vrati balicek karet - stare
     * @return = balicek karet
     */
    public List<Integer> getCards(){
        List<Integer> tmpList = new ArrayList<Integer>();
        Treasure tmpTreasure;
        int tmpInt = 0;

        while((tmpTreasure = treasurePack.popCard().poklad) != null){
            tmpInt = tmpTreasure.getCode();
            tmpList.add(tmpInt);
        }

        return tmpList;
    }

    /**
     * Vraci volny kamen
     * @return kamen
     */
    public MazeCard getFreeCard(){
        return freeCard;
    }

    /**
     * Provadi posuvy radku a sloupci podle zadanych hodnot
     * @param mf policko
     */
    public void shift(MazeField mf){
        MazeCard tmp;
        if((mf.row() == 1) && (mf.col()%2 == 0)){
            tmp = get(boardSize, mf.col()).getCard();
            for(int i = boardSize-1; i > 0; i--){
                get(i+1, mf.col()).putCard(get(i, mf.col()).getCard());
            }
            get(1, mf.col()).putCard(freeCard);
            freeCard = tmp;
        }else if((mf.row() == boardSize) && (mf.col()%2 == 0)){
            tmp = get(1, mf.col()).getCard();
            for(int i = 0; i < boardSize-1; i++){
                get(i+1, mf.col()).putCard(get(i+2, mf.col()).getCard());
            }
            get(boardSize, mf.col()).putCard(freeCard);
            freeCard = tmp;
        }else if((mf.row()%2 == 0) && (mf.col() == 1)){
            tmp = get(mf.row(), boardSize).getCard();
            for(int i = boardSize-1; i > 0; i--){
                get(mf.row(), i+1).putCard(get(mf.row(), i).getCard());
            }
            get(mf.row(), 1).putCard(freeCard);
            freeCard = tmp;
        }else  if((mf.row()%2 == 0) && (mf.col() == boardSize)){
            tmp = get(mf.row(), 1).getCard();
            for(int i = 0; i < boardSize-1; i++){
                get(mf.row(), i+1).putCard(get(mf.row(), i+2).getCard());
            }
            get(mf.row(), boardSize).putCard(freeCard);
            freeCard = tmp;
        }
    }
}