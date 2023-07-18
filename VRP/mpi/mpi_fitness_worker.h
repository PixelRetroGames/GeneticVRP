#pragma once

#include "../../genetic/fitness_calculator.h"
#include "../problem.h"
#include "../VRP_individual.h"
#include "../best_individual_keeper.h"

namespace VRP {
    class MpiFitnessWorker {
    private:
        VRP::Problem* problem;
        int numVehicles;
        int genomeSize;
        int maxSize;

        double* fitness;
        double* genome;

    public:
        MpiFitnessWorker(VRP::Problem* problem, int numVehicles, int genomeSize, int maxSize)
            : problem(problem), numVehicles(numVehicles), genomeSize(genomeSize), maxSize(maxSize) {
            fitness = (double*)calloc(maxSize, sizeof(double));
            genome = (double*)calloc(maxSize * genomeSize, sizeof(double));
        }
        void calculateInterval();

    private:
        double calculate(double* genome, int genomeSize, bool print = false);
        double getDistance(pair<int, int> a, pair<int, int> b);
    };
}