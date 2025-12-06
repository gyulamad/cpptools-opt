#pragma once

#include <functional>
#include "../misc/Values.hpp"
#include "OptimizerDirection.hpp"

using namespace std;

// using OptimizerObjective function<float (const Values&)>;

class OptimizerObjective {
public:
    OptimizerObjective() {}
    virtual ~OptimizerObjective() {}

    virtual void init(void*, OptimizerDirection) = 0;
    virtual float call(const Values&) = 0;
};
