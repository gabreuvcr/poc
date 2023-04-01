#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP
#include <random>

namespace Constants {
    const double COVERAGE_THRESHOLD = 0.9;
    const double A1 = 1;
    const double A2 = 0;
    const double B1 = 1;
    const double B2 = 0.5;
    const double F = 0.15;
    const double CR_DE = 0.2;
    const double MR_C = 0.15;
    const double CR_C = 0.95;
    const double FLIP_COIN = 0.5;
    const int SEED = 3842;
    // const int SEED = std::random_device{}();
};

#endif