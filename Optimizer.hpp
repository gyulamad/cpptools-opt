#pragma once

#include "../misc/Initializable.hpp"
#include "OptimizerDirection.hpp"
#include "OptimizedResult.hpp"
#include "OptimizerObjective.hpp"
#include "../misc/array_key_exists.hpp"
#include "../misc/array_dump.hpp"

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
        const string direction = inifile.get<string>("direction");
        if (!array_key_exists(direction, OPTIMIZER_DIRECTION_MAP)) {
            throw ERROR(
                "Invalud optimization direction: '" + F(F_DEBUG, direction) + "'"
                " - possible value(s): " + array_dump(array_keys(OPTIMIZER_DIRECTION_MAP)) +
                " - .ini file: " + F(F_FILE, getIniFileCRef().getFilenameCRef())
            );
        }
        this->direction = OPTIMIZER_DIRECTION_MAP.at(direction);
    }

    void reset(const IniData* inidata = nullptr) override {
        Initializable::reset(inidata);
        this->direction = OPTIMIZER_DIRECTION_MAP.at(inifile.get<string>("direction"));
    } 

    OptimizerDirection getDirection() const { return direction; }

    virtual OptimizedResult optimize(
        OptimizerObjective* objective, 
        const ObjectiveParameters& iguess
    ) = 0;

protected:
    OptimizerDirection direction = UNSET;
};
