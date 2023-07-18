#pragma once

#include "VRP_individual.h"
#include "problem.h"
#include <string>

namespace VRP {
    class SolutionWriter {
    public:
        void Write(VRP::VRPIndividual *individual, int numVehicles, int genomeSize, VRP::Problem *problem, std::string filename);
    };
}
