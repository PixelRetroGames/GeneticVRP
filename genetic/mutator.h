#pragma once

#include "individual.h"

namespace genetic {
    class Mutator {
    public:
        virtual void mutate(IndividualArray &individuals) = 0;
    };
}
