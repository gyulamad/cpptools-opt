#pragma once

#include <map>
#include <string>

using namespace std;

enum OptimizerDirection { UNSET = 0, MIN, MAX };
const map<string, OptimizerDirection> OPTIMIZER_DIRECTION_MAP = {
    { "MIN", MIN },
    { "MAX", MAX },
};