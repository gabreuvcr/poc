#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>
#include <chrono>
#include "DifferentialEvolution.hpp"
#include "PointOfInterest.hpp"
#include "Utils.hpp"
#include "Sensor.hpp"
#include "Agent.hpp"
#include "Constants.hpp"

#define RUNS 10

void remove_duplicate_from_pareto_front(std:: vector<Agent>& pareto_front) {
    sort(pareto_front.begin(), pareto_front.end(), [](Agent& a, Agent& b) {
        return a.num_active_sensors < b.num_active_sensors
            || (a.num_active_sensors == b.num_active_sensors 
                && a.coverage_ratio < b.coverage_ratio);
        }
    );
    pareto_front.erase(std::unique(pareto_front.begin(), pareto_front.end()), pareto_front.end());
}

void run_all(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois) {
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

int main(int argc, char* argv[]) {
    std::string filename;
    bool all = false;

    Utils::check_arguments(argv, argc, filename, all);

    std::ifstream file = Utils::open_file(filename);

    DifferentialEvolutionConfig config = Utils::read_differential_evolution_config(file);

    double radius = Utils::read_sensor_radius(file);
    double radius_err = radius * 0.5;

    std::vector<PointOfInterest> pois = Utils::read_pois(file, config.W, config.H);

    Sensor::set_values(radius, radius_err, config.W, config.H);

    if (all) {
        run_all(config, pois);
    } else {
        DifferentialEvolution de = DifferentialEvolution(config, pois);
        de.run();
        std::vector<Agent> pareto_front = de.get_pareto_front();
        Utils::log_population(pareto_front);
    }
    
    return 0;
}
