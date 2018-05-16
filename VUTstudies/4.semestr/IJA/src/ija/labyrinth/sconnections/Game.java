package ija.labyrinth.sconnections;

import ija.labyrinth.board.MazeBoard;
import ija.labyrinth.board.MazeCard;

import java.io.*;
import java.net.Socket;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Random;

/**
 * Projekt: IJA - Labyrinth
 * Probihaji zde veskere vypocty pro hru a uchovani informaci pro hru
 * @author xbayer05, xjehla00
 */
public class Game {
    private static int numberOfPlayers = 0;
    private static int gameSize = 0;
    private static List<Player> playerList = new ArrayList<Player>();
    private static int gameState;
    private static int turnNumber = 0;
    private ArrayList<Integer> fieldMazes = new ArrayList<Integer>();
    private ArrayList<Integer> fieldMazesRotation = new ArrayList<Integer>();
    private ArrayList<Integer> fieldTreasuresX = new ArrayList<Integer>();
    private static int prevIndexPlayer = turnNumber - 1;
    private static int findedTreasure = -1;

    private static MazeBoard mb;

    private static ArrayList<Boolean> booleanMap = new ArrayList<Boolean>();

    private static boolean isNewGame = true;

    /**
     * konstruktor hry
     * @param numberOfPlayersIn = pocet hracu
     * @param gameSizeIn        = velikost hraci plochy
     */
    public Game(int numberOfPlayersIn, int gameSizeIn){
        numberOfPlayers = numberOfPlayersIn;
        gameSize = gameSizeIn;
    }


    public String getSearchedTrNewLoad(int indexPlayer){
        return playerList.get(indexPlayer).getSearchedTreasures();
    }

    public String getTrNewLoad(int indexPlayer){
        return playerList.get(indexPlayer).getTrStringNewLoad();
    }

    /**
     * Provede inicializaci seznamu pokladu
     */
    public void initFieldTrX(){
        int iter = 0;

        if (gameSize == 5)
            iter = 12;
        else
            iter = 24;

        for (int i = 0; i < iter; i++) {
            fieldTreasuresX.add(-1);
        }
    }

    /**
     * Nastavi jestli se jedna o load, nebo novou hru
     * @param state load=false, new=true
     */
    public void setIsNewGame(boolean state){
        isNewGame = state;
    }

    /**
     * Vraci true kdyz se jedna o novou hru a ne o load
     * @return load=false, new=true
     */
    public boolean getIsNewGmae(){
        return isNewGame;
    }

    /**
     * Nastavi cislo hrace ktery je na tahu
     * @param number cislo
     */
    public void setTurnNumber(int number){
        turnNumber = number;
    }

    /**
     * Nastavi volny kamen v MazeBoard
     * @param mazeType typ
     * @param mazeTreasure poklad
     * @param mazeRotation rotace
     */
    public void setFreeCard(char mazeType, int mazeTreasure,int mazeRotation){
        mb.setFreeCard(mazeType, mazeTreasure, mazeRotation);
    }

    /**
     * Nacte kameny ze souboru do hraci desky
     * @param mazeArray seznam kamenu a rotaci....typ,rotace;typ;rotace;
     */
    public void setBoardFromLoad(String[] mazeArray){
        char mazeTypeChar = 'C';
        String[] sutr;

        for (int i = 0; i < gameSize * gameSize; i++) {
            sutr = mazeArray[i].split(",");

            switch (Integer.parseInt(sutr[0])){
                case 1:
                    mazeTypeChar = 'C';
                    break;
                case 2:
                    mazeTypeChar = 'L';
                    break;
                case 3:
                    mazeTypeChar = 'F';
                    break;
            }

            mb.setMazeCardToPosition(i / gameSize, i % gameSize, mazeTypeChar, Integer.parseInt(sutr[1]));
            //mb.get(i / gameSize + 1, i % gameSize + 1).putCard(MazeCard.createRotate(mazeTypeChar, Integer.parseInt(sutr[1])));
        }
    }

    /**
     * Nacte ulozene poklady do hraciho pole
     * @param treasureArray pole pokladu
     */
    public void setTreasuresFromLoad(String[] treasureArray){
        int index;

        for (int i = 0; i < treasureArray.length; i++) {
            index = Integer.parseInt(treasureArray[i]);
            mb.get(index / gameSize + 1, index % gameSize + 1).getCard().setTreasure(i);
        }
    }

