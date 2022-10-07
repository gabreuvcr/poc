#include <cmath>
#include <algorithm>
#include "DifferentialEvolution.hpp"
#include "Random.hpp"

DifferentialEvolution::DifferentialEvolution(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois) {
    std::cout << "DifferentialEvolution constructor" << std::endl;

    this->config = config;
    this->pois = pois;
    std::cout << config.H << " " << config.W << std::endl; 
    std::cout << Sensor::max_radius << " " << Sensor::min_radius << std::endl; 

    this->min_sensores = ceil(
        (config.W / (2 * Sensor::max_radius)) * (config.H / (2 * Sensor::max_radius))
    );
    this->max_sensores = ceil(
        (config.W / (2 * Sensor::min_radius)) * (config.H / (2 * Sensor::min_radius))
    );
    
    this->population = std::vector<Agent>(config.pop_size);
}

void DifferentialEvolution::run() {
    this->init_first_population();
}

void DifferentialEvolution::update_dominat_sensors() {
    std::sort(population.begin(),
        population.end(),
        [](const Agent& a, const Agent& b) {
            return a.num_active_sensors < b.num_active_sensors;
        });
    int current = 0;
    for (int i = 1; i < population.size(); i++) {
        if (population[i].num_active_sensors == population[current].num_active_sensors 
            && population[i].coverage_ratio > population[current].coverage_ratio) {
                current = i;
        } else if (population[i].num_active_sensors == population[current].num_active_sensors
            && population[i].coverage_ratio <= population[current].coverage_ratio) {
                continue;
        } else {
            population[current].is_dominant = true;
            current = i;
        }
    }
    population[current].is_dominant = true;
}

void DifferentialEvolution::init_first_population() {
    std::cout << "init_first_population" << std::endl;

    for (int i = 0; i < population.size(); i++) {
        std::vector<Sensor> new_sensors(max_sensores);

        for (int s = 0; s < max_sensores; s++) {
            new_sensors[s] = Sensor::random_sensor();
        }

        std::vector<bool> active_sensors(max_sensores, false);
        int num_active_sensors = Random::random_value(min_sensores, max_sensores);

        for (int j = 0; j < num_active_sensors; j++) {
            active_sensors[j] = true;
        }

        std::shuffle(begin(active_sensors), end(active_sensors), Random::rng);

        population[i] = Agent(new_sensors, active_sensors);
        population[i].set_num_active_sensors(num_active_sensors);
        population[i].calculate_coverage_ratio(this->pois);

        // std::cout << population[i].num_active_sensors << "," << population[i].coverage_ratio << ";";
        // Sensor::cout_sensors(population[i].sensors, population[i].active_sensors);
        // std::cout << std::endl << std::endl;
    }
    std::cout << population.size() << std::endl;
    update_dominat_sensors();
    for (int i = 0; i < population.size(); i++) {
        std::cout << population[i].num_active_sensors << "," << population[i].coverage_ratio << ",";
        std::cout << population[i].is_dominant << ";";
    }
}
