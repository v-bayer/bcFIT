/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gatimesheduler;
/**
 *
 * @author Vaclav Bayer
 */
public class GATimeSheduler {
    public static final float SUCCESS = 1.0f;
    public static final int LOOP_LIMIT = 100000;

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        BaseInfoManager baseInfoManager = new BaseInfoManager();
        baseInfoManager.loadParseCSVFiles();
        
        PopulationManager populationManager = new PopulationManager();
        populationManager.generateInitPopulation();
        
        int i = 0;
        float bestChromosomeFitness = populationManager.getBestChromosomeFitness();
        do{
            i++;
            populationManager.processPopulation();
            bestChromosomeFitness = populationManager.getBestChromosomeFitness();
            System.out.println("Chromosome biggest fitness score of the population is " + bestChromosomeFitness);
            //populationManager.printBestChromosomes(populationManager.getActualPopulation().size() / populationManager.PERCT_OF_PARENTS);
        }while(bestChromosomeFitness < SUCCESS && i < LOOP_LIMIT);
        populationManager.printBestChromosome();
    }

    
}