    /**
     * metoda ulozeni
     */
    public void saveAll(){

        String finalString = "";
        finalString += "SAVE GAME\n";
        finalString += numberOfPlayers + "\n";
        finalString += gameSize + "\n";

        for (int i = 0; i < numberOfPlayers; i++) {
            finalString += getInfoAboutPlayer(i) + ";";
            finalString += getScoreFromPlayer(i) + ";";
            finalString += playerList.get(i).getTrString() + "\n";
        }

        finalString += getTurnNumber() + ";" + gameState + ";" + mb.getFreeCard().getMazeType() + ";" +
                mb.getFreeCard().getTreasure() + ";" + mb.getFreeCard().getMazeRound() + "\n";

        finalString += getMazesInfo() + "\n";
        finalString += getFieldsTreasuresX();

        DateFormat df = new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss");
        String sdt = df.format(new Date(System.currentTimeMillis()));
        //System.out.println(sdt);

        try {
            PrintWriter out = new PrintWriter(new FileWriter("./examples/save_" + sdt + ".txt"));
            out.print(finalString);
            out.close();
        } catch (IOException e) {
            //System.out.println("Do souboru se nepodarilo psat.");
            e.printStackTrace();
        }
    }

    /**
     * otoci volny kamen
     * @param rotation = stupen rotace
     */
    public void setRotationFreeMaze(int rotation){
        while (mb.getFreeCard().getMazeRound() != rotation){
            mb.getFreeCard().turnRight();
        }
    }

    /**
     * zkontroluje, zdali je na danem poli poklad, ktery hrac hleda
     * @param positionX     = X-ova souradnice
     * @param positionY     = Y-ova souradnice
     * @param indexPlayer   = index hrace, ktery slapl
     * @return vraci true/false stoji-li na pokladu
     */
    public boolean isThereTreasure(int positionX, int positionY, int indexPlayer){
        boolean win = false;

        if(getSearchTrFromPlayer(indexPlayer) == 24){
            switch (indexPlayer){
                case 0:
                    if (positionX == (gameSize - 1) && positionY == (gameSize - 1)){
                        win = true;
                    }
                    break;
                case 1:
                    if (positionX == (gameSize - 1) && positionY == 0){
                        win = true;
                    }
                    break;
                case 2:
                    if (positionX == 0 && positionY == 0){
                        win = true;
                    }
                    break;
                case 3:
                    if (positionX == 0 && positionY == (gameSize - 1)){
                        win = true;
                    }
                    break;
            }
        }
        else {
            int treasure = mb.get(positionY+1, positionX+1).getCard().getTreasure();
            //System.out.println("poklad: " + treasure);

            if(treasure == getSearchTrFromPlayer(indexPlayer)){
                findedTreasure = getSearchTrFromPlayer(indexPlayer);
                setSearchTrToPlayer(indexPlayer);
            }
        }

        return win;
    }

    /**
     * nastavi hraci novy hledany poklad
     * @param indexPlayer   = index daneho hrace
     */
    public void setSearchTrToPlayer(int indexPlayer){
        playerList.get(indexPlayer).setSearchTreasure();
    }

    /**
     * vrati vygenerovanou mapu, kam hrac muze slapnout
     * @return = mapa
     */
    public String getBooleanMap(){
        String tmpString = "";
        for (int i = 0; i < booleanMap.size(); i++) {
            if (booleanMap.get(i)){
                tmpString += "0,";
            }
            else {
                tmpString += "1,";
            }
        }
        return tmpString;
    }

    /**
     * nastavi mapu, kde se muze hrac pohybovat
     */
    public void initBooleanMap(){
        int size = booleanMap.size();
        for (int i = 0; i < size; i++) {
            booleanMap.remove(0);
        }

        for (int i = 0; i < gameSize * gameSize; i++) {
            booleanMap.add(true);
        }
    }

