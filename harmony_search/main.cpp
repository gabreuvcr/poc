#include <iostream>
#include <vector>
#include <string.h>
#include "HarmonySearch.hpp"
#include "HarmonySearchConfig.hpp"
#include "PointOfInterest.hpp"
#include "Utils.hpp"
#include "Sensor.hpp"

void run_test(HarmonySearchConfig config, std::vector<PointOfInterest> pois, bool fixed_sensors, int num_fixed_sensors) {
    int runs = 30;

    std::vector<double> coverage_ratios(runs);
    double best_coverage_ratio = 0;

    HarmonySearch hs;
    for (int i = 0; i < runs; i++) {
        if (fixed_sensors) {
            hs = HarmonySearch(config, pois, num_fixed_sensors);
        } else {
            hs = HarmonySearch(config, pois);
        }
        hs.set_test(true);
        coverage_ratios[i] = hs.run();
        best_coverage_ratio = std::max(best_coverage_ratio, coverage_ratios[i]);
        std::cout << "Run " << i << ": " << coverage_ratios[i] << std::endl;
    }

    double sum = std::accumulate(coverage_ratios.begin(), coverage_ratios.end(), 0.0);
    double average = sum / runs;

    double accum = 0.0;
    std::for_each(coverage_ratios.begin(), coverage_ratios.end(), [&](const double d) {
        accum += (d - average) * (d - average);
    });
    double standard_deviation = sqrt(accum / runs);

    std::cout << "Best coverage ratio: " << best_coverage_ratio << std::endl;
    std::cout << "Average: " << average << std::endl;
    std::cout << "Standard deviation: " << standard_deviation << std::endl;
}

int main(int argc, char* argv[]) {
    std::string filename;
    bool fixed_sensors = false; int num_fixed_sensors = 0;
    bool run_tests = false;

    Utils::check_arguments(argv, argc, filename, fixed_sensors, num_fixed_sensors, run_tests);

    std::ifstream file = Utils::open_file(filename);

    HarmonySearchConfig config = Utils::read_harmony_config(file);

    double radius = Utils::read_sensor_radius(file);
    double radius_err = radius * 0.5;

    std::vector<PointOfInterest> pois = Utils::read_pois(file, config.W, config.H);

    Sensor::set_values(radius, radius_err, config.W, config.H);

    if (run_tests) {
        run_test(config, pois, fixed_sensors, num_fixed_sensors);
    } else if (fixed_sensors) {
        HarmonySearch hs = HarmonySearch(config, pois, num_fixed_sensors);
        hs.run();
    } else {
        HarmonySearch hs = HarmonySearch(config, pois);
        hs.run();
    }

    return 0;
}
