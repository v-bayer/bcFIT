/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gatimesheduler;

import gatimesheduler.models.Subject;
import gatimesheduler.models.Room;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Random;

/**
 *
 * @author Vaclav Bayer
 */
public class BaseInfoManager {
    
    public static final List<String> Days = Arrays.asList("PO", "UT", "ST", "CT", "PA");
    
    public static final int PARAM_NAME = 0;
    public static final int PARAM_CAPACITY = 1;
    public static final int PARAM_DURATION = 2;

    public static final String CSV_ROOMS = "rooms.csv";
    public static final String CSV_SUBJECTS = "subjects.csv";

    private static List<Room> roomsList = new ArrayList<Room>();
    private static List<Subject> subjectsList = new ArrayList<Subject>();

    public BaseInfoManager() {
    }

    public static List<Room> getRoomsList() {
        return roomsList;
    }

    public static List<Subject> getSubjectsList() {
        return subjectsList;
    }

    public void loadParseCSVFiles() {
        loadParseCSVFile(CSV_ROOMS);
        loadParseCSVFile(CSV_SUBJECTS);
    }

    public void loadParseCSVFile(String csvFile) {
        BufferedReader br = null;
        String line = "";
        String cvsSplitBy = ";";
        int lineIndex = 0;

        try {

            br = new BufferedReader(new InputStreamReader(
                    this.getClass().getResourceAsStream("/resources/" + csvFile)));
            while ((line = br.readLine()) != null) {
                String[] params = line.split(cvsSplitBy);
                if (csvFile.equalsIgnoreCase(CSV_ROOMS)) {
                    parseRoomsInputParams(params, lineIndex);
                } else if (csvFile.equalsIgnoreCase(CSV_SUBJECTS)) {
                    parseSubjectsInputParams(params, lineIndex);
                }
                lineIndex++;
            }

        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            if (br != null) {
                try {
                    br.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    public void parseRoomsInputParams(String[] params, int lineIndex) {
        int i = 0;
        Room room = new Room(lineIndex);
        for (String param : params) {
            if (i == PARAM_NAME) {
                room.setShortcut(param);
                //System.out.println(param);
            } else if (i == PARAM_CAPACITY) {
                room.setCapacity(Integer.parseInt(param));
            }
            i++;
        }
        roomsList.add(room);
    }

    public void parseSubjectsInputParams(String[] params, int lineIndex) {
        int subjectDuration = 1;
        if (params[PARAM_DURATION] != null) {
            subjectDuration = Integer.parseInt(params[PARAM_DURATION]);
        }
        for (int i = 0; i < subjectDuration; i++) {
            Subject subject = new Subject(lineIndex);
            for (int j = 0; j < params.length; j++) {
                switch (j) {
                    case PARAM_NAME:
                        String shortcut = params[PARAM_NAME].replaceAll(" ","");
                        subject.setShortcut(shortcut);
                        //System.out.println(params[PARAM_NAME] + " offset " + i);
                        break;
                    case PARAM_CAPACITY:
                        subject.setCapacity(Integer.parseInt(params[PARAM_CAPACITY]));
                        break;
                    case PARAM_DURATION:
                        subject.setDuration(1);
                        break;
                    default:
                        break;
                }
                subject.setOffset(i);
            }
            subjectsList.add(subject);
        }
    }

    public static int getNumberOfRooms() {
        return roomsList.size();
    }

    public static int getNumberOfSubjects() {
        return subjectsList.size();
    }

    public static Subject getRandomSubject() {
        return subjectsList.get(new Random().nextInt(subjectsList.size()));
    }

    public static int getTotalSubjectsDuration() {
        int totalDuration = 0;
        for (Subject subject : subjectsList) {
            totalDuration += subject.getDuration();
        }
        return totalDuration;
    }
}
