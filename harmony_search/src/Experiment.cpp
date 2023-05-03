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

    double calculate_hyper_volume(
        std::vector<double> average_coverage,
        int min_sensors,
        int max_sensors,
        int last_number_of_sensors
    ) {
        double hyper_volume = 0;
        double last_average_coverage = average_coverage[last_number_of_sensors];

        for (int x_b = min_sensors + 1; x_b <= max_sensors; x_b++) {
            if (x_b > last_number_of_sensors) {
                hyper_volume += last_average_coverage;
            } else {
                int x_a = x_b - 1;
                double y_a = average_coverage[x_a];
                double y_b = average_coverage[x_b];

                double dx = x_b - x_a;
                double dy = abs(y_b - y_a);

                hyper_volume += (dx * dy * 0.5) + (dx * y_a);
            }
        }

        return hyper_volume;
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

        double hyper_volume = calculate_hyper_volume(
            average_coverage,
            min_sensors,
            max_sensors,
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
        Utils::log_hyper_volume(hyper_volume);
        Utils::log_time(average_total_time);
        Utils::log_seed(Constants::SEED);
    }
}
