package ija.labyrinth.treasure;

/**
 * Vytvoreni pokladu
 * @author xbayer05, xjehla00
 */
public class Treasure {
    private int code;
    private static int pocetPokladu;
    static Treasure[] arrPoklad = new Treasure[pocetPokladu];

    /**
     * Konstruktor pokladu
     * @param code Identifikacni cislo pokladu
     */
    private Treasure(int code) {
        this.code = code;
    }

    /**
     * Vytvori set pokladu
     * @param pocet Pocet pokladu
     */
    public static void createSet(int pocet){
        pocetPokladu = pocet;
        for(int i = 0 ; i < pocetPokladu; i++)
        {
            arrPoklad[i] = new Treasure(i);
        }
    }

    public int getCode(){
        return code;
    }

    /**
     * Vrati poklad s danym identifikacnim cislem
     * @param code Identifikacni cislo karty
     * @return Vraci poklad s danym code. Kdyz neexistuje vraci NULL
     */
    public static Treasure getTreasure(int code){
        for(int i = 0; i < pocetPokladu; i++){
            if(arrPoklad[i].code == code)
                return arrPoklad[i];
        }
        return null;
    }

    /**
     * Zjisti, kolik je vytvorenych pokladu pro hru
     *
     * @return Pocet vytvorenych pokladu
     */
    public static int numOfTreasures(){
        return pocetPokladu;
    }
}
