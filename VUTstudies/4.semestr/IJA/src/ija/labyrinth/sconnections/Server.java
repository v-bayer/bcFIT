package ija.labyrinth.sconnections;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

/**
 * Projekt: IJA - Labyrinth
 * Trida server umoznuje multivlaknovou komunikaci
 * mezi klientama a serverem
 * @author xbayer05, xjehla00
 */
public class Server {
    private static ServerSocket server;
    private static int port = 2222;
    private static List<AllConnections> connections = new ArrayList<AllConnections>();
    private static int indexCon = 0;

    /**
     * odstrani pripojeni
     * @param index = index daneho pripojeni
     */
    public static void removeCon (int index){
        connections.remove(index);
        indexCon--;
    }

    /**
     * stara se o pripojeni
     */
    public Server() {
        try {
            server = new ServerSocket(port);
            System.out.println("Waiting...");
            while (true) {
                Socket socket = server.accept();
                //System.out.println("Connected: " + socket);
                AllConnections service = new AllConnections(socket);
                connections.add(service);
                indexCon++;
                service.start();
            }
        } catch (IOException e) {
            System.out.println("Connection failed: " + e);
            System.exit(1);
        }

    }

    /**
     * trida pripojeni
     */
    class AllConnections extends Thread {

        private Socket socket;
        private BufferedReader in;
        private PrintWriter out;

        /**
         * pripojeni noveho hrace
         * @param socket = socket hrace
         */
        public AllConnections(Socket socket) {
            this.socket = socket;
            try {
                in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                out = new PrintWriter(socket.getOutputStream(), true);
            } catch (IOException e) {
                System.out.println("in/out failed: " + e);
                System.exit(1);
            }
        }

        /**
         * komunikace s klientem
         */
        @Override
        public void run() {
            int i = 0;
            String line;
            Protocol protocol = new Protocol(socket, indexCon);
            List<String> answer;

            while (true) {
                try {
                    line = in.readLine();
                    i++;
                    //System.out.println(i + " PRIJMAM: " + line);

                    if (i == 1) {
                        protocol.ParseLine(line);
                    }else {
                        protocol.FinalM(line, i);
                    }

                    if(line.equals("end")){
                        answer = protocol.genAnswer();
                        //System.out.print("na indexu 0: " + answer.get(0) + "\n");
                        //System.out.print("na indexu 1: " + answer.get(1) + "\n");
                        if (answer.get(1).equals("one")){
                            sendMessage(answer.get(0));
                        }else if (answer.get(1).equals("all")){
                            sendToAnyone(answer.get(0));
                        }else if(answer.get(1).equals("uniq")){
                            sendMessage(answer.get(0));
                            answer = protocol.genAnswer();
                            //System.out.print("na indexu 0: " + answer.get(0) + "\n");
                            //System.out.print("na indexu 1: " + answer.get(1) + "\n");
                            sendToAnyone(answer.get(0));
                        }else if(answer.get(1).equals("maxC")){
                            sendMessage(answer.get(0));
                            break;
                        }
                        i = 0;
                    }
                    if(line.equals("EXIT")){
                        break;
                    }
                } catch (IOException e) {
                    //System.out.println("Read failed: " + e);
                    sendToAnyone("EXIT\n" + Protocol.getActulPlayer() + "\nend\n");
                    System.exit(1);
                } catch (NullPointerException e) {
                    System.out.println("Null pointer: " + e);
                    System.exit(1);
                }
            }
        }

        /**
         * odesila hraci zpravu
         * @param message   = zprava
         */
        protected void sendMessage(String message) {
            out.println(message);
        }

        /**
         * odesila zpravu vsem hracum
         * @param message = zprava
         */
        private void sendToAnyone(String message) {
            for (AllConnections connection : connections) {
                //System.out.println(connection.socket);
                connection.sendMessage(message);
            }
        }

    }

    /**
     * main funkce
     * @param args = vstupní argumenty
     */
    public static void main(String[] args) {
        new Server();
    }
}
