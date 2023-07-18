#pragma once

#include "individual.h"

namespace genetic {
    class BestIndividualKeeper {
    public:
        virtual void keepBestIndividual(int pos, IndividualArray &individuals, double fitness) = 0;
        virtual Individual *getBest() = 0;
        virtual double getFitness() = 0;
    };
}
