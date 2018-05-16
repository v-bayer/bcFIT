package ija.labyrinth.board;

/**
 * Vypsani hry na stdout
 * @author xbayer05, xjehla00
 */
public class MazePrint {
    /**
     * Konstruktor vypisu cele hraci desky = stare
     * @param mb    = hraci deska
     */
    public MazePrint(MazeBoard mb){
        for (int i = 1; i < 8; i++){
            System.out.println("+-+-+-+-+-+-+-+");
            System.out.print('|');
            for (int j = 1; j < 8; j++){
                System.out.print(mb.get(i, j).getCard().getMazeType());
                System.out.print('|');
            }
            System.out.println("");
        }
        System.out.println("+-+-+-+-+-+-+-+");
        System.out.println("Volny kamen je:");
        System.out.println(mb.getFreeCard().getMazeType());
    }
}
