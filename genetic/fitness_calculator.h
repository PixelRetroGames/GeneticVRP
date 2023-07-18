#pragma once

#include "individual.h"

namespace genetic {
    class FitnessCalculator {
    public:
        virtual void calculate(IndividualArray &individuals, double *fitness) = 0;
    };
}
