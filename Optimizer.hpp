#pragma once

#include "../misc/Initializable.hpp"
#include "OptimizerDirection.hpp"
#include "OptimizedResult.hpp"
#include "OptimizerObjective.hpp"
#include "../misc/array_key_exists.hpp"
#include "../misc/array_dump.hpp"

using namespace std;

#define EXTERN_OPTIMIZER(clazz) EXTERN_INITIALIZABLE(clazz)

class Optimizer: public Initializable {
public:
    Optimizer(
        const string& inifname, 
        bool load,
        bool createIfNotExists,
        bool throwsIfNotExists,
        bool warnsIfNotExists,
        bool verbose
    ):
        Initializable(
            inifname,
            load,
            createIfNotExists,
            throwsIfNotExists,
            warnsIfNotExists,
            verbose
        )
    {
        // Initializable::init(inifname, createIfNotExists, throwsIfNotExists);
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

    virtual ~Optimizer() {}
    
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
