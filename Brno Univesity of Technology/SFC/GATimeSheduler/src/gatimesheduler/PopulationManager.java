/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gatimesheduler;

import gatimesheduler.models.SheduleItem;
import gatimesheduler.models.Subject;
import gatimesheduler.models.Room;
import gatimesheduler.models.Chromosome;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ThreadLocalRandom;

/**
 *
 * @author Vaclav Bayer
 */
public class PopulationManager {

    public static final int INIT_POPULATION_COUNT = 400;
    public static final int BEST_CHROMOSOMES_PERCT = 40;

    public static final int PERCT_OF_PARENTS = 4;
    public static final int NUMBER_OF_CROSSOVER_POINTS = 10;
    public static final int MUTATION_SIZE = 1;

    private List<Chromosome> oldPopulation;
    private List<Chromosome> actualPopulation = new ArrayList<>();

    private float oldPopFitnessScore = 0;
    private float actualPopFitnessScore = 0;

    public PopulationManager() {
    }

    public void generateInitPopulation() {
        for (int i = 0; i < INIT_POPULATION_COUNT; i++) {
            Chromosome chromosome = new Chromosome(i);
            actualPopulation.add(chromosome);
            chromosome.generateFitnessScore();
            //System.out.print("Chromosome " + chromosome.getChromosomeId() + " has fitness " + chromosome.getFitnessScore() + "; ");
        }
        actualPopFitnessScore = generatePopulationFitnessScore();
        processPopulation();
    }

    public void processPopulation() {
        oldPopulation = new ArrayList<>(actualPopulation);
        oldPopFitnessScore = actualPopFitnessScore;
        crossOver();
        mutation();
        actualPopFitnessScore = generatePopulationFitnessScore();
        System.out.println("Fitness average of the population is " + actualPopFitnessScore);
        //printBestChromosomes((int) actualPopulation.size() / BEST_CHROMOSOMES_PERCT);
    }

    public List<Chromosome> getOldPopulation() {
        return oldPopulation;
    }

    public void setOldPopulation(List<Chromosome> oldPopulation) {
        this.oldPopulation = oldPopulation;
    }

    public List<Chromosome> getActualPopulation() {
        return actualPopulation;
    }

    public void setActualPopulation(List<Chromosome> actualPopulation) {
        this.actualPopulation = actualPopulation;
    }

    public void printBestChromosomes(int count) {
        int i = 0;
        System.out.println("Printig best " + count + " chromosomes:");
        for (Chromosome bestChromosome : getBestChromosomes(count)) {
            i++;
            System.out.println(i + ". Chromosome " + bestChromosome.getChromosomeId() + " with fitness " + bestChromosome.getFitnessScore());
        }
    }

    public float getBestChromosomeFitness() {
        return getBestChromosomes(1).get(0).getFitnessScore();
    }

    public Chromosome getBestChromosome() {
        return getBestChromosomes(1).get(0);
    }

    public List<Chromosome> getBestChromosomes(int count) {
        List<Chromosome> bestChromosomes = new ArrayList<>();
        bestChromosomes = getChromosomesRandomly(count);
        for (Chromosome chromosome : actualPopulation) {
            for (final ListIterator<Chromosome> i = bestChromosomes.listIterator(); i.hasNext();) {
                final Chromosome bestChromosome = i.next();
                if (bestChromosome.getFitnessScore() < chromosome.getFitnessScore()) {
                    i.set(chromosome);
                    break;
                }
            }
        }
        return bestChromosomes;
    }

    public List<Chromosome> getParentsRandomly(int count) {
        count = Math.round(count / 2) * 2; // zaokroulit na sude cislo
        return getChromosomesRandomly(count);
    }

    public List<Chromosome> getChromosomesRandomly(int count) {
        List<Chromosome> copy = new LinkedList<Chromosome>(actualPopulation);
        Collections.shuffle(copy);
        return copy.subList(0, count);
    }

