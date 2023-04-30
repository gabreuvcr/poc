#include <chrono>
#include <vector>
#include "Experiment.hpp"
#include "HarmonySearch.hpp"
#include "Utils.hpp"
#include "Constants.hpp" 

namespace Experiment {
    #define RUNS 10

    std::vector<double> calculate_average_coverage(
        std::vector<std::vector<double>> coverages,
        int min_sensors,
        int last_number_of_sensors
    ) {
        std::vector<double> average_coverage(last_number_of_sensors + 1);

        for (int i = min_sensors; i <= last_number_of_sensors; i++) {
            double sum = 0;
            for (int run = 0; run < RUNS; run++) {
                sum += coverages[run][i];
            }
            average_coverage[i] = sum / RUNS;
        }
        return average_coverage;
    }

    std::vector<double> calculate_std_coverage(
        std::vector<std::vector<double>> coverages,
        std::vector<double> average_coverage,
        int min_sensors,
        int last_number_of_sensors
    ) {
        std::vector<double> std_coverage(last_number_of_sensors + 1);

        for (int i = min_sensors; i <= last_number_of_sensors; i++) {
            double sum = 0;
            for (int run = 0; run < RUNS; run++) {
                double diff = coverages[run][i] - average_coverage[i];
                sum += diff * diff;
            }
            std_coverage[i] = sqrt(sum / RUNS);
        }
        return std_coverage;
    }

    void run_test(HarmonySearchConfig config, std::vector<PointOfInterest> pois) {
        HarmonySearch hs = HarmonySearch(config, pois);
        
        int max_sensors = hs.calculated_max_sensors;
        int min_sensors = hs.calculated_min_sensors;
        int last_number_of_sensors = max_sensors;
        std::chrono::duration<double> total_time { 0 };
        std::vector<std::vector<double>> coverages(
            RUNS, 
            std::vector<double>(max_sensors + 1)
        );

        for (int run = 0; run < RUNS; run++) {
            auto start_time = std::chrono::high_resolution_clock::now();
            for (int i = min_sensors; i <= last_number_of_sensors; i++) {
                HarmonySearch hs = HarmonySearch(config, pois, i);
                double curr_coverage = hs.run();
                coverages[run][i] = curr_coverage;
                if (curr_coverage == 1.0) {
                    last_number_of_sensors = i;
                }
            }
            auto end_time = std::chrono::high_resolution_clock::now();
            total_time += (end_time - start_time);
        }
        auto average_total_time = total_time / RUNS;


        std::vector<double> average_coverage = calculate_average_coverage(
            coverages,
            min_sensors,
            last_number_of_sensors
        );
        std::vector<double> std_coverage = calculate_std_coverage(
            coverages,
            average_coverage,
            min_sensors,
            last_number_of_sensors
        );

        for (int run = 0; run < RUNS; run++) {
            Utils::log_population(min_sensors, last_number_of_sensors, coverages[run]);
        }
        Utils::log_population(
            min_sensors, 
            last_number_of_sensors,
            average_coverage
        );
        Utils::log_population(
            min_sensors, 
            last_number_of_sensors,
            std_coverage
        );
        Utils::log_time(average_total_time);
        Utils::log_seed(Constants::SEED);
    }
}
