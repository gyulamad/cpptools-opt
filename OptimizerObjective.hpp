#pragma once

#include "OptimizerDirection.hpp"
#include "../misc/Values.hpp"

using namespace std;

// using OptimizerObjective function<float (const Values&)>;

class OptimizerObjective {
public:
    OptimizerObjective() {}
    virtual ~OptimizerObjective() {}

    virtual void init(void*, void* = nullptr, OptimizerDirection = MAX) = 0;
    virtual float call(const Values&) = 0;
};
