package ija.labyrinth.board;

/**
 * Vytvoreni policka do hraci desky
 * @author xbayer05, xjehla00
 */
public class MazeField {
    private int radek;
    private int sloupec;
    private MazeCard kamen = null;

    /**
     * konstruktor policka o kameni
     * @param row   = index radku kamene
     * @param col   = index sloupce kamene
     */
    public MazeField(int row, int col){
        this.radek = row;
        this.sloupec = col;
    }

    /**
     * Vraci cislo radku
     * @return cislo radku
     */
    public int row(){
        return radek;
    }

    /**
     * Vraci cislo sloupce
     * @return cislo sloupce
     */
    public int col(){
        return sloupec;
    }

    /**
     * Vraci kamen z policka
     * @return kamen
     */
    public MazeCard getCard(){
        return kamen;
    }

    /**
     * Umisti kamen do policka
     * @param c kamen
     */
    public void putCard(MazeCard c){
        this.kamen = c;
    }
}