    /* Nahodny vyber chromosonu, ktere nepatri do elity */
    public List<Chromosome> getTargetsRandomlyNonElite(int count) {
        count = Math.round(count / 2) * 2; // zaokroulit na sude cislo
        List<Chromosome> bestChromosones = getBestChromosomes((int) actualPopulation.size() / BEST_CHROMOSOMES_PERCT);
        List<Chromosome> copy = new LinkedList<Chromosome>(actualPopulation);
        List<Chromosome> bestChromosonesToRemove = new ArrayList<>();
        copy.removeAll(bestChromosones);
//        for (Chromosome bestChromosone : bestChromosones) {
//            for (Chromosome copyChromosome : copy) {
//                if(copyChromosome.getChromosomeId() == bestChromosone.getChromosomeId()){
//                    int i = copy.size();
//                    copy.remove(copyChromosome);
//                    int j = copy.size();
//                    if(i == j){
//                        System.out.println("aaaaaaaaaaaaaaaaaaaaaaaa " + copyChromosome.getChromosomeId());
//                    }
//                    bestChromosonesToRemove.add(bestChromosone);
//                    System.out.println(bestChromosone.getChromosomeId());
//                    break;
//                }
//            }
//        }
//        bestChromosones.removeAll(bestChromosonesToRemove);
//        System.out.println(actualPopulation.size());
//        System.out.println(bestChromosones.size());
//        System.out.println(copy.size());
        Collections.shuffle(copy);
        return copy.subList(0, count);
    }

    public float generatePopulationFitnessScore() {
        float sumScore = 0;
        int i = 0;
        for (Chromosome chromosome : actualPopulation) {
            chromosome.generateFitnessScore();
            float chromosomeFitness = chromosome.getFitnessScore();
            sumScore += chromosomeFitness;
            i++;
        }

        //System.out.println("\nChromosome " + biggestFitnessIndex + " has biggest fitness score: " + biggestFitness);
        return (float) sumScore / i;
    }

    public List<Integer> generateCrossOverPoints() {
        List<Integer> crossOverPoints = new ArrayList<>();
        while (crossOverPoints.size() != NUMBER_OF_CROSSOVER_POINTS) {
            int randomNum = ThreadLocalRandom.current().nextInt(1, Chromosome.getChromosomeSize());
            if (crossOverPoints.contains(randomNum) || randomNum == 0) {
                continue;
            } else {
                crossOverPoints.add(randomNum);
            }
        }
        Collections.sort(crossOverPoints);
        return crossOverPoints;
    }

    /**
     * Vyber kazde druhe dvojce vygenerovanych crossover bodu, kvuli krizeni a
     * jednodussi indexaci do HashMapy
     */
    public List<Integer> translateCrossOverPointsToIndexes(List<Integer> crossOverPoints) {
        List<Integer> crossOverIndexes = new ArrayList<>();
        for (int pointEven = 0; pointEven < crossOverPoints.size(); pointEven++) {
            Integer point1 = crossOverPoints.get(pointEven);
            Integer point2 = crossOverPoints.get(++pointEven);
            for (int i = point1; i < point2; i++) {
                crossOverIndexes.add(i);

            }
        }
        return crossOverIndexes;
    }

    public void crossOver() {
        List<Chromosome> parents = getParentsRandomly(actualPopulation.size() / PERCT_OF_PARENTS);
        List<Chromosome> targets = getTargetsRandomlyNonElite(actualPopulation.size() / PERCT_OF_PARENTS);
        List<Integer> crossOverIndexes = generateCrossOverPoints();
        crossOverIndexes = translateCrossOverPointsToIndexes(crossOverIndexes);

        for (int i = 0; i < parents.size(); i += 2) {
            int i2 = i + 1;
            Chromosome parent1 = parents.get(i);
            Chromosome parent2 = parents.get(i2);
            HashMap<Subject, Integer> parent1HashMap = parent1.getChromSubjHMap();
            HashMap<Subject, Integer> parent2HashMap = parent2.getChromSubjHMap();

            Chromosome target1 = targets.get(i);
            Chromosome target2 = targets.get(i2);

            HashMap<Subject, Integer> newHashMap1 = new HashMap<>();
            HashMap<Subject, Integer> newHashMap2 = new HashMap<>();

            for (Map.Entry<Subject, Integer> entry : parent1HashMap.entrySet()) {
                Subject key = entry.getKey();
                Integer value = entry.getValue();
                if (crossOverIndexes.contains(value)) {
                    if(!newHashMap2.containsKey(key)){
                        newHashMap2.put(key, value);
                    }else{
                        newHashMap1.put(key, value);
                    }
                } else {
                    if(!newHashMap1.containsKey(key)){
                        newHashMap1.put(key, value);
                    }else{
                        newHashMap2.put(key, value);
                    }
                }
            }

            for (Map.Entry<Subject, Integer> entry : parent2HashMap.entrySet()) {
                Subject key = entry.getKey();
                Integer value = entry.getValue();
                if (crossOverIndexes.contains(value)) {
                    if(!newHashMap1.containsKey(key)){
                        newHashMap1.put(key, value);
                    }else{
                        newHashMap2.put(key, value);
                    }
                } else {
                    if(!newHashMap2.containsKey(key)){
                        newHashMap2.put(key, value);
                    }else{
                        newHashMap1.put(key, value);
                    }
                }
            }

            String parent1HashMapStr = "";
            String parent2HashMapStr = "";
            String target1HashMapStr = "";
            String target2HashMapStr = "";
            for (Object object : parent1HashMap.values()) {
                parent1HashMapStr += object.toString() + ", ";
            }
            for (Object object : parent2HashMap.values()) {
                parent2HashMapStr += object.toString() + ", ";
            }
            for (Object object : newHashMap1.values()) {
                target1HashMapStr += object.toString() + ", ";
            }
            for (Object object : newHashMap2.values()) {
                target2HashMapStr += object.toString() + ", ";
            }
//            System.out.println(parent1HashMapStr + " --- " + parent1HashMap.size());
//            System.out.println(parent2HashMapStr + " --- " + parent2HashMap.size());
//            System.out.println(target1HashMapStr + " --- " + newHashMap1.size());
//            System.out.println(target2HashMapStr + " --- " + newHashMap2.size());
            
            for (Chromosome actualChromosome : actualPopulation) {
                if (actualChromosome.getChromosomeId() == target1.getChromosomeId()) {
                    //target1.setChromSubjHMap(newHashMap1);
                    actualChromosome.setChromSubjHMap(newHashMap1);
                } else if (actualChromosome.getChromosomeId() == target2.getChromosomeId()) {
                    //target2.setChromSubjHMap(newHashMap2);
                    actualChromosome.setChromSubjHMap(newHashMap2);
                }
            }
//            target1.setChromSubjHMap(newHashMap1);
//            target2.setChromSubjHMap(newHashMap2);
        }
    }

