#pragma once

#include <string>

using namespace std;

namespace VRP {
    struct Node {
        int x, y;

        Node(int x, int y) : x(x), y(y) {}
        Node() {}
    };

    struct Demand {
        int quantity;

        Demand() {}
        Demand(int quantity) : quantity(quantity) {}
    };

    class Problem {
    public:
        string name;
        string comment;
        string type;
        int dimension;
        string edgeWeightType;
        int capacity;
        int depotPos;

        Node *nodes;
        Demand *demands;
    };
}
