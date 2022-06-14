#include <cmath>

#include "HarmonySearch.hpp"
#include "Constants.hpp"
#include "Random.hpp"

HarmonySearch::HarmonySearch(HarmonySearchConfig config) {
    this->config = config;

    this->min_sensors = ceil(
        (config.W / (2 * Sensor::max_radius)) * (config.H / (2 * Sensor::max_radius))
    );
    this->max_sensors = ceil(
        (config.W / (2 * Sensor::min_radius)) * (config.H / (2 * Sensor::min_radius))
    );

    HM.resize(config.hm_size, std::vector<Sensor>(max_sensors, Sensor()));
    num_sensors.resize(config.hm_size);
    coverage_ratios.resize(config.hm_size);
    objectives.resize(config.hm_size);

    this->i_worst = -1;
    this->i_best = -1;
}

void HarmonySearch::init_harmony_search() {
    this->init_random_harmony_memory();
    int iteration = 0;
    this->cout_harmony_memory(iteration);

    while (iteration < config.num_iterations) {
        std::vector<Sensor> new_sensors(max_sensors, Sensor());
        int n_sensors = 0;

        for (int s = 0; s < max_sensors; s++) {

            if (Random::random() <= Constants::HMCR) {
                new_sensors[s] = memory_consideration(s);

                if (new_sensors[s].active && Random::random() <= Constants::PAR) {
                    new_sensors[s] = pitch_adjustment(new_sensors[s]);
                }
                
            } else { 
                new_sensors[s] = random_consideration();
            }

            if (new_sensors[s].active) n_sensors++;
        }

        if (n_sensors >= min_sensors) {
            double coverage_ratio = calculate_coverage_ratio(new_sensors);
            double new_objective = calculate_objective(new_sensors, n_sensors, coverage_ratio);
            
            if (new_objective >= objectives[i_worst]) {
                HM[i_worst] = new_sensors;
                objectives[i_worst] = new_objective;
                num_sensors[i_worst] = n_sensors;
                coverage_ratios[i_worst] = coverage_ratio;

                update_best_and_worst_index();
            }
        }

        if (iteration % 500 == 0) this->cout_harmony_memory(iteration);

        iteration++;
    }
    this->cout_harmony_memory(iteration);
}

void HarmonySearch::init_random_harmony_memory() {
    for (int i = 0; i < HM.size(); i++) {
        std::vector<Sensor> new_sensors(max_sensors, Sensor());
        int n_sensors = Random::random_value(min_sensors, max_sensors);

        for (int s = 0; s < n_sensors; s++) {
            new_sensors[s] = random_consideration();
        }

        std::shuffle(begin(new_sensors), end(new_sensors), Random::rng);

        HM[i] = new_sensors;
        num_sensors[i] = n_sensors;
        coverage_ratios[i] = calculate_coverage_ratio(new_sensors);
        objectives[i] = calculate_objective(new_sensors, num_sensors[i], coverage_ratios[i]);

        update_best_and_worst_index(i);
    }
}

double HarmonySearch::calculate_objective(std::vector<Sensor> sensors, int n_sensors, double c_ratio) {
    double min_dist_between_sensors = Sensor::min_dist(sensors, config.W, config.H);
    const int k_const = 32;

    return ((double)1 / sqrt(n_sensors)) * pow(c_ratio, 3) * min_dist_between_sensors * k_const;
}

Sensor HarmonySearch::memory_consideration(int s) {
    int i = Random::random_value(0, HM.size() - 1);
    
    return HM[i][s];
}

Sensor HarmonySearch::pitch_adjustment(Sensor new_sensor) {
    if (Random::random() <= 0.5) {
        new_sensor.x += (Random::random() * Constants::BW);
    } else {
        new_sensor.x -= (Random::random() * Constants::BW);
    } 

    if (Random::random() <= 0.5) {
        new_sensor.y += (Random::random() * Constants::BW);
    } else {
        new_sensor.y -= (Random::random() * Constants::BW);
    }

    return new_sensor;
}

Sensor HarmonySearch::random_consideration() {
    return Sensor::random_sensor();
}

void HarmonySearch::update_best_and_worst_index() {
    for (int i = 0; i < HM.size(); i++) {
        if (objectives[i] > objectives[i_best]) {
            i_best = i;
        }
        if (objectives[i] < objectives[i_worst]) {
            i_worst = i;
        }
    }
}

void HarmonySearch::update_best_and_worst_index(int i) {
    if (i_worst == -1 || objectives[i] < objectives[i_worst]) {
        i_worst = i;
    }
    if (i_best == -1 || objectives[i] > objectives[i_best]) {
        i_best = i;
    }
}

double HarmonySearch::calculate_coverage_ratio(std::vector<Sensor> sensors) {
    int num_covered_pois = 0;
    for (PointOfInterest p : config.pois) {
        if (p.is_covered_by_at_least_one(sensors)) num_covered_pois++;
    }
    return (double)num_covered_pois / config.pois.size();
}

void HarmonySearch::cout_harmony_memory(int iteration) {
    std::cout.precision(4);
    std::cout << std::fixed;

    std::cout << "coverage ratios: " << coverage_ratios[i_best] << " | ";
    std::cout << "sensors: " << num_sensors[i_best] << " | ";
    std::cout << "objective: " << objectives[i_best] << " | ";
    std::cout << iteration << "/"<< config.num_iterations << std::endl;
    std::cout << std::endl;

    if (iteration == config.num_iterations) {
        std::cout << "max: " << max_sensors << " min: " << min_sensors << std::endl;
        std::cout << "Sensors: " << std::endl;
        Sensor::cout_sensors(HM[i_best]);
    }
    std::cout << std::endl;
}
