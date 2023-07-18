#include "best_individual_keeper.h"
#include <cstdio>
#include <float.h>

VRP::BestIndividualKeeper::BestIndividualKeeper(int genomeSize) : fitness(-DBL_MAX) {
    best = new VRP::VRPIndividual(new double[genomeSize]);
}

void copyIndividual(VRP::VRPIndividual *src, VRP::VRPIndividual *dst, int size) {
    for (int i = 0; i < size; i++) {
        dst->genome[i] = src->genome[i];
    }
}

void VRP::BestIndividualKeeper::keepBestIndividual(int pos, genetic::IndividualArray &individuals, double fitness) {
    if (fitness > this->fitness) {
        copyIndividual((VRP::VRPIndividual *) ((*individuals.getIndividuals())[pos]), best, individuals.getGenomeSize());
        this->fitness = fitness;
    }
}

genetic::Individual *VRP::BestIndividualKeeper::getBest() {
    return best;
}

double VRP::BestIndividualKeeper::getFitness() {
    return fitness;
}