    public void mutation() {
        List<Chromosome> randomChromosomes = getChromosomesRandomly(MUTATION_SIZE);
        for (Chromosome randomChromosome : randomChromosomes) {
            randomChromosome.generateFitnessScore();
            float fit = randomChromosome.getFitnessScore();
            int randomNum = ThreadLocalRandom.current().nextInt(0, randomChromosome.getRoomsShedule().size());
            Subject subject = new Subject();
            Integer result = null;
            do {
                subject = BaseInfoManager.getRandomSubject();
                result = randomChromosome.getChromSubjHMap().remove(subject);
            } while (result == null);
            randomChromosome.getChromSubjHMap().put(subject, randomNum);

            randomChromosome.generateFitnessScore();
            float fit2 = randomChromosome.getFitnessScore();
            if (fit != fit2) {
                System.out.println(fit + "->" + fit2);
            }
            System.out.println("");
        }
    }

    public void printBestChromosome() {
        Chromosome bestChromosome = getBestChromosome();
        printChromosome(bestChromosome);
    }

    public void printChromosome(Chromosome bestChromosome) {
        List<SheduleItem> sheduleItems = new ArrayList<>();
        int i = 0;
        for (Map.Entry<Subject, Integer> entry : bestChromosome.getChromSubjHMap().entrySet()) {
            Subject subject = entry.getKey();
            Integer value = entry.getValue();
            int hour = bestChromosome.getSheduleHourIndex(value);
            int dayInt = bestChromosome.getSheduleDayIndex(value);
            Room room = bestChromosome.getSheduleRoom(value);
            String roomName = room.getShortcut();

            SheduleItem sheduleItem = new SheduleItem(i);
            sheduleItem.setDayInt(dayInt);
            sheduleItem.setHour(hour);
            sheduleItem.setRoomId(room.getId());
            sheduleItem.setRoomShortcut(roomName);
            sheduleItem.setSubjectId(subject.getId());
            sheduleItem.setSubjectOffset(subject.getOffset());
            sheduleItem.setSubjectShortcut(subject.getShortcut());
            sheduleItems.add(sheduleItem);

            i++;
        }

        //sheduleItems.sort(Comparator.comparingInt(SheduleItem::dayInt));
        Collections.sort(sheduleItems, new Comparator<SheduleItem>() {
            @Override
            public int compare(SheduleItem p1, SheduleItem p2) {
                return p1.hour - p2.hour; // Ascending
            }

        });

        for (SheduleItem sheduleItem : sheduleItems) {
            System.out.println(sheduleItem.getSubjectShortcut() + "\t" + sheduleItem.getDay() + "\t" + sheduleItem.getHour() + "\t" + sheduleItem.getRoomShortcut());
        }
    }
}
