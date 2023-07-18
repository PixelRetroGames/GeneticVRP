#pragma once

#include "../../genetic/fitness_calculator.h"
#include "../problem.h"
#include "../VRP_individual.h"
#include "../best_individual_keeper.h"

namespace VRP {
    class LinearFitnessCalculator : public genetic::FitnessCalculator {
    private:
        VRP::Problem *problem;
        int numVehicles;
        BestIndividualKeeper *keeper;

    public:
        LinearFitnessCalculator(VRP::Problem *problem, int numVehicles, BestIndividualKeeper *keeper)
        : problem(problem), numVehicles(numVehicles), keeper(keeper) {}

        void calculate(genetic::IndividualArray &individuals, double *fitness);

    private:
        void calculateInterval(double *genome, int len, int genomeSize, vector<double> &fitness);
        double calculate(double *genome, int genomeSize, bool print = false);
        double getDistance(pair<int, int> a, pair<int, int> b);
        void printIndividual(VRP::VRPIndividual *individual, int genomeSize);

        void sort(genetic::IndividualArray &individuals, double *fitness);
    };
}
