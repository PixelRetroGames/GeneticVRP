#pragma once

#include "../genetic/individual.h"

namespace VRP {
    class VRPIndividual : public genetic::Individual {
    public:
        double *genome;

        VRPIndividual(double *genome) : genome(genome) {}
        VRPIndividual(){}
    };
}
