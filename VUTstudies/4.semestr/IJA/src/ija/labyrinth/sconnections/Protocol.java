package ija.labyrinth.sconnections;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

/**
 * Projekt: IJA - Labyrinth
 * Trida protokol si rozparsovava zpavu od klienta,
 * vytahuje si potrebne informace a generuje odpoved
 * @author xbayer05, xjehla00
 */
public class Protocol {

    /**
     * konstruktor
     * @param socket    = socket hrace
     * @param indexCon  = index pripojeni
     */
    public Protocol(Socket socket, int indexCon){
        this.socket = socket;
        this.indexCon = indexCon;
    }

    private static Game game;

    private int indexCon;
    private Socket socket;

    static private boolean createGame = false;
    static private boolean joinGame = false;
    static private boolean readyCheck = false;
    static private boolean playGame = false;
    static private boolean newGame = false;
    static private boolean saveGame = false;
    static private boolean loadGame = false;
    static private boolean newLoad = false;
    static private boolean turn = false;
    static private boolean win = false;
    static private boolean end = false;

    static private int numberOfJoiners = 0;
    static private int numberOfPlayers = 0;

    int sizeGame = 0;
    static private int createGameIterator = 0;

    static private int getKrigl = -1;
    static private List<Integer> krigl = new ArrayList<Integer>();
    static private int iterator = 0;

    static private int actualPlayer = 0;

    static private int positionX;
    static private int positionY;

    static private int loadState;
    static private String[] loadArray;

    /**
     * vrati krigly hracu
     * @return  = seznam kriglu
     */
    static private String getKrigly(){
        String tmpString = "";
        for (int i = 0; i < krigl.size(); i++){
            tmpString += " " + krigl.get(i);
        }
        return tmpString;
    }

    /**
     * zjisti hlavicku protokolu
     * @param line  = overovaci zaznam
     */
    public void ParseLine(String line){
        if (line.equals("CREATE GAME")){
            createGame = true;
            numberOfJoiners++;
        }
        else if (line.equals("JOIN GAME")){
            joinGame = true;
            numberOfJoiners++;
        }
        else if (line.equals("READY CHECK")) readyCheck = true;
        else if (line.equals("PLAY GAME")) playGame = true;
        else if (line.equals("TURN")) turn = true;
        else if (line.equals("SAVE GAME")) saveGame = true;
        else if (line.equals("LOAD GAME")) loadGame = true;
        else if (line.equals("EXIT")) end = true;
    }

    /**
     * funkce zjistujici co serveru poslal klient
     * @param line  = radek zpravy
     * @param i     = index radku
     */
    public void FinalM(String line, int i){

        if (createGame){
            if(i == 2){
                numberOfPlayers = Integer.parseInt(line);
            }else if(i == 3){
                getKrigl = Integer.parseInt(line);
                //System.out.println("Hru vytvoril: " + socket);
            }else if(i == 4) {
                sizeGame = Integer.parseInt(line);
                game = new Game(numberOfPlayers, sizeGame);
                game.setPlayerList(socket);
                game.createMazeBoard(true);
                game.setFieldMazes();
                game.treasureShuffle();
                game.initBooleanMap();
            }
        }

        if (readyCheck){
            if(i == 2){
                actualPlayer = Integer.parseInt(line);
            }
            if(i == 3){
                getKrigl = Integer.parseInt(line);
            }
        }

        if (turn){
            // TODO posunout šutry / posunout hrace
            if (i == 3){
                positionY = Integer.parseInt(line);
            }
            if (i == 4){
                positionX = Integer.parseInt(line);
            }
            if (i == 5){
                game.setScoreToPlayer(actualPlayer, Integer.parseInt(line));
            }
            if (i == 6){
                if (game.getGameState() == 1){
                    game.setRotationFreeMaze(Integer.parseInt(line));
                    game.insertMaze(positionY, positionX);
                    game.setFieldMazes();
                    // Nastavi pro hrace posuv figurky
                    game.movePlayersPekinese(positionX, positionY);
                    game.setBooleanMap();
                } else {
                    game.setXToPlayer(actualPlayer, positionX);
                    game.setYToPlayer(actualPlayer, positionY);
                    win = game.isThereTreasure(positionX, positionY, actualPlayer);
                    turn = !win;
                    actualPlayer++;
                    actualPlayer = actualPlayer % game.getNumberOfPlayers();
                }
            }
        }

        if(saveGame){
            game.saveAll();
        }

        if(loadGame){
            if(i == 2){
                loadState = Integer.parseInt(line);
            }
            if(i == 3 && loadState == 2){
                setGameFromLoad(line);
                game.setIsNewGame(false); // nejedna se o novou hru
            }
        }
    }

