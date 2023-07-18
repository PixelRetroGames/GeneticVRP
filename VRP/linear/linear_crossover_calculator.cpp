#include "linear_crossover_calculator.h"
#include <algorithm>

VRP::LinearCrossoverCalculator::LinearCrossoverCalculator(int generationSize, int genomeSize, double elitismPercentage,
                                                          double newRandomIndividualsPercentage,
                                                          VRP::LinearInitializer *linearInitializer) {
    this->genomeSize = genomeSize;
    this->elitismPercentage = elitismPercentage;
    this->newRandomIndividualsPercentage = newRandomIndividualsPercentage;
    this->linearInitializer = linearInitializer;
    //gen = mt19937(chrono::system_clock::now().time_since_epoch().count());
    gen = mt19937(0);
    pointGenerator = uniform_int_distribution<>(0, 1);

    int numAvailableParents = generationSize - newRandomIndividualsPercentage * generationSize;
    availableParents = vector<int>(numAvailableParents);
    for (int i = 0; i < numAvailableParents; i++) {
        availableParents[i] = i;
    }
}

void VRP::LinearCrossoverCalculator::crossover(genetic::IndividualArray &individuals, genetic::IndividualArray &individualsChildren, vector<pair<int, int>> &parents) {
    VRP::VRPIndividual *parentA, *parentB;
    VRP::VRPIndividual *childA, *childB;
    int childrenPos = 0;
    for (auto parentsPair : parents) {
        if (parentsPair.second == -1) {
            parentA = (VRPIndividual*) ((*individuals.getIndividuals())[parentsPair.first]);
            childA = (VRPIndividual*) ((*individualsChildren.getIndividuals())[childrenPos++]);

            copyInNextGen(parentA, childA);
        } else {
            parentA = (VRPIndividual*) ((*individuals.getIndividuals())[parentsPair.first]);
            parentB = (VRPIndividual*) ((*individuals.getIndividuals())[parentsPair.second]);

            childA = (VRPIndividual*) ((*individualsChildren.getIndividuals())[childrenPos++]);
            childB = (VRPIndividual*) ((*individualsChildren.getIndividuals())[childrenPos++]);

            crossover(parentA, parentB, childA, childB);
        }
    }

    for (;childrenPos < individualsChildren.size(); childrenPos++) {
        linearInitializer->initalizeGenome(((VRPIndividual*) ((*individualsChildren.getIndividuals())[childrenPos]))->genome);
    }
}

void VRP::LinearCrossoverCalculator::copyInNextGen(VRP::VRPIndividual *parentA, VRP::VRPIndividual *childA) {
    for (int i = 0; i < genomeSize; i++) {
        childA->genome[i] = parentA->genome[i];
    }
}

void VRP::LinearCrossoverCalculator::crossover(VRP::VRPIndividual *parentA, VRP::VRPIndividual *parentB,
                                               VRP::VRPIndividual *childA, VRP::VRPIndividual *childB) {
    int point;
    // First part of genome
    for (int i = 0; i < genomeSize; i++) {
        point = pointGenerator(gen);
        if (point == 0) {
            childA->genome[i] = parentA->genome[i];
            childB->genome[i] = parentB->genome[i];
        } else {
            childB->genome[i] = parentA->genome[i];
            childA->genome[i] = parentB->genome[i];
        }
    }
}
