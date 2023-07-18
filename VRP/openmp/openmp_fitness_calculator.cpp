#include "openmp_fitness_calculator.h"
#include "../VRP_individual.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <float.h>

using namespace VRP;
using namespace std;

vector<int> static sortIndividuals(int n, genetic::IndividualArray& individuals) {
    vector<int> idxs(n);
    for (int i = 0; i < n; i++) {
        idxs[i] = i;
    }

    sort(idxs.begin(), idxs.end(),
        [&](int a, int b) {
            return ((VRP::VRPIndividual*)(*individuals.getIndividuals())[a])->genome < ((VRP::VRPIndividual*)(*individuals.getIndividuals())[b])->genome;
        }
    );

    return idxs;
}

void VRP::OpenmpFitnessCalculator::calculateInterval(double* genome, int len, int genomeSize, vector<double>& fitness) {
    int i;
    #pragma omp parallel for private(i)
    for (i = 0; i < len; i++) {
        fitness[i] = calculate(genome + i * genomeSize, genomeSize);
    }
}

void VRP::OpenmpFitnessCalculator::calculate(genetic::IndividualArray& individuals, double* fitness) {
    double* genome = (double*)individuals.getGenome();
    auto positions = sortIndividuals(individuals.size(), individuals);

    double mean = 0;
    double maxFitness = -DBL_MAX;
    int pos = 0;

    double maxA = -DBL_MAX, maxB = -DBL_MAX;
    vector<double> fitness2(individuals.size());
    calculateInterval(genome, individuals.size(), individuals.getGenomeSize(), fitness2);

    for (int i = 0; i < fitness2.size(); i++) {
        fitness[positions[i]] = fitness2[i];
        mean += fitness[positions[i]];
        maxFitness = max(maxFitness, fitness[positions[i]]);
        if (maxFitness == fitness[positions[i]]) {
            pos = positions[i];
        }
    }

    keeper->keepBestIndividual(pos, individuals, maxFitness);

    static int generation = 0;
    generation++;

    mean /= individuals.size();

    if (generation % 200 == 0) {
        printf("%d Generation fitness %lf\n", generation, mean);
        printf("Max generation fitness %lf\n", maxFitness);
    }
}

vector<int> static sortIndividualsByFitness(int n, double* fitness) {
    vector<int> idxs(n);
    for (int i = 0; i < n; i++) {
        idxs[i] = i;
    }

    sort(idxs.begin(), idxs.end(),
        [&](int a, int b) {
            return (fitness[a] > fitness[b]);
        }
    );

    return idxs;
}

void VRP::OpenmpFitnessCalculator::sort(genetic::IndividualArray& individuals, double* fitness) {
    vector<int> sortedPos = sortIndividualsByFitness(individuals.size(), fitness);
    vector<int> invSortedPos(sortedPos.size());
    for (int i = 0; i < sortedPos.size(); i++) {
        invSortedPos[sortedPos[i]] = i;
    }

    for (int i = 0; i < sortedPos.size() / 2; i++) {
        int dest = invSortedPos[i];
        swap((*individuals.getIndividuals())[sortedPos[i]], (*individuals.getIndividuals())[i]);
        swap(fitness[sortedPos[i]], fitness[i]); // delete after testing
        swap(invSortedPos[sortedPos[i]], invSortedPos[dest]);
        swap(sortedPos[i], sortedPos[dest]);
    }
}

void VRP::OpenmpFitnessCalculator::printIndividual(VRP::VRPIndividual* individual, int genomeSize) {
    vector<vector<pair<double, int>>> cars(numVehicles);
    int car;
    for (int i = 0; i < genomeSize; i++) {
        car = floor(individual->genome[i]);
        cars[car].push_back(make_pair(individual->genome[i] - car, i));
    }

    int carNr = 0;
    for (auto& car : cars) {
        std::sort(car.begin(), car.end());
        printf("Car #%d: ", carNr);
        for (auto p : car) {
            printf("%d ", p.second + 2);
        }
        printf("\n");
        carNr++;
    }

    /*for (int i = 0; i < genomeSize; i++) {
        printf("%lf, ", individual->genome[i]);
    }*/
    printf("\n");
}

double VRP::OpenmpFitnessCalculator::calculate(double* genome, int genomeSize, bool print) {
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

double VRP::OpenmpFitnessCalculator::getDistance(pair<int, int> a, pair<int, int> b) {
    //printf("A = %d; %d\n", a.first, a.second);
    //printf("B = %d; %d\n", b.first, b.second);
    //printf("getDistance = %lf\n", sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)));
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}
