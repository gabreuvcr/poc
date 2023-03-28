#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <random>

namespace Constants {
    const double HMCR = 0.9;
    const double PAR = 0.3;
    const double BW = 0.2;
    const double COVERAGE_THRESHOLD = 0.9;
    const double A1 = 1;
    const double A2 = 0;
    const double B1 = 1;
    const double B2 = 0.5;
    const int SEED = 1234;
    // const int SEED = std::random_device{}();
};

#endif