#pragma once

#include "../../genetic/crossover_calculator.h"
#include "../VRP_individual.h"
#include "linear_initializer.h"

#include <chrono>
#include <random>

namespace VRP {
    class LinearCrossoverCalculator : public genetic::CrossoverCalculator {
    private:
        std::mt19937 gen;
        std::uniform_int_distribution<> pointGenerator;
        int genomeSize;

        double elitismPercentage;
        double newRandomIndividualsPercentage;

        VRP::LinearInitializer *linearInitializer;

        std::vector<int> availableParents;

    public:
        LinearCrossoverCalculator(int generationSize, int genomeSize, double elitismPercentage, double newRandomIndividualsPercentage, VRP::LinearInitializer *linearInitializer);

        void crossover(genetic::IndividualArray &individuals, genetic::IndividualArray &individualsChildren, vector<pair<int, int>> &parents);
    private:
        void copyInNextGen(VRP::VRPIndividual *parentA, VRP::VRPIndividual *childA);

        void crossover(VRP::VRPIndividual *parentA, VRP::VRPIndividual *parentB,
                       VRP::VRPIndividual *childA, VRP::VRPIndividual *childB);
    };
}