    /**
     * ziska aktualniho hrace
     * @return  = aktualni hrac
     */
    public static int getActulPlayer(){
        return actualPlayer;
    }

    /**
     * generuje odpoved pro klienta
     * @return  = vygenerovana odpoved
     */
    public List<String> genAnswer(){
        List<String> tmpAnswer = new ArrayList<String>();
        String toClient = "";
        String toClinetType = "";

        if (createGame){
            if(createGameIterator == 0) {
                krigl.add(getKrigl);
                iterator++;
                toClient = "JOIN GAME\n";
                toClient += "ok\n";
                toClient += (game.getIndexPlayer(socket) + 1) + "\n";
                toClient += getKrigly() + "\n";
                toClient += "end";
                game.setKriglToPlayer(game.getIndexPlayer(socket), getKrigl);
                getKrigl = -1;
                toClinetType = "one";
            }else{
                toClient = "JOIN GAME\n";
                toClient += "nok\n";
                toClient += "game already exist\n";
                toClient += "end";
                toClinetType = "maxC";
                Server.removeCon(indexCon - 1);
            }
            createGameIterator++;
            createGame = false;
        }

        //System.out.println("Hdnota play game: " + playGame);
        if(playGame || turn){
            iterator = 0;
            if ((!game.getIsNewGmae()) && (newGame)) {
                toClient = "NEW LOAD\n";
                newGame = false;
            }else if (newGame){
                toClient = "NEW GAME\n";
                newGame = false;
                for (int j = 0; j < game.getNumberOfPlayers(); j++){
                    game.setXToPlayer(j, (game.getGameSize() - 1) * (((j / 2) + 1) % 2));
                    game.setYToPlayer(j, (game.getGameSize() - 1) * ((((j & 1) ^ (j & 2)) + 1) % 2));
                }
            }else{
                toClient = "PLAY GAME\n";
            }
            toClient += game.getNumberOfPlayers() + "\n";
            toClient += game.getGameSize() + "\n";
            while(iterator < numberOfPlayers){
                toClient += game.getInfoAboutPlayer(iterator) + "\n";
                iterator++;
            }
            toClient += game.getInfoAboutPlayer2() + "\n";
            toClient += game.getMazesInfo() + "\n";
            toClient += game.getFieldsTreasuresX() + "\n";
            if (game.getGameState() == 2){
                toClient += game.getBooleanMap() + "\n";
            }
            toClient += "end";
            toClinetType = "all";
            playGame = false;
        }

        if(newLoad){
            iterator = 0;
            toClient = "NEW LOAD\n";
            toClient += game.getNumberOfPlayers() + "\n";
            toClient += game.getGameSize() + "\n";

            while(iterator < numberOfPlayers){
                toClient += game.getNewLoadInfoPlayer(iterator) + "\n";
                iterator++;
            }
            toClient += game.getNewLoadInfoPlayer2(iterator) + "\n";
            toClient += game.getMazesInfo() + "\n";
            toClient += game.getFieldsTreasuresX() + "\n";

            toClient += "end";
            toClinetType = "all";
            newLoad = false;
        }

        if (joinGame){
            toClient = "JOIN GAME\n";
            if(numberOfJoiners <= game.getNumberOfPlayers()) {
                game.setPlayerList(socket);
                //System.out.println("Ke hre se pripojil: " + socket);
                if (game.getIsNewGmae()) {
                    toClient += "ok\n" + (game.getIndexPlayer(socket) + 1) + "\n" + getKrigly() + "\nend";
                    toClinetType = "one";
                }else{
                    toClient += "ok\n" + (game.getIndexPlayer(socket) + 1) + "\n" + "-1" + "\nend";
                    setPlayerFromLoad(game.getIndexPlayer(socket));
                    if (numberOfJoiners+1 == game.getNumberOfPlayers()){
                        //newGame = true;
                        //playGame = true;
                        newLoad = true;
                        toClinetType = "uniq";
                    }else{
                        toClinetType = "one";
                    }
                }
            } else {
                toClient += "nok\n";
                toClient += "no game\nend";
                toClinetType = "maxC";
                Server.removeCon(indexCon - 1);
            }
            joinGame = false;
        }

        if (win){
            toClient = "WIN\n";
            toClient += actualPlayer + "\n";
            toClient += "end";
            toClinetType = "all";
        }

        if (readyCheck){
            if (krigl.contains(getKrigl)){
                toClient = "READY CHECK\nnok\n" + getKrigly() + "\nend";
                toClinetType = "one";
            }else {
                iterator++;
                if (iterator == game.getNumberOfPlayers()){
                    playGame = true;
                    newGame = true;
                    iterator = 0;
                    game.setKriglToPlayer(game.getIndexPlayer(socket), getKrigl);
                    getKrigl = -1;
                    toClient = "READY CHECK\nok\nwaiting\nend";
                    toClinetType = "uniq";
                    game.setCardsToPlayers();
                }else {
                    krigl.add(getKrigl);
                    //toClient = "READY CHECK\nok\n" + getKrigly() + "\n" + game.getIndexPlayer(socket) + "\n" + getKrigl + "\nend";
                    toClient = "READY CHECK\nok\nwaiting\nend";
                    game.setKriglToPlayer(game.getIndexPlayer(socket), getKrigl);
                    getKrigl = -1;
                    toClinetType = "one";
                }
            }
            actualPlayer = 0;
            readyCheck = false;
        }

        if(saveGame){
            toClinetType += "save";
            saveGame = false;
        }

        if(loadGame){
            if (createGameIterator == 0) {
                // zasle klientovi seznam souboru
                if (loadState == 1) {
                    toClient = "LOAD GAME\n";
                    toClient += getFilesName();
                    toClient += "end\nend";
                    toClinetType = "one";
                }
            }else{
                toClient = "LOAD GAME\n";
                toClient += "nok\n";
                toClient += "end\n";
                toClient += "end";
                toClinetType = "one";
            }
            createGameIterator++;
            loadGame = false;
        }

        if (end){
            toClient += "EXIT";
            toClinetType += "one";
            end = false;
        }

        tmpAnswer.add(toClient);
        tmpAnswer.add(toClinetType);

        return tmpAnswer;
    }

