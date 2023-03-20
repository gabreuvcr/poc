#include <iostream>
#include <vector>
#include <string.h>
#include <chrono>
#include "HarmonySearch.hpp"
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"
#include "Utils.hpp"
#include "Sensor.hpp"

#define RUNS 10

void run_all(HarmonySearchConfig config, std::vector<PointOfInterest> pois) {
    HarmonySearch hs = HarmonySearch(config, pois);
    int max_sensors = hs.calculate_max_sensors;  
    int min_sensors = hs.calculate_min_sensors;
    bool reached_full_coverage = false;
    int num_sensors_full_coverage = max_sensors;
    std::chrono::nanoseconds total_time{ 0 };
    
    hs.all = true;
    std::vector<double> coverage_average(max_sensors + 1);
    std::vector<double> coverage_max(max_sensors + 1, std::numeric_limits<double>::min());
    std::vector<double> coverage_min(max_sensors + 1, std::numeric_limits<double>::max());

    for (int i = min_sensors; i < max_sensors + 1; i++) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        double coverage_sum = 0;
        for (int run = 0; run < RUNS; run++) {
            hs.set_num_fixed_sensors(i);
            double curr_coverage = hs.run();

            coverage_sum += curr_coverage;
            coverage_max[i] = std::max(coverage_max[i], curr_coverage);
            coverage_min[i] = std::min(coverage_min[i], curr_coverage);
            
            if (hs.coverage_ratios[hs.i_best] == 1.0) {
                reached_full_coverage = true;
                num_sensors_full_coverage = i;
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();

        total_time += (end_time - start_time) / RUNS;
        coverage_average[i] = coverage_sum / RUNS;
        if (reached_full_coverage) break;
    }

    for (int i = min_sensors; i < num_sensors_full_coverage + 1; i++) {
        std::cout << i << "," << coverage_average[i] << ";";;
    }
    std::cout << std::endl;
    for (int i = min_sensors; i < num_sensors_full_coverage + 1; i++) {
        std::cout << i << "(" << coverage_min[i] << "|" << coverage_max[i] << ");";;
    }

    std::cout << std::endl;
    std::cout << std::chrono::duration_cast<std::chrono::seconds>(total_time).count() << " s" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename;
    bool fixed_sensors = false; int num_fixed_sensors = 0;

    Utils::check_arguments(argv, argc, filename, fixed_sensors, num_fixed_sensors);

    std::ifstream file = Utils::open_file(filename);

    HarmonySearchConfig config = Utils::read_harmony_config(file);

    double radius = Utils::read_sensor_radius(file);
    double radius_err = radius * 0.5;

    std::vector<PointOfInterest> pois = Utils::read_pois(file, config.W, config.H);

    Sensor::set_values(radius, radius_err, config.W, config.H);

    if (fixed_sensors) {
        HarmonySearch hs = HarmonySearch(config, pois, num_fixed_sensors);
        hs.run();
    } else {
        run_all(config, pois);
    }

    return 0;
}