    /**
     * zjistuje pro generovani boolean mapy zdali jsou mozne pohyby danym smerem (plni boolean mapu)
     * @param positionX = X-ova pozice kamene
     * @param positionY = Y-ova pozice kamene
     */
    public void isPossibleMove(int positionX, int positionY){
        MazeCard act = mb.get(positionY, positionX).getCard();

        int setx = positionX - 1;
        int sety = positionY - 1;

        if (act.canGo(MazeCard.CANGO.LEFT) && (positionX - 1 > 0) && mb.get(positionY, positionX - 1).getCard().canGo(MazeCard.CANGO.RIGHT) && (booleanMap.get(sety * gameSize + setx - 1))){
            booleanMap.set(sety * gameSize + setx - 1, false);
            isPossibleMove(positionX - 1, positionY);
        }
        if (act.canGo(MazeCard.CANGO.UP) && (positionY - 1 > 0) && mb.get(positionY - 1, positionX).getCard().canGo(MazeCard.CANGO.DOWN) && (booleanMap.get((sety - 1) * gameSize + setx))){
            booleanMap.set((sety - 1) * gameSize + setx, false);
            isPossibleMove(positionX, positionY - 1);
        }
        if (act.canGo(MazeCard.CANGO.RIGHT) && (positionX + 1 <= gameSize) && mb.get(positionY, positionX + 1).getCard().canGo(MazeCard.CANGO.LEFT) && (booleanMap.get(sety * gameSize + setx + 1))){
            booleanMap.set(sety * gameSize + setx + 1, false);
            isPossibleMove(positionX + 1, positionY);
        }
        if (act.canGo(MazeCard.CANGO.DOWN) && (positionY + 1 <= gameSize) && mb.get(positionY + 1, positionX).getCard().canGo(MazeCard.CANGO.UP) && (booleanMap.get((sety + 1) * gameSize + setx))){
            booleanMap.set((sety + 1) * gameSize + setx, false);
            isPossibleMove(positionX, positionY + 1);
        }
    }

    /**
     * Nastavi mapu pruchodu
     */
    public void setBooleanMap(){
        for (int i = 0; i < gameSize * gameSize; i++) {
            booleanMap.set(i, true);
        }

        int x = getXFromPlayer(Protocol.getActulPlayer());
        int y = getYFromPlayer(Protocol.getActulPlayer());

        booleanMap.set(y * gameSize + x, false);

        isPossibleMove(x + 1, y + 1);
    }

    /**
     * nastavi hracum karty, ktere maji hledat
     */
    public void setCardsToPlayers(){
        int packSize = mb.getSizeCardPack();
        List<Integer> tmpList = new ArrayList<Integer>();
        List<Integer> tmpList2 = new ArrayList<Integer>();
        Random random = new Random();
        int randomIndex = 0;

        for(int i = 0; i < packSize; i++){
            tmpList.add(i);
        }

        for (int i = 0; i < packSize; i++){
            randomIndex = random.nextInt(tmpList.size());
            tmpList2.add(tmpList.get(randomIndex));
            tmpList.remove(randomIndex);
        }

        int numberOfCards = packSize/numberOfPlayers;
        for(int i = 0; i < packSize; i++){
            setTreasureToPlayer((i/numberOfCards),tmpList2.get(i));
        }

        for (int i = 0; i < numberOfPlayers; i++) {
            setTreasureToPlayer(i, 24);
        }
    }

    /**
     * zamicha karty
     */
    public void treasureShuffle(){
        Random random = new Random();
        List<Integer> tmpList = new ArrayList<Integer>();
        int randomIndex;

        for(int i = 1; i < gameSize*gameSize; i++){
            if(i != gameSize-1 && i != (gameSize*gameSize-gameSize) && i != gameSize*gameSize-1){
                tmpList.add(i);
            }
        }

        for (int i = 0; i < mb.getSizeCardPack(); i++){
            randomIndex = random.nextInt(tmpList.size());
            fieldTreasuresX.add(tmpList.get(randomIndex));
            tmpList.remove(randomIndex);
        }

        for (int i = 0; i < fieldTreasuresX.size(); i++){
            mb.get(fieldTreasuresX.get(i)/gameSize + 1, fieldTreasuresX.get(i)%gameSize + 1).getCard().setTreasure(i);
        }
    }

    /**
     * nastavi hraci kameny na hraci plose
     */
    public void setFieldMazes(){
        int size = fieldMazes.size();
        for (int i = 0; i < size; i++) {
            fieldMazes.remove(0);
            fieldMazesRotation.remove(0);
        }

        for (int y = 1; y <= gameSize; y++){
            for (int x = 1; x <= gameSize; x++){
                switch (mb.get(y,x).getCard().getMazeType()){
                    case 'C':
                        fieldMazes.add(1);
                        break;
                    case 'L':
                        fieldMazes.add(2);
                        break;
                    case 'F':
                        fieldMazes.add(3);
                        break;
                }
                fieldMazesRotation.add(mb.get(y,x).getCard().getMazeRound());
            }
        }
    }