    /**
     * Ziska seznam ulozenych her
     * @return seznam her
     */
    public String getFilesName(){
        String tmpString = "";

        File folder = new File("./examples/");
        File[] listOfFiles = folder.listFiles();

        if(listOfFiles != null) {
            for (int i = 0; i < listOfFiles.length; i++) {
                if (listOfFiles[i].isFile()) {
                    tmpString += listOfFiles[i].getName() + "\n";
                }
            }
        }else{
            tmpString = "nok\n";
        }
        return tmpString;
    }

    /**
     * Nacte data ze souboru a nastavi data pro hru
     * @param fileToLoad jmeno souboru
     */
    private void setGameFromLoad(String fileToLoad){
        String text = null;
        File file = new File("./examples/" + fileToLoad);

        try {
            FileReader reader = new FileReader(file);
            char[] chars = new char[(int) file.length()];
            reader.read(chars);
            text = new String(chars);
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        if (text != null) {
            loadArray = text.split("\n");

            numberOfPlayers = Integer.parseInt(loadArray[1]);
            sizeGame = Integer.parseInt(loadArray[2]);
            game = new Game(numberOfPlayers, sizeGame);
            game.setPlayerList(socket);

            game.createMazeBoard(false);
            game.setBoardFromLoad(loadArray[4+numberOfPlayers].split(";"));
            game.setFieldMazes();
            game.setTreasuresFromLoad(loadArray[5+numberOfPlayers].split(","));

            String[] stateInfo = loadArray[3+numberOfPlayers].split(";");
            game.setFreeCard(stateInfo[2].charAt(0), Integer.parseInt(stateInfo[3]), Integer.parseInt(stateInfo[4]));
            game.setGameState(Integer.parseInt(stateInfo[1]));
            game.setTurnNumber(Integer.parseInt(stateInfo[0]));

            game.initFieldTrX();
            game.initBooleanMap();

            setPlayerFromLoad(game.getIndexPlayer(socket));
        }
    }

    private void setPlayerFromLoad(int indexPlayer){
        String[] playerInfo = loadArray[indexPlayer+3].split(";");
        Scanner playerTreasures = new Scanner(playerInfo[6].replace(',' , ' '));

        game.setKriglToPlayer(indexPlayer, Integer.parseInt(playerInfo[1]));
        game.setXToPlayer(indexPlayer, Integer.parseInt(playerInfo[2]));
        game.setYToPlayer(indexPlayer, Integer.parseInt(playerInfo[3]));
        game.setScoreToPlayer(indexPlayer, Integer.parseInt(playerInfo[5]));
        int tmp;
        boolean tmpBool = false;
        while(playerTreasures.hasNextInt()){
            tmp = playerTreasures.nextInt();
            game.setTreasureToPlayer(indexPlayer, tmp);
            if ((tmp != Integer.parseInt(playerInfo[4])) && (!tmpBool)){
                game.setSearchTrToPlayer(indexPlayer);
            }
            else {
                tmpBool = true;
            }
        }
        game.setSearchTrToPlayer(indexPlayer, Integer.parseInt(playerInfo[4]));
    }

}
