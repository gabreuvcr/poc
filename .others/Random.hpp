#ifndef RANDOM_H
#define RANDOM_H

#include <iostream>
#include <cmath>
#include <random>

class Random {
    public:
        Random();

        std::mt19937 rng;
        std::uniform_real_distribution<> dist_0_1;

        double random();
        int random_value(int min, int max);
        double random_value(double min, double max);
};

#endif