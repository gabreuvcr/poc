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
    int a, b;
    do {
        a = Random::random_value(0, config.pop_size - 1);
    } while (a == i);
    do {
        b = Random::random_value(0, config.pop_size - 1);
    } while (b == i || b == a);
    
    if (population[a].is_dominant(population[b])) {
        return population[a];
    } else if (population[b].is_dominant(population[a])) {
        return population[b];
    } else if (Random::random() <= Constants::FLIP_COIN){
        return population[a];
    } else {
        return population[b];
    }
}

int count_num_active_sensors(std::vector<bool> active_sensors) {
    int num_active_sensors = 0;
    for (int i = 0; i < active_sensors.size(); i++) {
        if (active_sensors[i]) {
            num_active_sensors++;
        }
    }
    return num_active_sensors;
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
    if (count_num_active_sensors(child) < this->min_sensores) {
        return win_active_sensors;
    }

    return child;
}

std::vector<bool> DifferentialEvolution::mutation_active_sensors(std::vector<bool> active_sensors) {
    if (Random::random() <= Constants::MR_C) {
        std::vector<bool> mutated_active_sensors(active_sensors);

        int i = 0;
        do {
            int random_sensor = Random::random_value(0, this->max_sensores - 1);
            mutated_active_sensors[random_sensor] = !active_sensors[random_sensor];
            i++;
        } while (i < 2 || count_num_active_sensors(mutated_active_sensors) < this->min_sensores);

        return mutated_active_sensors;
    }

    return active_sensors;
}

std::vector<Agent> DifferentialEvolution::calculate_crowding_distance(std::vector<Agent>& pop) {
    const int n = pop.size();
    for (Agent& a : pop) {
        a.distance = 0;
    }

    //number of sensors
    std::sort(pop.begin(), pop.end(), [](Agent& a, Agent& b) {
            return a.num_active_sensors < b.num_active_sensors;
        }
    );
    double min = pop.front().num_active_sensors;
    double max = pop.back().num_active_sensors;
    pop[0].distance = std::numeric_limits<double>::infinity();
    pop[n - 1].distance = std::numeric_limits<double>::infinity();
    
    for (int i = 1; i < n - 1; i++) {
        pop[i].distance += 
            (pop[i + 1].num_active_sensors - pop[i - 1].num_active_sensors) / (max - min);
    }

    //coverage ratio
    std::sort(pop.begin(), pop.end(), [](Agent& a, Agent& b) {
            return a.coverage_ratio < b.coverage_ratio;
        }
    );
    min = pop.front().coverage_ratio;
    max = pop.back().coverage_ratio;
    pop[0].distance = std::numeric_limits<double>::infinity();
    pop[n - 1].distance = std::numeric_limits<double>::infinity();
    
    for (int i = 1; i < n - 1; i++) {
        pop[i].distance += 
            (pop[i + 1].coverage_ratio - pop[i - 1].coverage_ratio) / (max - min);
    }

    return pop;
}

bool agent_doesnt_dominate_anyone(int agent_index, std::vector<Agent>& new_pop) {
    for (int k = 0; k < new_pop.size(); k++) {
        if (agent_index == k) continue;

        if (new_pop[agent_index].is_dominant(new_pop[k])) {
            return false;
        }
    }

    return true;
}

bool agent_with_less_crowding_distance(int agent_index, std::vector<Agent>& new_pop) {
    for (int k = 0; k < new_pop.size(); k++) {
        if (agent_index == k) continue;

        if (new_pop[k].is_dominant(new_pop[agent_index])) continue;

        if (new_pop[agent_index].distance > new_pop[k].distance) {
            return false;
        }
    }

    return true;
}

bool agent_is_not_dominated(int agent_index, std::vector<Agent>& pop) {
    for (int k = 0; k < pop.size(); k++) {
        if (agent_index == k) continue;

        if (pop[k].is_dominant(pop[agent_index])) {
            return false;
        }
    }
    return true;
}

std::vector<Agent> DifferentialEvolution::get_pareto_front() {
    std::vector<Agent> pareto_front;
    for (int j = 0; j < this->population.size(); j++) {
        if (agent_is_not_dominated(j, this->population)) {
            pareto_front.push_back(this->population[j]);
        }
    }
    return pareto_front;
}

std::vector<Agent> DifferentialEvolution::get_pareto_front(std::vector<Agent>& population) {
    std::vector<Agent> pareto_front;
    for (int j = 0; j < population.size(); j++) {
        if (agent_is_not_dominated(j, population)) {
            pareto_front.push_back(population[j]);
        }
    }
    return pareto_front;
}

std::vector<Agent> DifferentialEvolution::run() {
    this->init_first_population();
    int generation_count = 0;
    std::vector<Agent> new_pop = std::vector<Agent>(config.pop_size);
    int m = 0;
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

            if (trial_agent.is_weak_dominat(population[i])) {
                new_pop[i] = trial_agent;
            }
            else {
                new_pop[i] = population[i];
                if (!population[i].is_dominant(trial_agent)) {
                    new_pop.push_back(trial_agent);
                    m++;
                }
            }
        }

        this->calculate_crowding_distance(new_pop);

        while (m > 0) {
            int selected_index;

            for (int j = 0; j < new_pop.size(); j++) {
                if (agent_doesnt_dominate_anyone(j, new_pop) &&
                    agent_with_less_crowding_distance(j, new_pop)) {
                    selected_index = j;
                }
            }
            new_pop.erase(new_pop.begin() + selected_index);
            m--;
        }

        population = new_pop;
        generation_count++;
    }

    return this->population;
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
