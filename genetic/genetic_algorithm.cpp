#include "genetic_algorithm.h"
#include <cstring>
#include <cstdio>

namespace genetic {
    GeneticAlgorithm::GeneticAlgorithm(Initializer *initalizer,
        FitnessCalculator *fitnessCalculator,
        Selector *selector,
        CrossoverCalculator *crossoverCalculator,
        Mutator *mutator,
        BestIndividualKeeper *bestIndividualKeeper) : initalizer(initalizer), fitnessCalculator(fitnessCalculator),
                            selector(selector), crossoverCalculator(crossoverCalculator), mutator(mutator),
                            bestIndividualKeeper(bestIndividualKeeper) {
            individuals[0] = new IndividualArray();
            individuals[1] = new IndividualArray();
        }

        Individual *GeneticAlgorithm::run(int generationSize, int generationCount) {
            fitness = new double[generationSize];
            memset(fitness, 0, generationSize * sizeof(fitness));

            initalizer->initialize(generationSize, *individuals[0]);
            initalizer->initialize(generationSize, *individuals[1]);

            vector<pair<int, int>> parents;
            for (int currGen = 0; currGen < generationCount; currGen++) {
                IndividualArray *individualsCurrentGen = individuals[currGen % 2];
                IndividualArray *individualsNextGen = individuals[(currGen + 1)% 2];

                fitnessCalculator->calculate(*individualsCurrentGen, fitness);
                parents = selector->select(*individualsCurrentGen, fitness);

                crossoverCalculator->crossover(*individualsCurrentGen, *individualsNextGen, parents);
                mutator->mutate(*individualsCurrentGen);

                if (currGen % 200 == 0) {
                    printf("\n\n Best fitness = %lf\n\n", bestIndividualKeeper->getFitness());
                }
            }

            return bestIndividualKeeper->getBest();
        }
}
