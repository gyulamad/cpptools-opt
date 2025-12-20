#pragma once

// #include <stdint.h>

template<typename real = float> // , typename S = uint32_t>
class ObjectiveParametersT: public ValuesT<real> { // , S> {
public:
    ObjectiveParametersT(): ValuesT<real>() {} // , S>() {}
    virtual ~ObjectiveParametersT() {}

protected:

};

// template<typename... Ts>
// ObjectiveParametersT(...) -> ObjectiveParametersT<float, uint32_t>;

using ObjectiveParameters = ObjectiveParametersT<float>; // , uint32_t>;
