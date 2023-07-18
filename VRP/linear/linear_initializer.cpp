#include "linear_initializer.h"
#include "../VRP_individual.h"
#include <vector>

using namespace std;

VRP::LinearInitializer::LinearInitializer(int genomeSize, int numVehicles) {
    //gen = mt19937(chrono::system_clock::now().time_since_epoch().count());
    gen = mt19937(0);
    deviationGenerator = uniform_real_distribution<>(0.0, 0.9999);
    vehicleGenerator = uniform_int_distribution<>(0, numVehicles - 1);

    this->genomeSize = genomeSize;
    this->numVehicles = numVehicles;
}

void VRP::LinearInitializer::initialize(int generationSize, genetic::IndividualArray &individuals) {
    vector<genetic::Individual*> ind(generationSize);

    double *genomes = new double[generationSize * genomeSize];

    for (int i = 0; i < generationSize; i++) {
        ind[i] = new VRP::VRPIndividual(&genomes[i * genomeSize]);
        initalizeGenome(&genomes[i * genomeSize]);
    }

    individuals = genetic::IndividualArray(ind, generationSize, genomeSize, genomes);
}

void VRP::LinearInitializer::initalizeGenome(double *genome) {
    double deviation, vehicle, gene;
    for (int i = 0; i < genomeSize; i++) {
        deviation = deviationGenerator(gen);
        vehicle = vehicleGenerator(gen);
        gene = vehicle + deviation;

        genome[i] = gene;
    }
}
