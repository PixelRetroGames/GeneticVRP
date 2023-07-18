#include "mpi_fitness_worker.h"

#include "../VRP_individual.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <float.h>

#include <mpi/mpi.h>

using namespace VRP;
using namespace std;

void VRP::MpiFitnessWorker::calculateInterval() {
    MPI_Status status;
    int len;
    MPI_Recv(&len, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    
    //double* genome = (double *) calloc(len * genomeSize, sizeof(double));
    MPI_Recv(genome, len * genomeSize, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
   
    //double* fitness = (double *) calloc(len, sizeof(double));
    for (int i = 0; i < len; i++) {
        fitness[i] = calculate(genome + i * genomeSize, genomeSize);
    }

    MPI_Send(fitness, len, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    //free(genome);
    //free(fitness);
}

double VRP::MpiFitnessWorker::calculate(double* genome, int genomeSize, bool print) {
    static double* g1 = nullptr;
    static double* g2 = nullptr;
    g2 = g1;
    g1 = genome;
    vector<vector<pair<double, int>>> cars(numVehicles);
    int car;
    for (int i = 0; i < genomeSize; i++) {
        car = floor(genome[i]);
        cars[car].push_back(make_pair(genome[i] - car, i));
    }

    for (auto& car : cars) {
        std::sort(car.begin(), car.end());
    }

    int request;
    pair<int, int> depot = { problem->nodes[0].x, problem->nodes[0].y };
    pair<int, int> pos = depot;
    pair<int, int> requestPos;
    double totalDistance = 0, maxDistance = 0;
    int capacity;
    double penalization = 0;
    for (auto& car : cars) {
        pos = depot;
        totalDistance = 0;
        capacity = problem->capacity;
        for (auto& p : car) {
            request = p.second + 1;
            requestPos = { problem->nodes[request].x, problem->nodes[request].y };
            totalDistance += getDistance(pos, requestPos);
            pos = requestPos;
            capacity -= problem->demands[request].quantity;
        }

        penalization += max(0, -capacity);

        totalDistance += getDistance(pos, depot);
        maxDistance += totalDistance;
    }

    //double fitness = 1 - maxDistance / (1 + abs(maxDistance)) - (penalization * penalization) / (1 + (penalization * penalization));
    //double fitness = (2 - maxDistance / (1 + abs(maxDistance)) - penalization * penalization / (1 + penalization * penalization))/2;
    double fitness = -(maxDistance + penalization * penalization * penalization * penalization);

    if (print) {
        printf("maxDistance = %lf\n", maxDistance);
        printf("penalization = %lf\n", penalization);
        printf("fitness= %lf\n", fitness);
        //printIndividual(individual, genomeSize);
    }

    return fitness;
}

double VRP::MpiFitnessWorker::getDistance(pair<int, int> a, pair<int, int> b) {
    //printf("A = %d; %d\n", a.first, a.second);
    //printf("B = %d; %d\n", b.first, b.second);
    //printf("getDistance = %lf\n", sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)));
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}
