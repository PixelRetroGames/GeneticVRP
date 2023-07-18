#pragma once

#include "individual.h"
#include <vector>

using namespace std;

namespace genetic {
    class Selector {
    public:
        virtual vector<pair<int, int>> select(IndividualArray &individuals, double *fitness) = 0;
    };
}
