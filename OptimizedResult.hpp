#pragma once

#include <string>
#include "../misc/Values.hpp"
#include "ObjectiveParameters.hpp"

using namespace std;

class OptimizedResult {
public:

    OptimizedResult(
        const string& stop_reson,
        const ObjectiveParameters& best_params,
        float best_result,
        void* data = nullptr
    ):
        stop_reson(stop_reson),
        best_params(best_params),
        best_result(best_result),
        data(data)
    {}

    virtual ~OptimizedResult() {}

    const string stop_reson;
    const ObjectiveParameters best_params;
    const float best_result;
    const void* data = nullptr;
};
