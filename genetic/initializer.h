#pragma once

#include "individual.h"

namespace genetic {
    class Initializer {
    public:
        virtual void initialize(int generationSize, IndividualArray &individuals) = 0;
    };
}
