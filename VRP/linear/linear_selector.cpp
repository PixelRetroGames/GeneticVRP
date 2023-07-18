#include "linear_selector.h"
#include <algorithm>
#include <cstdio>
#include "../VRP_individual.h"

VRP::LinearSelector::LinearSelector(int generationSize, double parentsPercentage, double tournamentSizePercentage, double elitismPercentage) {
    this->parentsPercentage = parentsPercentage;
    this->tournamentSizePercentage = tournamentSizePercentage;
    this->elitismPercentage = elitismPercentage;
    //gen = mt19937(chrono::system_clock::now().time_since_epoch().count());
    gen = mt19937(0);
    posGenerator = uniform_real_distribution<>(0, 1);
    valueSelector = uniform_int_distribution<>(0, generationSize * (generationSize + 1) / 4 - 1);
}

vector<int> sortIndividuals(int n, double *fitness) {
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

int getIndividual(int selectedValue, int populationSize) {
    return floor(populationSize + 0.5 - sqrt(populationSize * (populationSize + 1) - 4 * selectedValue + 0.25));
}

vector<pair<int, int>> VRP::LinearSelector::select(genetic::IndividualArray &individuals, double *fitness) {
    // Sorting phase
    vector<int> sortedPos = sortIndividuals(individuals.size(), fitness);
    //printf("%.2lf %.2lf %.2lf %.2lf %.2lf\n", fitness[sortedPos[0]], fitness[sortedPos[1]], fitness[sortedPos[2]], fitness[sortedPos[3]], fitness[sortedPos[4]]);
    // Survivor selection
    int numElite = elitismPercentage * individuals.size();
    int numParents = parentsPercentage * individuals.size();

    // if {x, -1} -> x is elite
    // if {x, y} -> crossover between x, y
    vector<pair<int, int>> parentPairs(numParents / 2 + numElite);

    // indices of all possible parents
    vector<int> parents(numParents);

    //printf("%lf\n", fitness[sortedPos[0]]);
    for (int i = 0; i < numElite; i++) {
        // printf("%d %lf\n", i, fitness[sortedPos[i]]);
        parentPairs[numParents / 2 + i] = {sortedPos[i], -1};
    }

    for (int i = 0; i < numParents; i++) {
        parents[i] = sortedPos[getIndividual(valueSelector(gen), individuals.size())];
    }
    chooseParents(parents, parentPairs);

    return parentPairs;
}

void VRP::LinearSelector::chooseParents(vector<int> &parents, vector<pair<int, int>> &parentPairs) {
    for (int i = 0; i < parents.size(); i += 2) {
        auto a = parentPairs[i / 2];
        int x = parents[i];
        int y = parents[i + 1];
        parentPairs[i / 2] = {parents[i], parents[i + 1]};
    }
}
