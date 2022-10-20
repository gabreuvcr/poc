#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>
#include "DifferentialEvolution.hpp"
#include "Random.hpp"
#include "Constants.hpp"

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

void DifferentialEvolution::run2() {
    this->init_first_population();
}

std::vector<Sensor> DifferentialEvolution::mutation_sensors(int i) {
    //escolhe 3 agentes aleatorios
    int a, b, c;
    do {
        a = Random::random_value(0, config.pop_size - 1);
    } while (a == i);
    do {
        b = Random::random_value(0, config.pop_size - 1);
    } while (b == i || b == a);
    do {
        c = Random::random_value(0, config.pop_size - 1);
    } while (c == i || c == a || c == b);

    std::vector<Sensor> mutated_sensors(this->max_sensores);
    //realiza mutação: v_i = x_a + F(x_b - x_c)
    for (int j = 0; j < this->max_sensores; j++) {
        double x_a = population[a].sensors[j].x;
        double x_b = population[b].sensors[j].x;
        double x_c = population[c].sensors[j].x;
        double y_a = population[a].sensors[j].y;
        double y_b = population[b].sensors[j].y;
        double y_c = population[c].sensors[j].y;

        double x = x_a + Constants::F * (x_b - x_c);
        double y = y_a + Constants::F * (y_b - y_c);

        mutated_sensors[j] = Sensor(x, y);
        // std::cout << "x_old: " << x_a << " y_old: " << y_a << std::endl;
        // std::cout << "x: " << x << " y: " << y << std::endl;
    }
    return mutated_sensors;
}

std::vector<Sensor> DifferentialEvolution::crossover_sensors(int i, std::vector<Sensor> mutated_sensors) {
    int j_rand = Random::random_value(0, this->max_sensores - 1);

    std::vector<Sensor> crossed_sensors(this->max_sensores);

    for (int j = 0; j < this->max_sensores; j++) {
        if (Random::random() <= Constants::CR || j == j_rand) {
            crossed_sensors[j] = mutated_sensors[j];
        } else {
            crossed_sensors[j] = population[i].sensors[j];
        }
    }

    return crossed_sensors;
}

void DifferentialEvolution::run() {
    this->init_first_population();

    int generation_count = 0;

    std::vector<std::vector<double>> mutation(max_sensores, std::vector<double>(2));
    std::cout << config.num_generation << std::endl;
    while (generation_count < 1) {
    // while (generation_count < config.num_gerations) {
        for (int i = 0; i < 1; i++) {
        // for (int i = 0; i < config.pop_size; i++) {
            std::vector<Sensor> mutated_sensors = this->mutation_sensors(i);

            //realiza crossover
            // u_ij = v_ij, se rand() <= CR ou j == j_rand
            // u_ij = x_ij, caso contrário
            std::vector<Sensor> trial = crossover_sensors(i, mutated_sensors);

            Sensor::cout_sensors(population[i].sensors);
            std::cout << population[i].coverage_ratio << std::endl;

            std::cout << std::endl;

            Agent agent_trial = Agent(trial, population[i].active_sensors);
            agent_trial.calculate_coverage_ratio(this->pois);
            Sensor::cout_sensors(trial);
            std::cout << agent_trial.coverage_ratio << std::endl;

            //se trial domina x_i, então x_i = trial
            //se não, então x_i = x_i

            generation_count++;
        }
    } 
}

// void DifferentialEvolution::update_dominat_sensors() {
//     std::sort(population.begin(), population.end(),
//         [](const Agent& a, const Agent& b) {
//             return a.num_active_sensors < b.num_active_sensors;
//         }
//     );

//     int current = 0;
//     for (int i = 1; i < population.size(); i++) {
//         if (population[i].num_active_sensors == population[current].num_active_sensors 
//             && population[i].coverage_ratio > population[current].coverage_ratio) {
//                 current = i;
//         } else if (population[i].num_active_sensors == population[current].num_active_sensors
//             && population[i].coverage_ratio <= population[current].coverage_ratio) {
//                 continue;
//         } else {
//             population[current].is_dominant = true;
//             current = i;
//         }
//     }
//     population[current].is_dominant = true;
// }

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

    }
    std::cout << population.size() << std::endl;
    // update_dominat_sensors();
    for (int i = 0; i < population.size(); i++) {
        std::cout << population[i].num_active_sensors << "," << population[i].coverage_ratio << ",";
        std::cout << population[i].is_dominant << ";  ";
    }
    std::cout << std::endl;
}
