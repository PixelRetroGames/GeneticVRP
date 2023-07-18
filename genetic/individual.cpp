#include "individual.h"

genetic::IndividualArray::IndividualArray(vector<Individual*> individuals, int arrSize, int genomeSize, void *genome) {
    this->individuals = individuals;
    this->arrSize = arrSize;
    this->genomeSize = genomeSize;
    this->genome = genome;
}

genetic::IndividualArray::IndividualArray() {
}

vector<genetic::Individual*> *genetic::IndividualArray::getIndividuals() {
    return &individuals;
}

int genetic::IndividualArray::size() {
    return arrSize;
}

int genetic::IndividualArray::getGenomeSize() {
    return genomeSize;
}

void *genetic::IndividualArray::getGenome() {
    return genome;
}
