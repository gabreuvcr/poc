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
        double calculate_coverage_ratio(std::vector<Sensor> sensors);
        Sensor memory_consideration(int s);
        Sensor pitch_adjustment(Sensor new_sensor);
        Sensor random_consideration();

        void update_best_and_worst_index();
        void update_best_and_worst_index(int i);

    public:
        HarmonySearchConfig config;
        std::vector<std::vector<Sensor>> HM;
        int max_sensors, min_sensors;
        int i_best, i_worst;

        HarmonySearch(HarmonySearchConfig config);

        void init_harmony_search();
        void init_random_harmony_memory();
        void cout_harmony_memory(int interation);
};

#endif
