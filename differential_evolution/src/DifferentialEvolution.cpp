#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>
#include "DifferentialEvolution.hpp"
#include "Random.hpp"
#include "Constants.hpp"

DifferentialEvolution::DifferentialEvolution(DifferentialEvolutionConfig config, std::vector<PointOfInterest> pois) {
    this->config = config;
    this->pois = pois;
    this->min_sensores = ceil(
        (config.W / (2 * Sensor::max_radius)) * (config.H / (2 * Sensor::max_radius))
    );
    this->max_sensores = ceil(
        (config.W / (2 * Sensor::min_radius)) * (config.H / (2 * Sensor::min_radius))
    );
    this->population = std::vector<Agent>(config.pop_size);
}

std::vector<Sensor> DifferentialEvolution::mutation_sensors(int i) {
    //choose 3 random agents
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
    for (int j = 0; j < this->max_sensores; j++) {
        //execute mutation: v_i = x_a + F(x_b - x_c)
        double x_a = population[a].sensors[j].x;
        double x_b = population[b].sensors[j].x;
        double x_c = population[c].sensors[j].x;
        double y_a = population[a].sensors[j].y;
        double y_b = population[b].sensors[j].y;
        double y_c = population[c].sensors[j].y;

        double x = x_a + Constants::F * (x_b - x_c);
        double y = y_a + Constants::F * (y_b - y_c);
        
        //check if x and y are inside the grid
        if (x < 0) x = fabs(x);
        if (x > config.W) x = config.W - (x - config.W);
        if (y < 0) y = fabs(y);
        if (y > config.H) y = config.H - (y - config.H);

        mutated_sensors[j] = Sensor(x, y);
    }
    return mutated_sensors;
}

std::vector<Sensor> DifferentialEvolution::crossover_sensors(std::vector<Sensor> agent_sensors, std::vector<Sensor> mutated_sensors) {
    int j_rand = Random::random_value(0, this->max_sensores - 1);

    std::vector<Sensor> crossed_sensors(this->max_sensores);

    for (int j = 0; j < this->max_sensores; j++) {
        if (Random::random() <= Constants::CR_DE || j == j_rand) {
            crossed_sensors[j] = mutated_sensors[j];
        } else {
            crossed_sensors[j] = agent_sensors[j];
        }
    }

    return crossed_sensors;
}

Agent DifferentialEvolution::tournament_selection(int i) {
    int a;
    do {
        a = Random::random_value(0, config.pop_size - 1);
    } while (a == i);
    
    return population[a];
}

std::vector<bool> DifferentialEvolution::crossover_active_sensors(
        std::vector<bool> agent_active_sensors, 
        std::vector<bool> win_active_sensors
    ) {
    std::vector<bool> child = std::vector<bool>(this->max_sensores);
    for (int i = 0; i < this->max_sensores; i++) {
        if (Random::random() <= Constants::FLIP_COIN) {
            child[i] = agent_active_sensors[i];
        } else {
            child[i] = win_active_sensors[i];
        }
    }
    return child;
}

std::vector<bool> DifferentialEvolution::mutation_active_sensors(std::vector<bool> active_sensors) {
    if (Random::random() <= Constants::MR_C) {
        std::vector<bool> mutated_active_sensors(active_sensors);

        int random_sensor = Random::random_value(0, this->max_sensores - 1);
        mutated_active_sensors[random_sensor] = !active_sensors[random_sensor];

        return mutated_active_sensors;
    }

    return active_sensors;
}

void DifferentialEvolution::run() {
    this->init_first_population();

    int generation_count = 0;

    std::vector<Agent> new_pop = std::vector<Agent>(config.pop_size);

    for (int i = 0; i < config.pop_size; i++) {
        std::cout << population[i].num_active_sensors << "," << population[i].coverage_ratio << ";";
    }
    std::cout << std::endl;
    
    while (generation_count < config.num_generation) {
        for (int i = 0; i < config.pop_size; i++) {
            Agent agent = population[i];
            Agent trial_agent;
            if (Random::random() <= Constants::FLIP_COIN) {
                std::vector<Sensor> mutated_sensors = mutation_sensors(i);
                std::vector<Sensor> crossed_sensors = crossover_sensors(agent.sensors, mutated_sensors);
                trial_agent = Agent(crossed_sensors, agent.active_sensors);
            } else {
                Agent win = tournament_selection(i);
                std::vector<bool> crossed_active_sensors = crossover_active_sensors(agent.active_sensors, win.active_sensors);
                std::vector<bool> mutated_active_sensors = mutation_active_sensors(crossed_active_sensors);
                trial_agent = Agent(agent.sensors, mutated_active_sensors);
            }
            trial_agent.calculate_num_active_sensors();
            trial_agent.calculate_coverage_ratio(pois);

            if (trial_agent.dominates(population[i])) {
                new_pop[i] = trial_agent;
            }
            else {
                new_pop[i] = population[i];
            }

            generation_count++;
        }
    } 
    population = new_pop;
    for (int i = 0; i < config.pop_size; i++) {
        std::cout << population[i].num_active_sensors << "," << population[i].coverage_ratio << ";";
    }
    std::cout << std::endl;
}

void DifferentialEvolution::init_first_population() {
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
        population[i].calculate_coverage_ratio(this->pois);
    }
}
