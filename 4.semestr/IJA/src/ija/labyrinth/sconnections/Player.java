package ija.labyrinth.sconnections;

import java.net.Socket;
import java.util.ArrayList;

/**
 * Projekt: IJA - Labyrinth
 * Trida uchovavajici data o hraci
 * @author xbayer05, xjehla00
 */
public class Player {
    private Socket socket;
    private int indexPlayer;

    private int krigl = -1;
    private boolean newTreasure = false;
    private int positionX = 0;
    private int positionY = 0;
    private int score = 100;
    private int searchTreasure = 0;
    private int indexSearchTreasure = 0;
    private ArrayList<Integer> treasures = new ArrayList<Integer>();

    public Player(int indexPlayer, Socket socket){
        this.socket = socket;
        this.indexPlayer = indexPlayer;
    }

    public String getSearchedTreasures(){
        String tmpString = "";
        int i = 0;

        while (treasures.get(i) != indexSearchTreasure){
            tmpString += treasures.get(i) + ";";
            i++;
        }

        return tmpString;
    }

    public String getTrString(){
        String tmpString = "";

        for(int i = 0; i < treasures.size(); i++){
            tmpString += treasures.get(i) + ",";
        }
        return tmpString;
    }

    public String getTrStringNewLoad(){
        String tmpString = "";

        for(int i = 0; i < treasures.size(); i++){
            tmpString += treasures.get(i) + ";";
        }
        return tmpString;
    }

    public Socket getSocket(){
        return socket;
    }

    public int getIndexPlayer(){
        return indexPlayer;
    }

    public void setKrigl(int kriglIn){
        krigl = kriglIn;
    }

    public int getKrigl(){
        return krigl;
    }

    public void setNewTreasure(boolean newIn){
        newTreasure = newIn;
    }

    public boolean getNewTreasure(){
        return newTreasure;
    }

    public void setPositionX(int positionXIn){
        positionX = positionXIn;
    }

    public void setPositionY(int positionYIn){
        positionY = positionYIn;
    }

    public int getPositionX(){
        return positionX;
    }

    public int getPositionY(){
        return positionY;
    }

    public void setScore(int scoreIn){
        score = scoreIn;
    }

    public int getScore(){
        return score;
    }

    public void setSearchTreasure(){
        searchTreasure++;
    }

    public void setSearchTreasure(int indexTreasure){
        indexSearchTreasure = indexTreasure;
    }

    public int getSearchTreasure(){
        return treasures.get(searchTreasure);
    }

    public void setTreasures(int treasuresIn){
        treasures.add(treasuresIn);
    }

    public int getTreasures(){
        int lenght = treasures.size();
        if(!newTreasure) {
            return -1;
        }else{
            newTreasure = false;
            return treasures.get(lenght);
        }
    }
}
