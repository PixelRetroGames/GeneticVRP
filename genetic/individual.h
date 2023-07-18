#pragma once
#include <vector>

using namespace std;

namespace genetic {
    class Individual {
    };

    class IndividualArray {
        private:
        int genomeSize;
        int arrSize;
        vector<Individual*> individuals;
        void *genome;

        public:
        IndividualArray(vector<Individual*> individuals, int arrSize, int genomeSize, void *genome);
        IndividualArray();
        vector<Individual*> *getIndividuals();
        int size();
        int getGenomeSize();
        void *getGenome();
    };
}
