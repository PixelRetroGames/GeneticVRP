#include "../../genetic/genetic_algorithm.h"

#include "../../VRP/openmp/openmp_fitness_calculator.h"

#include "../../VRP/linear/linear_crossover_calculator.h"
#include "../../VRP/linear/linear_initializer.h"
#include "../../VRP/linear/linear_mutator.h"
#include "../../VRP/linear/linear_selector.h"
#include "../../VRP/VRP_individual.h"
#include "../../VRP/best_individual_keeper.h"

#include "../../VRP/input_parser.h"
#include "../../VRP/solution_writer.h"

#include "openmp_data1.h"

#include <cstdio>
#include <omp.h>

using namespace VRP;

int openmpRunData1(int GENERATION_SIZE, int GENERATION_COUNT, int NUM_THREADS) {
    const double MUTATION_INDIVIDUAL = 0.01;
    const double MUTATION_GENE = 0.001;

    const double MUTATION_GLOBAL_RATE = 0.4;
    const double MUTATION_GLOBAL_LIKENESS = 0.03;

    const double CROSSOVER_ELITE = 0.1;
    const double CROSSOVER_NEW_RANDOM = 0.01;

    const double PARENTS_PERCENTAGE = 0.84;
    const double TOURNAMENT_SIZE_PERCENTAGE = 0.2;
    const double ELITISM_PERCENTAGE = 0.1;

    omp_set_num_threads(NUM_THREADS);

    InputParser inputParser;
    Problem *problem;
    problem = inputParser.Parse("dataset/unicamp/problems/E-n22-k4.txt");

    printf("Entry point\n");

    int numVehicles = 4;
    int genomeLength = problem->dimension - 1;

    BestIndividualKeeper keeper(genomeLength);
    LinearInitializer initializer(genomeLength, numVehicles);
    OpenmpFitnessCalculator fitnessCalculator(problem, numVehicles, &keeper);
    LinearSelector selector(GENERATION_SIZE, PARENTS_PERCENTAGE, TOURNAMENT_SIZE_PERCENTAGE, ELITISM_PERCENTAGE);
    LinearCrossoverCalculator crossoverCalculator(GENERATION_SIZE, genomeLength, CROSSOVER_ELITE, CROSSOVER_NEW_RANDOM, &initializer);
    LinearMutator mutator(genomeLength, MUTATION_INDIVIDUAL, MUTATION_GENE, 0.5 * (numVehicles - 1.0), MUTATION_GLOBAL_RATE, MUTATION_GLOBAL_LIKENESS);

    genetic::GeneticAlgorithm algo(&initializer, &fitnessCalculator, &selector, &crossoverCalculator, &mutator, &keeper);
    genetic::Individual *best = algo.run(GENERATION_SIZE, GENERATION_COUNT);
    VRP::SolutionWriter solWriter;
    solWriter.Write((VRPIndividual*) best, numVehicles, genomeLength, problem, "dataset/unicamp/results/res-E-n22-k4.txt");
    return 0;
}