    /**
     * ziska informace o kamenech na hraci desce pro hrace
     * @return  = informace o kamenech (typ, natoceni)
     */
    public String getMazesInfo(){
        String tmpString = "";
        for(int i = 0; i < fieldMazes.size(); i++){
            tmpString += fieldMazes.get(i) + "," + fieldMazesRotation.get(i) + ";";
        }
        return tmpString;
    }

    /**
     * vytvori hraci desku
     * @param isNewGame boolean jestli se jedna o novou hru nebo load game
     */
    public void createMazeBoard(boolean isNewGame){
        mb = MazeBoard.createMazeBoard(gameSize);
        if(isNewGame)
            mb.newGame();
    }

    /**
     * ziska veskere informace o hraci potrebne pro prenos
     * @param indexPlayer   = index hrace o kterem dostavame info
     * @return              = info daneho hrace
     */
    public String getInfoAboutPlayer(int indexPlayer){
        String tmpString = (indexPlayer + 1) + ";" + getKriglFromPlayer(indexPlayer) + ";" + getXFromPlayer(indexPlayer) + ";" +
                getYFromPlayer(indexPlayer) + ";"  + getSearchTrFromPlayer(indexPlayer);
        return tmpString;
    }

    /**
     * vraci informace o tom, kdo je na tahu, jestli byl nalezen poklad a jaky je volny kamen
     * @return = informace
     */
    public String getInfoAboutPlayer2(){
        int freeMaze = 0;
        int freeMazeRound;
        freeMazeRound = mb.getFreeCard().getMazeRound();
        int poklad = mb.getFreeCard().getTreasure();


        switch (mb.getFreeCard().getMazeType()){
            case 'C':
                freeMaze = 1;
                break;
            case 'L':
                freeMaze = 2;
                break;
            case 'F':
                freeMaze = 3;
                break;
        }

        String tmpString = incrementAndGetTurnNumber() + ";" + getScoreFromPlayer(prevIndexPlayer) + ";"  +
                findedTreasure + ";" + incrementAndGetGameState() + ";" + freeMaze + ";" + poklad + ";" + freeMazeRound;
        if(findedTreasure != -1) findedTreasure = -1;
        return tmpString;
    }

    /**
     * ziska veskere informace o hraci potrebne pro prenos
     * @param indexPlayer   = index hrace o kterem dostavame info
     * @return              = info daneho hrace
     */
    public String getNewLoadInfoPlayer(int indexPlayer){
        String tmpString = (indexPlayer + 1) + ";" + getKriglFromPlayer(indexPlayer) + ";" + getXFromPlayer(indexPlayer) + ";" +
                getYFromPlayer(indexPlayer) + ";" + getSearchTrFromPlayer(indexPlayer) + ";" + getScoreFromPlayer(indexPlayer) + ";" +
                getSearchedTrNewLoad(indexPlayer);
        return tmpString;
    }

    /**
     * vraci informace o tom, kdo je na tahu, jestli byl nalezen poklad a jaky je volny kamen
     * @param indexPlayer index hrace
     * @return informace o hre
     */
    public String getNewLoadInfoPlayer2(int indexPlayer){
        int freeMaze = 0;
        int freeMazeRound;
        freeMazeRound = mb.getFreeCard().getMazeRound();
        int poklad = mb.getFreeCard().getTreasure();


        switch (mb.getFreeCard().getMazeType()){
            case 'C':
                freeMaze = 1;
                break;
            case 'L':
                freeMaze = 2;
                break;
            case 'F':
                freeMaze = 3;
                break;
        }

        String tmpString = getTurnNumber() + ";" + getGameState() + ";" + freeMaze + ";" + poklad + ";" + freeMazeRound;
        if(findedTreasure != -1) findedTreasure = -1;
        return tmpString;
    }

    /**
     * nastavuje danemu hraci jeho figurku
     * @param indexPlayer   = index daneho hrace
     * @param krigl         = krigl, jenz se mu ma nastavit
     */
    public void setKriglToPlayer(int indexPlayer ,int krigl){
        playerList.get(indexPlayer).setKrigl(krigl);
    }

    /**
     * vraci krigl od daneho hrace
     * @param indexPlayer   = index daneho hrace
     * @return              = jeho krigl
     */
    public int getKriglFromPlayer(int indexPlayer){
        return playerList.get(indexPlayer).getKrigl();
    }

