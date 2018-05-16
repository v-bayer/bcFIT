/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gatimesheduler.models;

import gatimesheduler.BaseInfoManager;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;

/**
 *
 * @author Vaclav Bayer
 */
public final class Chromosome {

    public static final int DAYS_IN_WEEK = 1;
    public static final int START_OF_THE_DAY = 9;
    public static final int END_OF_THE_DAY = 23;

    public static final boolean CONSISTENCY_CONDITION = true;
    public static final boolean CAPACITY_CONDITION = true;

    private int chromosomeId = 0;
    private int chromosomeSize;
    private List<Integer> roomsShedule;
    private HashMap<Subject, Integer> chromSubjHMap;
    private float fitnessScore = 0;

    private Boolean[] fitnessConditions = new Boolean[]{CONSISTENCY_CONDITION, CAPACITY_CONDITION};

    public Chromosome(int id) {
        chromosomeId = id;
        chromosomeSize = DAYS_IN_WEEK * (END_OF_THE_DAY - START_OF_THE_DAY) * BaseInfoManager.getNumberOfRooms();
        roomsShedule = new ArrayList<>();
        chromSubjHMap = new HashMap<Subject, Integer>();

        generateNewChromosome();
    }

    public int getChromosomeId() {
        return chromosomeId;
    }

    public List<Integer> getRoomsShedule() {
        return roomsShedule;
    }

    public HashMap<Subject, Integer> getChromSubjHMap() {
        return chromSubjHMap;
    }

    public void setChromSubjHMap(HashMap<Subject, Integer> chromSubjHMap) {
        this.chromSubjHMap = chromSubjHMap;
    }

    public float getFitnessScore() {
        return fitnessScore;
    }

    public static int getChromosomeSize() {
        return DAYS_IN_WEEK * (END_OF_THE_DAY - START_OF_THE_DAY) * BaseInfoManager.getNumberOfRooms();
    }

    public Integer getRandomSlotFromShedule() {
        return roomsShedule.get(new Random().nextInt(roomsShedule.size()));
    }

    public Integer getRandomEmptySlotFromShedule(List<Integer> occupiedSlots) {
        if (occupiedSlots.isEmpty() || occupiedSlots.size() > roomsShedule.size()) {
            return getRandomSlotFromShedule();
        }
        Integer randomSheduleSlot = null;
        do {
            randomSheduleSlot = getRandomSlotFromShedule();
        } while (occupiedSlots.contains(randomSheduleSlot) || randomSheduleSlot == null);

        return randomSheduleSlot;
    }

    public void generateNewChromosome() {
        List<Integer> occupiedSlots = new ArrayList<>();
        // initialize room shedule list
        for (int i = 0; i < chromosomeSize; i++) {
            roomsShedule.add(i);
        }
        // initialize hash map
        for (Subject subject : BaseInfoManager.getSubjectsList()) {
            for (int i = 0; i < subject.getDuration(); i++) {
                Integer sheduleEmptySlot = getRandomEmptySlotFromShedule(occupiedSlots);
                chromSubjHMap.put(subject, sheduleEmptySlot);
                occupiedSlots.add(sheduleEmptySlot);
            }

        }
        //System.out.println("occupied slots:" + occupiedSlots.size());
    }

    public void generateFitnessScore() {
        int maxSheduleScore = BaseInfoManager.getNumberOfSubjects() * getNumOfRequiredConditions();
        int sheduleScore = 0;

        for (Map.Entry<Subject, Integer> subjects : chromSubjHMap.entrySet()) {
            Subject subject = subjects.getKey();
            Integer scheduleIndex = subjects.getValue();

            if (CONSISTENCY_CONDITION) {
                int consistentCond = 1;
                /* podminka pro zachovani konzistence, 
                nemuze se stat ze se prednasi ve dvou ucebnach soucasne
                rozdilna hodina stejne prednasky */
                for (Map.Entry<Subject, Integer> subjectsToCompare : chromSubjHMap.entrySet()) {
                    // urychleni detekce nekonzistence, jestli se jiz nastla nesrovnalost -> preskoc vypocet
                    if(consistentCond == 0){
                        break;
                    }
                    Subject subject2 = subjectsToCompare.getKey();
                    Integer scheduleIndex2 = subjectsToCompare.getValue();
                    if (subject2.getId() == subject.getId() && subject2.getOffset() == subject.getOffset()) {
                        continue;
                    }
                    if (scheduleIndex == scheduleIndex2) {
//                        if (subject.getId() == subject2.getId() && subject.getOffset() != subject2.getOffset()) {
//                            consistentCond = 0;
//                        }
                        if(getSheduleRoom(scheduleIndex).getId() == getSheduleRoom(scheduleIndex2).getId()){
                            consistentCond = 0;
                        }
//                        if (getSheduleRoom(scheduleIndex).getId() == getSheduleRoom(scheduleIndex2).getId()) {
//                            int in1 = getSheduleRoom(scheduleIndex).getId();
//                            int in2 = getSheduleRoom(scheduleIndex2).getId();
//                            consistentCond = 0;
//                            break;
//                        }
                    }
                }
                sheduleScore += consistentCond;
            }

            if (CAPACITY_CONDITION) {
                int capacityCond = 1;
                int roomCapacity = getSheduleRoom(scheduleIndex).getCapacity();
                if (subject.getCapacity() > roomCapacity) {
                    capacityCond = 0;
                }
                sheduleScore += capacityCond;
            }
        }
        fitnessScore = (float) sheduleScore / maxSheduleScore;
    }

    public int getSheduleDayIndex(int sheduleIndex) {
        return (sheduleIndex / ((END_OF_THE_DAY - START_OF_THE_DAY) * BaseInfoManager.getNumberOfRooms()));
    }

    public int getSheduleRoomIndex(int sheduleIndex) {
        return ((sheduleIndex % ((END_OF_THE_DAY - START_OF_THE_DAY) * BaseInfoManager.getNumberOfRooms())) / (END_OF_THE_DAY - START_OF_THE_DAY));
    }

    public Room getSheduleRoom(int sheduleIndex) {
        int roomId = getSheduleRoomIndex(sheduleIndex);
        return BaseInfoManager.getRoomsList().get(roomId);
    }

    public int getSheduleHourIndex(int sheduleIndex) {
        return (sheduleIndex % (END_OF_THE_DAY - START_OF_THE_DAY)) + START_OF_THE_DAY;
    }

    private int getNumOfRequiredConditions() {
        int conditions = 0;
        for (Boolean fitnessCondition : fitnessConditions) {
            if (fitnessCondition) {
                conditions += 1;
            }
        }
        return conditions;
    }

}
