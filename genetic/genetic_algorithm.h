#pragma once

#include "individual.h"
#include "initializer.h"
#include "fitness_calculator.h"
#include "crossover_calculator.h"
#include "selector.h"
#include "mutator.h"
#include "best_individual_keeper.h"

namespace genetic {
    class GeneticAlgorithm {
    private:
        Initializer *initalizer;
        FitnessCalculator *fitnessCalculator;
        Selector *selector;
        CrossoverCalculator *crossoverCalculator;
        Mutator *mutator;
        BestIndividualKeeper *bestIndividualKeeper;

        IndividualArray *individuals[2];
        double *fitness;

    public:
        GeneticAlgorithm(Initializer *initalizer,
            FitnessCalculator *fitnessCalculator,
            Selector *selector,
            CrossoverCalculator *crossoverCalculator,
            Mutator *mutator,
            BestIndividualKeeper *bestIndividualKeeper);
        Individual *run(int generationSize, int generationCount);
    };
}
