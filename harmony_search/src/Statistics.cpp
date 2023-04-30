#include <chrono>
#include "Statistics.hpp"
#include "HarmonySearch.hpp"
#include "Utils.hpp"
#include "Constants.hpp" 

namespace Statistics {
    #define RUNS 10

    void run_all_tests(HarmonySearchConfig config, std::vector<PointOfInterest> pois) {
        HarmonySearch hs = HarmonySearch(config, pois);
        
        int max_sensors = hs.calculated_max_sensors;
        int min_sensors = hs.calculated_min_sensors;
        bool reached_full_coverage = false;
        int last_number_of_sensors = max_sensors;
        std::vector<double> coverage_average(max_sensors + 1);
        std::vector<double> coverage_max(max_sensors + 1, std::numeric_limits<double>::min());
        std::vector<double> coverage_min(max_sensors + 1, std::numeric_limits<double>::max());
        std::chrono::duration<double> average_total_time { 0 };

        for (int i = min_sensors; i <= max_sensors; i++) { 
            hs.set_num_fixed_sensors(i);
            double coverage_sum = 0;
            
            auto start_time = std::chrono::high_resolution_clock::now();
            for (int run = 0; run < RUNS; run++) {
                double curr_coverage = hs.run();

                coverage_sum += curr_coverage;
                coverage_max[i] = std::max(coverage_max[i], curr_coverage);
                coverage_min[i] = std::min(coverage_min[i], curr_coverage);
                
                if (hs.coverage_ratios[hs.i_best] == 1.0) {
                    reached_full_coverage = true;
                    last_number_of_sensors = i;
                }
            }
            auto end_time = std::chrono::high_resolution_clock::now();

            average_total_time += (end_time - start_time) / RUNS;
            coverage_average[i] = coverage_sum / RUNS;
            if (reached_full_coverage) break;
        }

        Utils::log_population(
            min_sensors, 
            last_number_of_sensors,
            coverage_min,
            coverage_max, 
            coverage_average 
        );
        Utils::log_time(average_total_time);
        Utils::log_seed(Constants::SEED);
    }
}
