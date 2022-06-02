#ifndef HARMONY_SEARCH_H
#define HARMONY_SEARCH_H

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "Sensor.hpp"
#include "PointOfInterest.hpp"
#include "Random.hpp"

class HarmonySearch {
    private:
        std::vector<int> num_sensors;
        // Random random = Random();

    public:
        std::vector<std::vector<Sensor>> HM;
        std::vector<PointOfInterest> POIs;
        int W, H, HM_size, max_sensors, min_sensors;
        double sensor_radius, sensor_uncertainty_radius;
        double x_lower, x_upper, y_lower, y_upper;

        HarmonySearch(
            int W,
            int H,
            int HM_size,
            double sensor_radius,
            double sensor_uncertainty_radius, 
            std::vector<PointOfInterest> POIs
        );

        void init_harmony_memory();
        void cout_harmony_memory();
};

#endif
