#include "solution_writer.h"
#include <cmath>
#include <algorithm>

double getDistance(pair<int, int> a, pair<int, int> b) {
    //printf("A = %d; %d\n", a.first, a.second);
    //printf("B = %d; %d\n", b.first, b.second);
    //printf("getDistance = %lf\n", sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second)));
    return sqrt((a.first - b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}

void VRP::SolutionWriter::Write(VRP::VRPIndividual *individual, int numVehicles, int genomeSize, VRP::Problem *problem, std::string filename) {
    FILE *out = fopen(filename.c_str(), "w");

    vector<vector<pair<double, int>>> cars(numVehicles);
    int car;
    for (int i = 0; i < genomeSize; i++) {
        car = floor(individual->genome[i]);
        cars[car].push_back(make_pair(individual->genome[i] - car, i));
    }

    int carNr = 1;
    for (auto &car : cars) {
        sort(car.begin(), car.end());
        fprintf(out, "Route #%d: ", carNr);
        for (auto p : car) {
            fprintf(out, "%d ", p.second + 2);
        }
        fprintf(out, "\n");
        carNr++;
    }

    int request;
    pair<int, int> depot = {problem->nodes[0].x, problem->nodes[0].y};
    pair<int, int> pos = depot;
    pair<int, int> requestPos;
    double totalDistance = 0, maxDistance = 0;
    int capacity;
    double penalization = 0;
    for (auto car : cars) {
        pos = depot;
        totalDistance = 0;
        capacity = problem->capacity;
        for (auto p : car) {
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

    fprintf(out, "Cost %lf", maxDistance);

    fclose(out);
}
