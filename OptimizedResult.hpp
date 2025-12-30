#pragma once

#include <string>
#include "ObjectiveParameters.hpp"

using namespace std;

class OptimizedResult {
public:

    OptimizedResult(
        const string& stop_reson,
        const ObjectiveParameters& best_params,
        float best_result
    ):
        stop_reson(stop_reson),
        best_params(best_params),
        best_result(best_result)
    {}

    virtual ~OptimizedResult() {}

    const string stop_reson;
    const ObjectiveParameters best_params;
    const float best_result;
};
