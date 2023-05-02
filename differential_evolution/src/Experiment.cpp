#include <chrono>
#include <algorithm>
#include "Experiment.hpp"
#include "DifferentialEvolution.hpp"
#include "Utils.hpp"
#include "Constants.hpp"

namespace Experiment {
    #define RUNS 10

    void remove_duplicate_from_pareto_front(std:: vector<Agent>& pareto_front) {
        sort(pareto_front.begin(), pareto_front.end(), [](Agent& a, Agent& b) {
            return a.num_active_sensors < b.num_active_sensors
                || (a.num_active_sensors == b.num_active_sensors 
                    && a.coverage_ratio < b.coverage_ratio);
            }
        );
        pareto_front.erase(
            std::unique(pareto_front.begin(), pareto_front.end()), 
            pareto_front.end()
        );
    }

    double calculate_hyper_volume(
        std::vector<Agent> pareto_front,
        int min_sensores,
        int max_sensores
    ) {
        double hyper_volume = 0.0;
        int last_numbers_of_sensors = pareto_front.back().num_active_sensors;

        for (int i = 1; i < pareto_front.size(); i++) {
            int x_b = pareto_front[i].num_active_sensors;
            int x_a = pareto_front[i - 1].num_active_sensors;
            double y_b = pareto_front[i].coverage_ratio;
            double y_a = pareto_front[i - 1].coverage_ratio;

            double dx = x_b - x_a;
            double dy = y_b - y_a;
            hyper_volume += (dx * dy * 0.5) + (dx * y_a);
        }
        for (int x_b = last_numbers_of_sensors + 1; x_b <= max_sensores; x_b++) {
            hyper_volume += 1;
        }

        return hyper_volume;
    }

    void run_test(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois) {
        DifferentialEvolution de = DifferentialEvolution(config, pois);

        auto start_time = std::chrono::high_resolution_clock::now();

        de.run();
    
        auto end_time = std::chrono::high_resolution_clock::now();

        std::vector<Agent> pareto_front = de.get_pareto_front();
        remove_duplicate_from_pareto_front(pareto_front);
            
        auto total_time = (end_time - start_time);
        
        double hyper_volume = calculate_hyper_volume(
            pareto_front,
            de.min_sensores,
            de.max_sensores
        );

        Utils::log_population(pareto_front);
        Utils::log_hyper_volume(hyper_volume);
        Utils::log_time(total_time);
        Utils::log_seed(Constants::SEED);
    }

    void run_test_super_pareto_front(
        DifferentialEvolutionConfig config, 
        std::vector<PointOfInterest> pois
    ) {
        DifferentialEvolution de = DifferentialEvolution(config, pois);

        std::vector<std::vector<Agent>> pareto_fronts(RUNS);
        std::chrono::duration<double> average_total_time { 0 };

        auto start_time = std::chrono::high_resolution_clock::now();
        for (int runs = 0; runs < RUNS; runs++) {
            de.run();
            pareto_fronts[runs] = de.get_pareto_front();
        }
        auto end_time = std::chrono::high_resolution_clock::now();
            
        average_total_time += (end_time - start_time) / RUNS;
        
        for (int i = 0; i < pareto_fronts.size(); i++) {
            remove_duplicate_from_pareto_front(pareto_fronts[i]);
        }

        std::vector<Agent> pareto_fronts_merged;
        for (int i = 0; i < pareto_fronts.size(); i++) {
            Utils::log_population(pareto_fronts[i]);

            pareto_fronts_merged.insert(
                pareto_fronts_merged.end(), 
                pareto_fronts[i].begin(), 
                pareto_fronts[i].end()
            );
        }

        std::vector<Agent> pareto_front_final = de.get_pareto_front(pareto_fronts_merged);
        remove_duplicate_from_pareto_front(pareto_front_final);

        Utils::log_population(pareto_front_final);
        Utils::log_time(average_total_time);
        Utils::log_seed(Constants::SEED);
    }
}
