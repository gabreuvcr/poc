#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <cmath>
#include <random>

namespace Random {
    extern std::mt19937 rng;
    extern std::uniform_real_distribution<> dist_0_1;

    double random();
    int random_value(int min, int max);
    double random_value(double min, double max);
};

#endif