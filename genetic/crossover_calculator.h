#pragma once

#include "individual.h"

namespace genetic {
    class CrossoverCalculator {
    public:
        virtual void crossover(IndividualArray &individuals, IndividualArray &individualsChildren, vector<pair<int, int>> &parents) = 0;
    };
}
