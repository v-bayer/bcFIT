package ija.labyrinth.board;
/**
 * Vytvoreni kamene
 * @author xbayer05, xjehla00
 */
public class MazeCard {
    private char mazeType;
    //private TreasureCard treasure;
    private int mazeRound = 0;
    private int treasure = -1;
    private static int actualTreasureCount = 0;
    private static int maxTreasureCount = 0;

    /**
     * Konstruktor kamene
     * @param type = typ kamene
     */
    public MazeCard(char type){
        this.mazeType = type;
    }

    /**
     * vrati typ kamene
     * @return = typ
     */
    public char getMazeType(){
        return mazeType;
    }

    /**
     * vrati stupen natoceni kamene
     * @return = stupen natoceni
     */
    public int getMazeRound(){
        return mazeRound;
    }

    /*public TreasureCard getTreasure(){
        return treasure;
    }*/

    /**
     * vrati poklad na danem kameni
     * @return = poklad
     */
    public int getTreasure(){
        return treasure;
    }

    /**
     * nastavi poklad v kameni
     * @param indexTreasure = index pokladu
     */
    public void setTreasure(int indexTreasure){
        treasure = indexTreasure;
    }

    /**
     * Definice smeru kterych muze kamen nabyvat
     * a operace nad nimi
     */
    public static enum CANGO {
        LEFT,
        UP,
        RIGHT,
        DOWN;

        /**
         * Rotace kamene do puvodniho stavu
         * kvuli zjisteni je-li pruchozi
         * @param dir cesta
         * @param round stupen otoceni
         * @return cesta
         */
        public MazeCard.CANGO rotateEnum(MazeCard.CANGO dir, int round){
            if (round > 0) {
                switch (dir) {
                    case UP:
                        return rotateEnum(LEFT, --round);
                    case RIGHT:
                        return rotateEnum(UP, --round);
                    case DOWN:
                        return rotateEnum(RIGHT, --round);
                    default:
                        return rotateEnum(DOWN, --round);
                }
            }else{
                return dir;
            }
        }
    }

    /**
     * Vytvoreni noveho kamene
     * nastavi pocet pokladu
     * @param treasureCount pocet pokladu
     */
    public static void setTreasureCount(int treasureCount){
        maxTreasureCount = treasureCount;
    }

    /**
     * Vytvoreni noveho kamene
     * @param type typ kamene
     * @return vrati kamen
     */
    public static MazeCard create(char type) {
        MazeCard CLF = null;
        switch (type) {
            case 'C':
                CLF = new MazeCard('C');
                break;
            case 'L':
                CLF = new MazeCard('L');
                break;
            case 'F':
                CLF = new MazeCard('F');
                break;
            default:
                throw new IllegalArgumentException();
        }
        /*if (actualTreasureCount < maxTreasureCount){
            CLF.treasure = new TreasureCard(Treasure.getTreasure(actualTreasureCount));
            actualTreasureCount++;
        }*/
        return CLF;
    }

    /**
     * Zjisti pruchodnost kamenem
     * @param dir dotaz je li cesta dir propustna
     * @return vraci true ci false
     */
    public boolean canGo(MazeCard.CANGO dir){
        dir = dir.rotateEnum(dir, mazeRound);
        switch (mazeType){
            case 'C':
                if(dir == CANGO.RIGHT || dir == CANGO.DOWN){
                    return false;
                }else{
                    return true;
                }
            case 'L':
                if(dir == CANGO.UP || dir == CANGO.DOWN){
                    return false;
                }else{
                    return true;
                }
            case 'F':
                if(dir == CANGO.DOWN) {
                    return false;
                }else{
                    return  true;
                }
            default:
                return false;
        }
    }

    /**
     * Otoceni kamene o 90 stupnu doprava
     */
    public void turnRight(){
        mazeRound++;
        if(mazeRound == 4){
            mazeRound = 0;
        }
    }

    /**
     * Otoceni kamene o 90 stupnu doleva
     */
    public void turnLeft(){
        this.turnRight();
        this.turnRight();
        this.turnRight();
    }

    /**
     * Otoceni kamene o 90 stupnu doleva
     */
    public void turn180(){
        this.turnRight();
        this.turnRight();
    }

    /**
     * Vytvoreni kamene otoceneho vlevo vuci defaultnimu smeru
     * @param type typ
     * @return kamen
     */
    public static MazeCard createLeft(char type)
    {
        MazeCard CLF = MazeCard.create(type);
        CLF.turnLeft();
        return CLF;
    }

    /**
     * Vytvoreni kamene otoceneho vpravo vuci defaultnimu smeru
     * @param type typ
     * @return kamen
     */
    public static MazeCard createRight(char type)
    {
        MazeCard CLF = MazeCard.create(type);
        CLF.turnRight();
        return CLF;
    }

    /**
     * Vytvoreni kamene otoceneho o 180 stupnu vuci defaultnimu smeru
     * @param type typ
     * @return kamen
     */
    public static MazeCard create180(char type)
    {
        MazeCard CLF = MazeCard.create(type);
        CLF.turn180();
        return CLF;
    }

    /**
     * Vytvoreni kamen otoceny podle specifikace
     * 0 = neotaci,
     * 1 = 1x vpravo,
     * 2 = 2x vpravo,
     * 3 = 3x vpravo,
     * cokoli jine = neotaci
     * @param type typ
     * @param rotace rotace
     * @return otoceny kamen
     */
    public static MazeCard createRotate(char type, int rotace)
    {
        switch (rotace){
            case 0:
                return MazeCard.create(type);
            case 1:
                return MazeCard.createRight(type);
            case 2:
                return MazeCard.create180(type);
            case 3:
                return MazeCard.createLeft(type);
            default:
                return MazeCard.create(type);
        }
    }
}