    /**
     * nastavuje hraci Xovou pozici na hraci plose
     * @param indexPlayer   = index hrace
     * @param positionX     = X-ova souradnice
     */
    public void setXToPlayer(int indexPlayer ,int positionX){
        playerList.get(indexPlayer).setPositionX(positionX);
    }

    /**
     * vraci X-ovou souradnici daneho hrace
     * @param indexPlayer   = index daneho hrace
     * @return              = X-ova souradnice
     */
    public int getXFromPlayer(int indexPlayer){
        return playerList.get(indexPlayer).getPositionX();
    }

    /**
     * nastavuje hraci Yovou pozici na hraci plose
     * @param indexPlayer   = index hrace
     * @param positionY     = Y-ova souradnice
     */
    public void setYToPlayer(int indexPlayer ,int positionY){
        playerList.get(indexPlayer).setPositionY(positionY);
    }

    /**
     * vraci Y-ovou souradnici daneho hrace
     * @param indexPlayer   = index daneho hrace
     * @return              = Y-ova souradnice
     */
    public int getYFromPlayer(int indexPlayer){
        return playerList.get(indexPlayer).getPositionY();
    }

    /**
     * Nastavi danemu hraci jeho score
     * @param indexPlayer   = index daneho hrace
     * @param score         = score hrace
     */
    public void setScoreToPlayer(int indexPlayer ,int score){
        playerList.get(indexPlayer).setScore(score);
    }

    /**
     * ziska skore od daneho hrace
     * @param indexPlayer   = index daneho hrace
     * @return              = jeho score
     */
    public int getScoreFromPlayer(int indexPlayer){
        if (indexPlayer == -1){
            int lastPlayer = playerList.size()-1;
            return playerList.get(lastPlayer).getScore();
        }else {
            return playerList.get(indexPlayer).getScore();
        }
    }

    /**
     * nastavi danemu hraci hledany poklad - stare
     * @param indexPlayer   = index daneho hrace
     * @param treasure      = poklad, ktery se ma nastavit
     */
    public void setSearchTrToPlayer(int indexPlayer ,int treasure){
        playerList.get(indexPlayer).setSearchTreasure(treasure);
    }

    /**
     * ziska hledany poklad daneho hrace
     * @param indexPlayer   = index hrace
     * @return              = poklad, ktery hleda
     */
    public int getSearchTrFromPlayer(int indexPlayer){
        return playerList.get(indexPlayer).getSearchTreasure();
    }

    /**
     * nastavi hraci poklad
     * @param indexPlayer   = index hrace
     * @param treasure      = poklad
     */
    public void setTreasureToPlayer(int indexPlayer ,int treasure){
        playerList.get(indexPlayer).setTreasures(treasure);
    }

    /**
     * Ziska poklad od hrace - stare
     * @param indexPlayer   = index daneho hrace
     * @return              = jeho poklad
     */
    public String getTreasureFromPlayer(int indexPlayer){
        int tmpInt;
        if (indexPlayer == -1){
            int lastPlayer = playerList.size()-1;
            tmpInt = playerList.get(lastPlayer).getTreasures();
        }else {
            tmpInt = playerList.get(indexPlayer).getTreasures();
        }
        return Integer.toString(tmpInt);
    }

    /**
     * nastavi seznam hracu
     * @param socket    = socket daneho hrace, ktery se zada do seznamu hracu
     * @return          = hodnota, zdali se povedlo
     */
    public boolean setPlayerList(Socket socket){
        int indexPlayer = playerList.size() + 1;
        if (indexPlayer <= numberOfPlayers) {
            Player player = new Player(indexPlayer, socket);
            playerList.add(player);
            return true;
        }
        return false;
    }

    /**
     * ziska index od daneho hrace
     * @param socket    = socket hrace, ktery se hleda
     * @return          = index hrace
     */
    public int getIndexPlayer(Socket socket){
        int position = 0;
        for (Player gamer : playerList){
            //System.out.println("V cyklu jsem nasel socket: " + gamer.getSocket());
            if(gamer.getSocket() == socket){
                position = gamer.getIndexPlayer();
            }
        }
        return position - 1;
    }


    /**
     * nastavi pocet hracu - stare
     * @param numberOfPlayersIn = pocet hracu
     */
    public void setNumberOfPlayers(int numberOfPlayersIn){
        numberOfPlayers = numberOfPlayersIn;
    }

