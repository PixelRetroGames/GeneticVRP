#pragma once

#include "../genetic/best_individual_keeper.h"
#include "VRP_individual.h"

namespace VRP {
    class BestIndividualKeeper : public genetic::BestIndividualKeeper {
    private:
        VRP::VRPIndividual *best;
        double fitness;

    public:
        BestIndividualKeeper(int genomeSize);
        void keepBestIndividual(int pos, genetic::IndividualArray &individuals, double fitness);
        genetic::Individual *getBest();
        double getFitness();
    };
}
