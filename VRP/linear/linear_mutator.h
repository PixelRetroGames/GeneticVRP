#pragma once

#include <chrono>
#include <random>

#include "../../genetic/mutator.h"
#include "../VRP_individual.h"

namespace VRP {
    class LinearMutator : public genetic::Mutator {
    private:
        std::mt19937 gen;
        std::uniform_real_distribution<> mutationChanceGenerator;
        std::uniform_real_distribution<> mutationGenerator;
        std::uniform_real_distribution<> mutationGeneGenerator;
        int genomeSize;
        double mutationIndividual;
        double mutationGene;
        double maxMutation;
        double globalMutationRate;
        double globalMutationLikeness;
    public:
        LinearMutator(int genomeSize, double mutationIndividual, double mutationGene, double maxMutation, double globalMutationRate, double globalMutationLikeness);
        void mutate(genetic::IndividualArray &individuals);

    private:
        void mutate(VRP::VRPIndividual *individual, int globalGene);
        void mutateGene(double &genome, double mutation);
    };
}
