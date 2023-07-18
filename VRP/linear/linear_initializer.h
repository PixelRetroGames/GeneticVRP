#pragma once

#include "../../genetic/initializer.h"

#include <chrono>
#include <random>

namespace VRP {
    class LinearInitializer : public genetic::Initializer {
    private:
        int genomeSize;
        int numVehicles;
        std::mt19937 gen;
        std::uniform_real_distribution<> deviationGenerator;
        std::uniform_int_distribution<> vehicleGenerator;

    public:
        LinearInitializer(int genomeSize, int numVehicles);
        void initialize(int generationSize, genetic::IndividualArray &individuals);
        void initalizeGenome(double *genome);
    };
}
