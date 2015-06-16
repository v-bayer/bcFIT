package ija.labyrinth.treasure;

/**
 * Vytvoreni karty pokladu
 * @author xbayer05, xjehla00
 */
public class TreasureCard {
    public Treasure poklad;

    /**
     * Konstruktor karty pokladu
     * @param tr Oznacuje poklad zobrazeny na karte
     */
    public TreasureCard(Treasure tr)
    {
        this.poklad = tr;
    }

    /**
     * Zjistuje, zdali je poklad na karte shodny s pokladem na karte tk
     * @param tk Porovnavajici se karta
     * @return Boolean, zdali jsou shodne
     */
    public boolean equals(Object tk) {
        if (tk instanceof TreasureCard) {
            TreasureCard tk2 = (TreasureCard)tk;
            return (poklad.equals(tk2.poklad));
        } else
            return false;
    }

    /**
     * Prepis funkce hashCode
     * @return Hash kod daneho pokladu
     */
    public int hashCode() {
        return poklad.hashCode();
    }
}
