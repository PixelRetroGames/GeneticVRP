#include "../../genetic/genetic_algorithm.h"

#include "../../VRP/linear/linear_crossover_calculator.h"
#include "../../VRP/linear/linear_initializer.h"
#include "../../VRP/linear/linear_mutator.h"
#include "../../VRP/linear/linear_selector.h"
#include "../../VRP/VRP_individual.h"
#include "../../VRP/best_individual_keeper.h"

#include "../../VRP/input_parser.h"
#include "../../VRP/solution_writer.h"

#include "../../VRP/mpi/mpi_fitness_worker.h"
#include "../../VRP/mpi/mpi_fitness_calculator.h"

#include "mpi_data3.h"

#include <cstdio>
#include <mpi/mpi.h>

using namespace VRP;

const double MUTATION_INDIVIDUAL = 0.01;
const double MUTATION_GENE = 0.001;

const double MUTATION_GLOBAL_RATE = 0;
const double MUTATION_GLOBAL_LIKENESS = 0.03;

const double CROSSOVER_ELITE = 0.1;
const double CROSSOVER_NEW_RANDOM = 0.01;

const double PARENTS_PERCENTAGE = 0.84;
const double TOURNAMENT_SIZE_PERCENTAGE = 0.2;
const double ELITISM_PERCENTAGE = 0.1;

int static startWorker(Problem* problem, int numVehicles, int genomeLength, int maxSize) {
    printf("worker start\n");
    fflush(stdout);
    MpiFitnessWorker worker(problem, numVehicles, genomeLength, maxSize);

    while (true) {
        worker.calculateInterval();
    }

    return 0;
}

int static masterRun(Problem* problem, int numVehicles, int genomeLength, int genSize, int genCnt) {
    int numWorkers;
    MPI_Comm_size(MPI_COMM_WORLD, &numWorkers);
    numWorkers--;

    BestIndividualKeeper keeper(genomeLength);
    LinearInitializer initializer(genomeLength, numVehicles);
    MpiFitnessCalculator fitnessCalculator(problem, numVehicles, &keeper, numWorkers);
    LinearSelector selector(genSize, PARENTS_PERCENTAGE, TOURNAMENT_SIZE_PERCENTAGE, ELITISM_PERCENTAGE);
    LinearCrossoverCalculator crossoverCalculator(genSize, genomeLength, CROSSOVER_ELITE, CROSSOVER_NEW_RANDOM, &initializer);
    LinearMutator mutator(genomeLength, MUTATION_INDIVIDUAL, MUTATION_GENE, 0.5 * (numVehicles - 1.0), MUTATION_GLOBAL_RATE, MUTATION_GLOBAL_LIKENESS);

    genetic::GeneticAlgorithm algo(&initializer, &fitnessCalculator, &selector, &crossoverCalculator, &mutator, &keeper);
    genetic::Individual* best = algo.run(genSize, genCnt);
    VRP::SolutionWriter solWriter;
    solWriter.Write((VRPIndividual*)best, numVehicles, genomeLength, problem, "dataset/unicamp/results/res-E-n51-k5.txt");
    return 0;
}

int mpiRunData3(int GENERATION_SIZE, int GENERATION_COUNT) {
    MPI_Init(NULL, NULL);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    InputParser inputParser;
    Problem* problem;
    problem = inputParser.Parse("dataset/unicamp/problems/E-n51-k5.txt");

    int numVehicles = 5;
    int genomeLength = problem->dimension - 1;
    int numWorkers;
    MPI_Comm_size(MPI_COMM_WORLD, &numWorkers);
    numWorkers--;

    int ret = 0;
    if (rank == 0) {
        ret = masterRun(problem, numVehicles, genomeLength, GENERATION_SIZE, GENERATION_COUNT);
    }
    else {
        ret = startWorker(problem, numVehicles, genomeLength, GENERATION_SIZE / numWorkers + GENERATION_SIZE % numWorkers);
    }

    MPI_Finalize();
    return ret;
}
