#pragma once

#include <vector>
#include <functional>
#include <map>
#include "../misc/Initializable.hpp"
#include "../misc/Values.hpp"
#include "OptimizedResult.hpp"
#include "ObjectiveParameters.hpp"
#include "OptimizerObjective.hpp"
#include "OptimizerDirection.hpp"

using namespace std;

class Optimizer: public Initializable {
public:
    // enum Direction { MIN, MAX };
    // const map<string, Direction> DIRECTION_MAP = {
    //     { "MIN", MIN },
    //     { "MAX", MAX },
    // };

    // TODO: add initializer to the shared libs
    bool throwsOnError = true;
    Optimizer() {}
    virtual ~Optimizer() {}

    void init(const string& inifname, bool createIfNotExists, bool throwsIfNotExists) override {
        Initializable::init(inifname, createIfNotExists, throwsIfNotExists);
        this->direction = OPTIMIZER_DIRECTION_MAP.at(inifile.get<string>("direction"));
    }

    void reset(const IniData* inidata = nullptr) override {
        Initializable::reset(inidata);
        this->direction = OPTIMIZER_DIRECTION_MAP.at(inifile.get<string>("direction"));
    } 

    OptimizerDirection getDirection() const { return direction; }

    virtual OptimizedResult optimize(
        OptimizerObjective* objective, 
        const ObjectiveParameters& iguess,
        void* data = nullptr
    ) = 0;

protected:
    OptimizerDirection direction = UNSET;
};
