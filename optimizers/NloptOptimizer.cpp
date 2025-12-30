// For initialization values (.ini file) see NloptOptimizer.md

// DEPENDENCY: stevengj/nlopt:v2.10.1

#include <math.h>                                      // for INFINITY, NAN
#include <exception>                                   // for exception
#include <functional>                                  // for function
#include <iostream>                                    // for basic_ostream
#include <map>                                         // for map
#include <nlopt.hpp>                                   // for opt, result
#include <string>                                      // for char_traits
#include <vector>                                      // for vector
#include "../../misc/ERROR.hpp"                                // for ERROR
#include "../../misc/EXTERN.hpp"                               // for EXTERN
#include "../../misc/array_dump.hpp"                           // for array_dump
#include "../../misc/vector_cast.hpp"                          // for vector_cast
#include "../../misc/Values.hpp"
#include "../../misc/vector_cast.hpp"
#include "../OptimizerObjective.hpp"
#include "../ObjectiveParameters.hpp"
#include "../Optimizer.hpp"                            // for Optimizer

using namespace std;

class NloptOptimizer: public Optimizer {
public:
    NloptOptimizer(): Optimizer() {}
    virtual ~NloptOptimizer() {}


    virtual void onLoad() {
        // TODO ...
    }

    virtual void onReload() {
        this->algo = inifile.get<string>("algo");
        this->relativeToleranceX = inifile.get<double>("relativeToleranceX");
        this->relativeToleranceF = inifile.get<double>("relativeToleranceF");
        this->maxEval = inifile.get<int>("maxEval");
        this->maxTime = inifile.get<double>("maxTime");
        initialized = true;
    }

    virtual OptimizedResult optimize(
        OptimizerObjective* objective, 
        const ObjectiveParameters& iguess
    ) override {
        SAFE(objective);

        if (iguess.getVariables().empty())
            throw ERROR("Initial guess is empty");
            
        if (!initialized) {
            LOG("Optimizer is not initialized, call reset()..");
            reset();
            if (!initialized)
                throw ERROR("Optimizer is not initialized, even after reset().");
        }
        this->objective = objective;
        this->parameters = iguess;
        try {
            // Create optimizer (NLOPT_LD_MMA = Method of Moving Asymptotes)
            nlopt::opt opt(algo.c_str(), iguess.getVariables().size());

            vector<double> iguess_values = vector_cast<double>(iguess.getVariables());
                        
            // Set objective function  
            best_params = iguess;
            switch (direction) {
                case MIN: 
                    best_result = INFINITY;
                    opt.set_min_objective(nlopt_objective, this); 
                    break;
                case MAX: 
                    best_result = -INFINITY;
                    opt.set_max_objective(nlopt_objective, this); 
                    break;
                default:
                    throw ERROR("Invalid optimizer direction");
            }
            
            // Set bounds:
            vector<double> lower_bounds = vector_cast<double>(iguess.getLowerBounds());
            vector<double> upper_bounds = vector_cast<double>(iguess.getUpperBounds());
            opt.set_lower_bounds(lower_bounds);
            opt.set_upper_bounds(upper_bounds);

            // TODO Add inequality constraint:
            // opt.add_equality_constraint();
            // opt.add_equality_mconstraint();
            // opt.add_inequality_constraint(constraint_function, nullptr, 1e-8);
            // opt.add_inequality_mconstraint();
            

            // Set stopping criteria
            opt.set_xtol_rel(relativeToleranceX);     // Relative tolerance on optimization parameters
            opt.set_ftol_rel(relativeToleranceF);     // Relative tolerance on function value
            opt.set_maxeval(maxEval < 0 ? __INT_MAX__ : maxEval);      // Maximum number of function evaluations
            opt.set_maxtime(maxTime < 0 ? INFINITY : maxTime); //  (in seconds)


            // Optimize
            cout << "Starting optimization..." << endl;

            double opt_f;
            vector<double> params = iguess_values;
            nlopt::result result = opt.optimize(params, opt_f);


            // Display results
            cout << "\nOptimization completed!" << endl;
            cout << "Result code: " << result << " (";
            
            string stop_reason;
            switch(result) {
                case nlopt::SUCCESS:
                    cout << (stop_reason = "SUCCESS");
                    break;
                case nlopt::STOPVAL_REACHED:
                    cout << (stop_reason = "STOPVAL_REACHED");
                    break;
                case nlopt::FTOL_REACHED:
                    cout << (stop_reason = "FTOL_REACHED");
                    break;
                case nlopt::XTOL_REACHED:
                    cout << (stop_reason = "XTOL_REACHED");
                    break;
                case nlopt::MAXEVAL_REACHED:
                    cout << (stop_reason = "MAXEVAL_REACHED");
                    break;
                case nlopt::MAXTIME_REACHED:
                    cout << (stop_reason = "MAXTIME_REACHED");
                    break;
                default:
                    cout << (stop_reason = "OTHER (" + to_string(result) + ")");
                    break;
            }
            cout << ")" << endl;
            cout << "Optimal value: " << opt_f << endl;
            cout << "Number of evaluations: " << opt.get_numevals() << endl;

            if (result <= 0)
                throw ERROR("Optimization failed, stop reason: " + stop_reason);

            OptimizedResult optimal(
                stop_reason,
                best_params,
                best_result
            );
            return optimal; // return a full result object: { bool best_result; ParameterSet best_params; enum? stop_reson; }
    
        } catch (exception &e) {
            throw ERROR("NLopt failed" + EWHAT);
        }
    }

    static double nlopt_objective(
        const vector<double>& params, vector<double>& grad, void* optimizer_void
    ) {
        // cout << "a:" << params.size() << " b:" << grad.size() << " c:" << objective << endl;
        return ((NloptOptimizer*)optimizer_void)
            ->instance_nlopt_objective(params, grad);
    }

    double instance_nlopt_objective(
        const vector<double>& params, vector<double>& /*grad*/
    ) {        
        try {
            LOG("Parameters: " + array_dump(params));
            // cout << "Parameters: " << array_dump(params) << endl;
            parameters.setVariables(vector_cast<float>(params));
            double result = objective->call(parameters);
            bool store = false;
            switch (direction) {
                case MIN: 
                    if (best_result >= result) store = true;
                    break;
                case MAX: 
                    if (best_result <= result) store = true;
                    break;
                default:
                    throw ERROR("Invalid optimizer direction");
            }
            if (store) {
                best_result = result;
                best_params.setVariables(parameters.getVariables());
            }
            LOG("Result: " + to_string(result) + " (best: " + to_string(best_result) + ")");
            // cout << "Result: " << result << " (best: " << that->best_result << ")" << endl;
            return result;
        } catch (exception &e) {
            LOG_OR_EXIT("Objective failed"); // TODO: nlopt swallows the exceptions and continues, to resolve it we just exit but have to investigate if we can ask nlopt to not doing this...
            // string errmsg = "Objective failed" + EWHAT;
            // LOG_ERROR(errmsg);
            // if (throwsOnError) 
            //     exit(1); // throw ERROR(errmsg);
            // // cerr << "Objective failed: " << e.what() << endl;
        }
        return NAN;
    };

private:
    bool initialized = false;

    // for nlopt
    string algo;
    double relativeToleranceX;
    double relativeToleranceF;
    int maxEval;
    double maxTime;

    // for optimizer
    OptimizerObjective* objective = nullptr;
    ObjectiveParameters parameters;
    float best_result;
    ObjectiveParameters best_params;
};

EXTERN(NloptOptimizer, (), ())