#include "linear_mutator.h"

VRP::LinearMutator::LinearMutator(int genomeSize, double mutationIndividual, double mutationGene,
                                  double maxMutation, double globalMutationRate, double globalMutationLikeness) {
    this->genomeSize = genomeSize;
    this->mutationIndividual = mutationIndividual;
    this->mutationGene = mutationGene;
    this->maxMutation = maxMutation;
    this->globalMutationRate = globalMutationRate;
    this->globalMutationLikeness = globalMutationLikeness;
    //gen = mt19937(chrono::system_clock::now().time_since_epoch().count());
    gen = mt19937(0);
    mutationChanceGenerator = uniform_real_distribution<>(0.0, 1.0);
    mutationGenerator = uniform_real_distribution<>(0.0, maxMutation);
    mutationGeneGenerator = uniform_real_distribution<>(0.0, 1.0 * genomeSize);
}

void VRP::LinearMutator::mutate(genetic::IndividualArray &individuals) {
    int gene = mutationGeneGenerator(gen);
    double mutationChance;
    /*std::vector<double> avgGeneValue(genomeSize, 0.0);
    for (int i = 0; i < individuals.size(); i++) {
        double *genome = ((VRPIndividual*) ((*individuals.getIndividuals())[i]))->genome;
        for (int j = 0; j < genomeSize; j++) {
            avgGeneValue[j] += genome[j] / individuals.size();
        }
    }
    std::vector<double> geneLikeness(genomeSize, 0.0);
    for (int i = 0; i < individuals.size(); i++) {
        double *genome = ((VRPIndividual*) ((*individuals.getIndividuals())[i]))->genome;
        for (int j = 0; j < genomeSize; j++) {
            geneLikeness[j] += std::abs(avgGeneValue[j] - genome[j]) / ((avgGeneValue[j] + genome[j]) / 2);
        }
    }

    double minVal = geneLikeness[0] / individuals.size();
    int minPos = 0;
    for (int j = 0; j < genomeSize; j++) {
        if (geneLikeness[j] < minVal) {
            minVal = geneLikeness[j] / individuals.size();
            minPos = j;
        }
    }*/

    static int generation = 0, sq = 0;
    generation++;
    /*if (generation % 200 == 0) {
        printf("Treshold = %lf\n", globalMutationLikeness);
        printf("Likeness %lf\n", minVal);
    }*/
    double minVal = 1;
    if (minVal < globalMutationLikeness || generation % 4000 == 0) {
        globalMutationLikeness *= 0.999;
        //globalMutationLikeness = max(0.03, globalMutationLikeness);
        sq++;
        //gene = minPos;
        if (sq % 1 == 0) {
            printf("\n\nMutate Global gene\n\n");
            //printf("Treshold = %lf\n", (globalMutationLikeness));
            //printf("Likeness %lf\n", minVal);
        }
    } else {
        gene = -1;
    }

    for (int i = 0; i < individuals.size(); i++) {
        mutate((VRPIndividual*) ((*individuals.getIndividuals())[i]), gene);
    }
}

void VRP::LinearMutator::mutate(VRP::VRPIndividual *individual, int globalGene) {
    double mutationChance, mutation;
    mutationChance = mutationChanceGenerator(gen);
    if (mutationChance < mutationIndividual) {
        for (int i = 0; i < genomeSize; i++) {
            mutationChance = mutationChanceGenerator(gen);
            if (mutationChance < mutationGene && globalGene != i) {
                mutation = mutationGenerator(gen);
                mutateGene(individual->genome[i], mutation);
            }
        }
    }

    mutationChance = mutationChanceGenerator(gen);
    if (globalGene != -1 && mutationChance < globalMutationRate) {
        mutation = mutationGenerator(gen);
        mutateGene(individual->genome[globalGene], mutation);
    }
}

void VRP::LinearMutator::mutateGene(double &genome, double mutation) {
    genome += mutation;
    if (genome > maxMutation) {
        genome = mutation;
    }
}
