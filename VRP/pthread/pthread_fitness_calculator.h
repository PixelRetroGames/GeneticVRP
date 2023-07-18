#pragma once

#include "../../genetic/fitness_calculator.h"
#include "../problem.h"
#include "../VRP_individual.h"
#include "../best_individual_keeper.h"

#include <time.h>
#define HAVE_STRUCT_TIMESPEC
#include "pthread.h"

namespace VRP {
    struct PthreadWrapper;

    class PthreadFitnessCalculator : public genetic::FitnessCalculator {
    private:
        int numThreads;
        VRP::Problem *problem;
        int numVehicles;
        BestIndividualKeeper *keeper;

        pthread_barrier_t barrierStart;
        pthread_barrier_t barrierEnd;

        pthread_t *threads;
        PthreadWrapper *threadArgs;

        vector<vector<pair<double, int>>> *cars;

    public:
        PthreadFitnessCalculator(int numThreads, int genomeSize, VRP::Problem *problem, int numVehicles, BestIndividualKeeper *keeper);

        void calculate(genetic::IndividualArray &individuals, double *fitness);

    private:
        static void *calculateIntervalWrapper(void *arg);

        void calculateInterval(int id, double *genome, int len, int genomeSize, double *fitness);
        double calculate(int id, double *genome, int genomeSize, bool print = false);
        double getDistance(pair<int, int> a, pair<int, int> b);
        void printIndividual(VRP::VRPIndividual *individual, int genomeSize);

        void sort(genetic::IndividualArray &individuals, double *fitness);
    };

  struct PthreadWrapper {
        int id;
        double *genome;
        int len;
        int genomeSize;
        double *fitness;
        PthreadFitnessCalculator *solver;

        pthread_barrier_t *barrierStart;
        pthread_barrier_t *barrierEnd;
    };
}
