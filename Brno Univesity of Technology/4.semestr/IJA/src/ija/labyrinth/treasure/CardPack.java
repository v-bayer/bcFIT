package ija.labyrinth.treasure;
import java.util.Random;

/**
 * Vytvoreni balicku karet s poklady
 * @author xbayer05, xjehla00
 */
public class CardPack {
    private int maxSize;
    private int initSize;
    static int vrcholBaliku;
    static TreasureCard[] balicek;

    /**
     * Konstruktor balicku
     * @param maxSize Maximalni velikost balicku
     * @param initSize Velikost baliku pri inicializaci
     */
    public CardPack(int maxSize, int initSize){
        this.maxSize = maxSize;
        this.initSize = initSize;
        this.vrcholBaliku = 0;
        balicek = new TreasureCard[initSize];

        if (maxSize >= initSize && (initSize >= 0) && maxSize < 24){
            for (int i = 0; i < initSize; i++){
                balicek[i] = new TreasureCard(Treasure.getTreasure(i));
            }
        }
    }

    /**
     * Zjistuje jaka je karta nahore balicku
     * @return Karta se zhora baliku
     */
    public TreasureCard popCard(){
        if (vrcholBaliku < initSize){
            return balicek[vrcholBaliku++];
        }
        return null;
    }

    /**
     * Zjistuje, kolik je karet v balicku
     * @return Pocet karet
     */
    public int size(){
        return (initSize - vrcholBaliku);
    }

    /**
     * Zamicha balicek
     */
    public void shuffle(){
        Random rand = new Random();
        int vymenitelnecislo;
        TreasureCard tmpKarta;
        int pocetkaret = initSize-vrcholBaliku;

        for (int i = vrcholBaliku; i < initSize; i++){
            vymenitelnecislo = rand.nextInt(pocetkaret);
            vymenitelnecislo += vrcholBaliku;
            if (i != vymenitelnecislo){
                tmpKarta = balicek[i];
                balicek[i] = balicek[vymenitelnecislo];
                balicek[vymenitelnecislo] = tmpKarta;
            }
        }
    }
}
