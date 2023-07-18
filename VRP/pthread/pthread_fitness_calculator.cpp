#include "pthread_fitness_calculator.h"
#include "../VRP_individual.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <float.h>
#include <ctime>

using namespace VRP;
using namespace std;

VRP::PthreadFitnessCalculator::PthreadFitnessCalculator(int numThreads, int genomeSize, VRP::Problem* problem, int numVehicles, BestIndividualKeeper* keeper)
    : numThreads(numThreads), problem(problem), numVehicles(numVehicles), keeper(keeper) {
    threads = new pthread_t[numThreads];
    threadArgs = new PthreadWrapper[numThreads];

    pthread_barrier_init(&barrierStart, NULL, numThreads + 1);
    pthread_barrier_init(&barrierEnd, NULL, numThreads + 1);

    cars = new vector<vector<pair<double, int>>>[numThreads];

    for(int i = 0; i < numThreads; i++) {
        cars[i] = vector<vector<pair<double, int>>>(numVehicles, vector<pair<double, int>>(genomeSize + 1, {0, 0}));
        threadArgs[i].id = i;
        threadArgs[i].barrierStart = &barrierStart;
        threadArgs[i].barrierEnd = &barrierEnd;
        pthread_create(&threads[i], NULL, calculateIntervalWrapper, &threadArgs[i]);
    }
}

vector<int> static sortIndividuals(int n, genetic::IndividualArray &individuals) {
    vector<int> idxs(n);
    for(int i = 0; i < n; i++) {
      idxs[i] = i;
    }

    sort(idxs.begin(), idxs.end(),
        [&](int a, int b) {
           return ((VRP::VRPIndividual *) (*individuals.getIndividuals())[a])->genome < ((VRP::VRPIndividual *) (*individuals.getIndividuals())[b])->genome;
        }
    );

    return idxs;
}

void VRP::PthreadFitnessCalculator::calculateInterval(int id, double *genome, int len, int genomeSize, double *fitness) {
    for (int i = 0; i < len; i++) {
        if (i % 500 == 0) {
            //printf("%d\n", i);
        }
        fitness[i] = calculate(id, genome + i * genomeSize, genomeSize);
    }
}

void *VRP::PthreadFitnessCalculator::calculateIntervalWrapper(void *arg) {
    PthreadWrapper *wrapper = (PthreadWrapper *)arg;
    while (true) {
        //printf("Waiting Start\n");
        pthread_barrier_wait(wrapper->barrierStart);
        //printf("Start\n");
        (wrapper->solver)->calculateInterval(wrapper->id, wrapper->genome, wrapper->len,
                                           wrapper->genomeSize, wrapper->fitness);
        //printf("End\n");

        pthread_barrier_wait(wrapper->barrierEnd);
        //printf("After End\n");
    }
}

void VRP::PthreadFitnessCalculator::calculate(genetic::IndividualArray &individuals, double *fitness) {
    double *genome = (double *) individuals.getGenome();
    auto positions = sortIndividuals(individuals.size(), individuals);

    double mean = 0;
    double maxFitness = -DBL_MAX;
    int pos = 0;

    double maxA = -DBL_MAX, maxB = -DBL_MAX;
    double *fitness2 = new double[individuals.size()];

    for(int i = 0; i < numThreads; i++) {
        int start = individuals.size() * i / numThreads;
        int end = individuals.size() * (i + 1) / numThreads;
        threadArgs[i].genome = genome + start * individuals.getGenomeSize();
        threadArgs[i].len = end - start;
        threadArgs[i].genomeSize = individuals.getGenomeSize();
        threadArgs[i].fitness = fitness2 + start;
        threadArgs[i].solver = this;
    }

    double t = time(NULL);
    pthread_barrier_wait(&barrierStart);
    pthread_barrier_wait(&barrierEnd);
    //printf("%lf\n", time(NULL) - t);

    /*for(int i = 0; i < numThreads; i++) {
      pthread_join(threads[i], NULL);
    }*/

    for (int i = 0; i < individuals.size(); i++) {
        fitness[positions[i]] = fitness2[i];
        mean += fitness[positions[i]];
        maxFitness = max(maxFitness, fitness[positions[i]]);
        if (maxFitness == fitness[positions[i]]) {
            pos = positions[i];
        }
    }

    delete fitness2;

    keeper->keepBestIndividual(pos, individuals, maxFitness);

    static int generation = 0;
    generation++;

    mean /= individuals.size();

    if (generation % 200 == 0) {
        printf("%d Generation fitness %lf\n", generation, mean);
        printf("Max generation fitness %lf\n", maxFitness);
    }
}

vector<int> static sortIndividualsByFitness(int n, double *fitness) {
    vector<int> idxs(n);
    for(int i = 0; i < n; i++) {
      idxs[i] = i;
    }

    sort(idxs.begin(), idxs.end(),
        [&](int a, int b) {
            return (fitness[a] > fitness[b]);
        }
    );

    return idxs;
}

void VRP::PthreadFitnessCalculator::sort(genetic::IndividualArray &individuals, double *fitness) {
    vector<int> sortedPos = sortIndividualsByFitness(individuals.size(), fitness);
    vector<int> invSortedPos(sortedPos.size());
    for(int i = 0; i < sortedPos.size(); i++) {
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

double VRP::PthreadFitnessCalculator::calculate(int id, double *genome, int genomeSize, bool print) {
    auto &myCars = cars[id];
    for (auto &car : myCars) {
        car[car.size() - 1] = {0, 0};
    }

    for (int i = 0; i < genomeSize; i++) {
        auto &car = myCars[floor(genome[i])];
        car[car[car.size() - 1].second++] = make_pair(genome[i] - floor(genome[i]), i);
    }

    for (auto &car : myCars) {
        //printf("%d\n", car[car.size() - 1].second);
        std::sort(car.begin(), car.begin() + car[car.size() - 1].second);
    }

    int request;
    pair<int, int> depot = {problem->nodes[0].x, problem->nodes[0].y};
    pair<int, int> pos = depot;
    pair<int, int> requestPos;
    double totalDistance = 0, maxDistance = 0;
    int capacity;
    double penalization = 0;
    for (auto &car : myCars) {
        pos = depot;
        totalDistance = 0;
        capacity = problem->capacity;
        for (int i = 0; i < car[car.size() - 1].second; i++) {
            auto &p = car[i];
            request = p.second + 1;
            requestPos = {problem->nodes[request].x, problem->nodes[request].y};
            totalDistance += getDistance(pos, requestPos);
            pos = requestPos;
            capacity -= problem->demands[request].quantity;
        }

        penalization += max(0, -capacity);

        totalDistance += getDistance(pos, depot);
        maxDistance += totalDistance;
    }

    double fitness = -(maxDistance + penalization * penalization * penalization * penalization);

    if (print) {
        printf("maxDistance = %lf\n", maxDistance);
        printf("penalization = %lf\n", penalization);
        printf("fitness= %lf\n", fitness);
    }

    return fitness;
}

double VRP::PthreadFitnessCalculator::getDistance(pair<int, int> a, pair<int, int> b) {
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}
