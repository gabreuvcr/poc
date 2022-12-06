#ifndef HARMONY_SEARCH_H
#define HARMONY_SEARCH_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"
#include "Sensor.hpp"

class HarmonySearch {
    private:
        std::vector<int> num_sensors;
        std::vector<double> coverage_ratios;
        std::vector<double> objectives;
        bool test = false;

        void init_random_harmony_memory();
        Sensor memory_consideration(int s);
        Sensor pitch_adjustment(Sensor new_sensor);
        Sensor random_consideration();
        double calculate_objective(std::vector<Sensor> sensors, int n_sensors, double c_ratio);
        double calculate_coverage_ratio(std::vector<Sensor> sensors);
        void update_best_and_worst_index();

    public:
        HarmonySearchConfig config;
        std::vector<PointOfInterest> pois;
        std::vector<std::vector<Sensor>> HM;
        int max_sensors, min_sensors;
        int calculate_max_sensors, calculate_min_sensors;
        int i_best, i_worst;

        HarmonySearch() {};
        HarmonySearch(HarmonySearchConfig config, std::vector<PointOfInterest> pois);
        HarmonySearch(HarmonySearchConfig config, std::vector<PointOfInterest> pois, int num_fixed_sensors);

        double run();
        void cout_harmony_memory(int interation);
        void set_test(bool test);
        void set_num_fixed_sensors(int num_sensors);
};

#endif