    /**
     * nastavi velikost hraci plochy
     * @param gameSizeIn = velikost plochy
     */
    public void setGameSize(int gameSizeIn){
        gameSize = gameSizeIn;
    }

    /**
     * nastavi stav hry (tah/vsunuti kamene)
     * @param stateIn   = stav
     */
    public void setGameState(int stateIn){
        gameState = stateIn;
    }

    /**
     * ziska pocet hracu
     * @return  = pocet hracu
     */
    public int getNumberOfPlayers(){
        return numberOfPlayers;
    }

    /**
     * ziska velikost hry
     * @return  = velikost hry
     */
    public int getGameSize(){
        return gameSize;
    }

    /**
     * ziska stav hry
     * @return  = stav hry
     */
    public int getGameState(){
        return gameState;
    }

    /**
     * vrati cislo tahu
     * @return  = cislo tahu
     */
    public int getTurnNumber(){
        return turnNumber;
    }

    /**
     * zvysi a vrati cislo tahu
     * @return  = cislo tahu
     */
    public int incrementAndGetTurnNumber(){
        if(gameState == 0 || gameState == 2){
            turnNumber++;
        }
        if(turnNumber > numberOfPlayers)
            turnNumber = 1;
        return turnNumber;
    }

    /**
     * zvysi a vrati stav hry
     * @return  = stav hry
     */
    public int incrementAndGetGameState(){
        gameState++;
        if(gameState == 3)
            gameState = 1;
        return gameState;
    }

    /**
     * nastavi kameny v polickach - stare
     * @param mazeIn = cislo kamene
     */
    public void setFieldMazes(int mazeIn){
        fieldMazes.add(mazeIn);
    }

    /**
     * odstrani kameny z policek - stare
     */
    public void removeFieldMazs(){
        for(int i : fieldMazes) {
            fieldMazes.remove(i);
        }
    }

    /**
     * ziska kameny z policek - stare
     * @return  = pole kamenu
     */
    public ArrayList<Integer> getFieldMazes(){
        return fieldMazes;
    }

    /**
     * nastavi poklady - stare
     * @param treasuresIn = poklad
     */
    public void setFieldTreasuresX(int treasuresIn){
        fieldTreasuresX.add(treasuresIn);
    }

    /**
     * odstrani poklady - stare
     */
    public void removeFieldTreasuresX(){
        for(int i : fieldTreasuresX) {
            fieldTreasuresX.remove(i);
        }
    }

    /**
     * vlozi kamen na dany index
     * @param x = X-ova souradnice
     * @param y = Y-ova souradnice
     */
    public void insertMaze(int x, int y){
        mb.shift(mb.get(x + 1,y + 1));
    }

    /**
     * posune pinclika hrace na danou pozici
     * @param positionX = X-ova souradnice
     * @param positionY = Y-ova souradnice
     */
    public void movePlayersPekinese(int positionX, int positionY){
        for (int j = 0; j < getNumberOfPlayers(); j++) {
            if ((positionX == 0) && (positionY == getYFromPlayer(j))){
                setXToPlayer(j, (getXFromPlayer(j) + 1) % getGameSize());
            }
            if ((positionX == getGameSize() - 1) && (positionY == getYFromPlayer(j))){
                if (getXFromPlayer(j) == 0){
                    setXToPlayer(j, getGameSize() - 1);
                }
                else{
                    setXToPlayer(j, getXFromPlayer(j) - 1);
                }
            }
            if ((positionY == 0) && (positionX == getXFromPlayer(j))){
                setYToPlayer(j, (getYFromPlayer(j) + 1) % getGameSize());
            }
            if ((positionY == getGameSize() - 1) && (positionX == getXFromPlayer(j))){
                if (getYFromPlayer(j) == 0){
                    setYToPlayer(j, getGameSize() - 1);
                }
                else{
                    setYToPlayer(j, getYFromPlayer(j) - 1);
                }
            }
        }
    }

    /**
     * vrati pole pokladu
     * @return  = pole pokladu
     */
    public String getFieldsTreasuresX(){
        int treasure;

        for (int row = 0; row < gameSize; row++){
            for (int col = 0; col < gameSize; col++){
                treasure = mb.get(row + 1, col + 1).getCard().getTreasure();
                if (treasure != -1){
                    fieldTreasuresX.set(treasure, row * gameSize + col);
                }
            }
        }

        String tmpString = "";

        for (int i : fieldTreasuresX){
            tmpString += i + ",";
        }

        return tmpString;
    }
}
