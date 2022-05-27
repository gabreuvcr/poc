#ifndef HARMONY_SEARCH_H
#define HARMONY_SEARCH_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

typedef struct sensor {
    double x, y;
    sensor() : x(-1), y(-1) {}
    sensor(double _x, double _y) : x(_x), y(_y) {}
    sensor(std::pair<double, double> point) : x(point.first), y(point.second) {}
} sensor;

class HarmonySearch {
    private:
        std::vector<int> num_sensors;
        std::mt19937 rng;
        std::uniform_real_distribution<> dist_0_1;
        
        void init_rng();
        double random();
        std::pair<double, double> random_point();

    public:
        std::vector<std::vector<sensor>> HM;
        int W, H, HM_size, max_sensors, min_sensors;
        double sensor_radius, sensor_uncertainty_radius;
        double x_lower, x_upper, y_lower, y_upper;

        HarmonySearch(int W, int H, int HM_size, double sensor_radius, double sensor_uncertainty_radius);

        void init_harmony_memory();
        void cout_harmony_memory();
};

#endif
