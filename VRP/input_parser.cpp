#include "input_parser.h"
#include <cstring>

#define BUFF_LEN 256

using namespace VRP;

VRP::Problem *VRP::InputParser::Parse(string filename) {
    FILE *in = fopen(filename.c_str(), "r");
    if (in == NULL) {
        fprintf(stderr, "Can't open file %s\n", filename.c_str());
        return nullptr;
    }

    Problem *problem = new Problem();

    char buff[BUFF_LEN];

    // Read header
    fscanf(in, "NAME : %s ", buff);
    problem->name = buff;
    fscanf(in, "COMMENT : ");
    fgets(buff, BUFF_LEN, in);
    buff[strlen(buff) - 1] = '\0';
    problem->comment = buff;
    fscanf(in, "TYPE : %s ", buff);
    problem->type = buff;
    fscanf(in, "DIMENSION : %d ", &problem->dimension);
    fscanf(in, "EDGE_WEIGHT_TYPE : %s ", buff);
    problem->edgeWeightType = buff;
    fscanf(in, "CAPACITY : %d ", &problem->capacity);

    fscanf(in, "NODE_COORD_SECTION ");
    // Allocate memory
    problem->nodes = new Node[problem->dimension];
    problem->demands = new Demand[problem->dimension];

    // Read nodes
    for (int i = 0; i < problem->dimension; i++) {
        int pos, x, y;
        fscanf(in, "%d %d %d ", &pos, &x, &y);
        problem->nodes[pos - 1] = Node(x, y);
    }

    fscanf(in, "DEMAND_SECTION ");
    // Read demands
    for (int i = 0; i < problem->dimension; i++) {
        int pos, quantity;
        fscanf(in, "%d %d ", &pos, &quantity);
        problem->demands[pos - 1] = Demand(quantity);
    }

    fscanf(in, "DEPOT_SECTION ");
    fscanf(in, "%d -1 EOF", &problem->depotPos);

    // Move depot node to position 0
    if (problem->depotPos != 1) {
        std::swap(problem->nodes[0], problem->nodes[problem->depotPos - 1]);
        std::swap(problem->demands[0], problem->demands[problem->depotPos - 1]);
        problem->depotPos = 0;
    }

    fclose(in);

    return problem;
}
