#pragma once

#include "problem.h"

namespace VRP {
    class InputParser {
    public:
        Problem *Parse(string filename);
    };
}
