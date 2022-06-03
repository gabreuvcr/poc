#ifndef HARMONY_SEARCH_H
#define HARMONY_SEARCH_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "HarmonySearchConfig.hpp"
#include "Sensor.hpp"

class HarmonySearch {
    private:
        std::vector<int> num_sensors;
        std::vector<int> num_k_coverage;
        std::vector<double> objectives;

        double objective(int sensor_index);
    public:
        std::vector<std::vector<Sensor>> HM;
        HarmonySearchConfig config;
        int max_sensors, min_sensors;
        double x_lower, x_upper, y_lower, y_upper;

        HarmonySearch(HarmonySearchConfig config);

        void init_harmony_memory();
        void cout_harmony_memory();
};

#endif
