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
        std::vector<double> coverage_ratios;
        std::vector<double> objectives;

        double calculate_objective(std::vector<Sensor> sensors, int n_sensors, double c_ratio);

    public:
        HarmonySearchConfig config;
        std::vector<std::vector<Sensor>> HM;
        int max_sensors, min_sensors;
        int best_sensors_index, worst_sensors_index;

        HarmonySearch(HarmonySearchConfig config);

        void starts();
        void init_harmony_memory();
        void cout_harmony_memory();
        double calculate_coverage_ratio(std::vector<Sensor> sensors, std::vector<PointOfInterest> pois);
        void update_best_and_worst_index();
};

#endif
