#pragma once

#include "../../genetic/selector.h"

#include <chrono>
#include <random>

namespace VRP {
    class LinearSelector : public genetic::Selector {
    private:
        double parentsPercentage;
        double tournamentSizePercentage;
        double elitismPercentage;

        std::mt19937 gen;
        std::uniform_real_distribution<> posGenerator;
        uniform_int_distribution<> valueSelector;
    public:
        LinearSelector(int generationSize, double parentsPercentage, double tournamentSizePercentage, double elitismPercentage);
        vector<pair<int, int>> select(genetic::IndividualArray &individuals, double *fitness);

    private:
        void chooseParents(vector<int> &parents, vector<pair<int, int>> &parentPairs);
    };
}
