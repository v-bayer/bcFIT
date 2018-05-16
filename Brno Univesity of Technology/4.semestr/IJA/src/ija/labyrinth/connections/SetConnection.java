package ija.labyrinth.connections;

import ija.labyrinth.panels.GamePanel;
import ija.labyrinth.panels.JoinSelectKryglPanel;
import ija.labyrinth.play;
import ija.labyrinth.textfields.JoinGameTextField;

import javax.swing.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

/**
 * trida obsluhujici spojeni se serverem a posilani zprav
 * Created by xjehla00, xbayer05 on 8. 5. 2015.
 */
public class SetConnection/* extends SwingWorker<Integer, String> */ {
    //String line = "CREATE GAME\n2\n11\n3\nend";
    //String line = "JOIN GAME\nend";
    //String line = "READY CHECK\n2\n4\nend";
    //String line = "TURN\nend";
    private static String line = "";
    private static BufferedReader in;
    private static PrintWriter out;
    private static Socket client = null;
    private static boolean condition = true;
    private static Protocol protocol = new Protocol();

    /**
     * Prida data k odeslani
     *
     * @param data = Data
     */
    public static void addLine(String data) {
        line += data + "\n";
    }

    public static String getLine() {
        try {
            return in.readLine();
        } catch (IOException e) {
            //System.out.println("No I/O");
            JOptionPane.showMessageDialog(play.getMenuFrame(), "No server listening.", "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
        return "";
    }

    /**
     * poslouchani dat ze serveru
     */
    public static void listen() {
        int i = 0;

        condition = true;

        try {
            while (condition) {
                line = in.readLine();
                i++;

                //System.out.println(i + " PRIJMAM: " + line);

                if (i == 1) {
                    protocol.ParseLine(line);
                } else {
                    i = protocol.FinalM(line, i);
                }

                if (line.equals("end")) {
                    i = 0;
                    condition = false;
                }
            }
        } catch (IOException e) {
            //System.out.println("No I/O");
            JOptionPane.showMessageDialog(play.getMenuFrame(), "No server listening.", "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
    }

    /**
     * Vytvoreni hry
     */
    public static void createGame() {
        line += "end";
        out.println(line);
        //System.out.print("ODESILAM: " + line + "\n");

        protocol.initFlags();

        listen();

        if (protocol.getEnd()) {
            closeConnection();
            play.setMainMenu();
            JOptionPane.showMessageDialog(play.getMenuFrame(), "Hra již byla vytvořena. Prosím připoj se v Join.", "Error", JOptionPane.ERROR_MESSAGE);
        } else {
            play.setWaitingPanel();

            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    listen();
                }
            });
        }
    }

    /**
     * pripojeni se ke hre - cast 1
     */
    public static void joinGame() {
        out.println("JOIN GAME\nend");
        //System.out.print("ODESILAM: " + "JOIN GAME\nend\n");

        protocol.initFlags();

        listen();

        if (protocol.getEnd()) {
            closeConnection();
            play.setMainMenu();
            JOptionPane.showMessageDialog(play.getMenuFrame(), "Pardon, ale máme hodně pivařů.", "Error", JOptionPane.ERROR_MESSAGE);
        } else {
            if (!protocol.getNoReadyCheck()){
                play.setJoinSelKrygl();
            }
            else {
                play.setWaitingPanel();
                SwingUtilities.invokeLater(new Runnable() {
                    @Override
                    public void run() {
                        listen();
                    }
                });
            }
        }
    }

    /**
     * pripojeni se ke hre - cast 2
     */
    public static void joinGameSelectKrygl() {
        out.println("READY CHECK\n" + protocol.getIndexPlayer() + "\n" + JoinSelectKryglPanel.getSelectedKrygl() + "\nend");
        //System.out.print("READY CHECK\n" + protocol.getIndexPlayer() + "\n" + JoinSelectKryglPanel.getSelectedKrygl() + "\nend\n");

        listen();
        //listen();

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                listen();
            }
        });
    }

    /**
     * Posle serveru udaje o tahu.
     *
     * @param state     = 1 - vklad kamene, 2 - tah hrace
     * @param positionX = X-ova pozice kam se kliklo
     * @param positionY = Y-ova pozice kam se kliklo
     */
    public static void turn(int state, int positionX, int positionY) {
        out.println("TURN\n" + state + "\n" + positionX + "\n" + positionY + "\n" + GamePanel.getScore(GamePanel.getNumberOfPlayer()) + "\n" + GamePanel.getRotationFreeMaze() + "\nend");
        //System.out.print("TURN\n" + state + "\n" + positionX + "\n" + positionY + "\n" + GamePanel.getScore(GamePanel.getNumberOfPlayer()) + "\n" + GamePanel.getRotationFreeMaze() + "\nend");

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                listen();
            }
        });
    }

    /**
     * Ulozi hru
     */
    public static void saveGame() {
        out.println("SAVE GAME\nend");
        //System.out.print("ODESILAM: SAVE GAME\nend\n");
    }

    /**
     * Nacte hru - zepta se na mozne hry
     *
     * @param stav = ptani se na jmena her
     * @param file = jmeno hry - nepotrebne
     */
    public static void loadGame(int stav, String file) {
        out.println("LOAD GAME\n" + stav + "\n" + file + "\nend");
        //System.out.print("ODESILAM: LOAD GAME\n" + stav + "\n" + file + "\nend");

        listen();

        if (protocol.getEnd()) {
            closeConnection();
            play.setMainMenu();
            line = "";
            JOptionPane.showMessageDialog(play.getMenuFrame(), "Omlouváme se, ale hra již byla vytvořena.", "Error", JOptionPane.ERROR_MESSAGE);
        } else {
            play.setLoadGameConfirmPanel();
        }
    }

    /**
     * Nacte hru - ceka na ostatni hrace
     *
     * @param stav = vybrani hry
     * @param file = vybrana hra
     */
    public static void loadGame2(int stav, String file) {
        out.println("LOAD GAME\n" + stav + "\n" + file + "\nend");
        //System.out.print("ODESILAM: LOAD GAME\n" + stav + "\n" + file + "\nend\n");

        play.setWaitingPanel();

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                listen();
                listen();
            }
        });
    }

    /**
     * ukonci hru
     */
    public static void exitGame() {
        out.println("EXIT GAME\nend");
        //System.out.print("ODESILAM: EXIT GAME\nend\n");

        closeConnection();
    }

    /**
     * Ukonceni komunikace
     */
    public static void closeConnection() {
        //System.out.print("Close Connection: ");
        try {
            in.close();
            out.close();
            client.close();
            //System.out.println("OK");
        } catch (IOException e) {
            //System.out.println("close failed: " + e);
            JOptionPane.showMessageDialog(play.getMenuFrame(), "Close failed: " + e, "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
    }

    /**
     * Zalozi komunikaci
     */
    public static void listenSocket() {
        try {
            client = new Socket(JoinGameTextField.getIpAddress(), 2222);
            in = new BufferedReader(new InputStreamReader(client.getInputStream()));
            out = new PrintWriter(client.getOutputStream(), true);
        } catch (UnknownHostException e) {
            //System.out.println("Join failed: " + JoinGameTextField.getIpAddress());
            JOptionPane.showMessageDialog(play.getMenuFrame(), "Join failed: " + JoinGameTextField.getIpAddress(), "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        } catch (IOException e) {
            //System.out.println("No I/O");
            JOptionPane.showMessageDialog(play.getMenuFrame(), "No server listening.", "Error", JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
    }
}
