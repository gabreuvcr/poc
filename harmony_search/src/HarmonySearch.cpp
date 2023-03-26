#include "HarmonySearch.hpp"
#include "Constants.hpp"
#include "Random.hpp"

HarmonySearch::HarmonySearch(HarmonySearchConfig config, std::vector<PointOfInterest> pois) {
    this->config = config;
    this->pois = pois;

    this->calculated_min_sensors = ceil(
        (config.W / (2 * Sensor::max_radius)) * (config.H / (2 * Sensor::max_radius))
    );
    this->calculated_max_sensors = ceil(
        (config.W / (2 * Sensor::min_radius)) * (config.H / (2 * Sensor::min_radius))
    );
    this->min_sensors = calculated_min_sensors;
    this->max_sensors = calculated_max_sensors;

    HM.resize(config.mem_size);
    num_sensors.resize(config.mem_size);
    coverage_ratios.resize(config.mem_size);
    objectives.resize(config.mem_size);
}

HarmonySearch::HarmonySearch(HarmonySearchConfig config, std::vector<PointOfInterest> pois, int num_fixed_sensors)
 : HarmonySearch(config, pois) {
    this->min_sensors = num_fixed_sensors;
    this->max_sensors = num_fixed_sensors;
}

void HarmonySearch::set_num_fixed_sensors(int num_sensors) {
    this->min_sensors = num_sensors;
    this->max_sensors = num_sensors;
}

double HarmonySearch::run() {
    this->init_random_harmony_memory();
    int iteration = 0;

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
        iteration++;
    }
    return coverage_ratios[i_best];
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
    }
    update_best_and_worst_index();
}

double HarmonySearch::calculate_objective(std::vector<Sensor> sensors, int n_sensors, double c_ratio) {
    double min_dist_between_sensors = Sensor::min_dist(sensors, config.W, config.H);
    const int k_const = 32;

    return ((double)1 / n_sensors) * c_ratio * min_dist_between_sensors * k_const;
}

Sensor HarmonySearch::memory_consideration(int s) {
    int i = Random::random_value(0, HM.size() - 1);
    
    return HM[i][s];
}

Sensor HarmonySearch::pitch_adjustment(Sensor new_sensor) {
    new_sensor.x += (Random::random() <= 0.5 ? -1 : 1) * (Random::random() * Constants::BW);
    new_sensor.y += (Random::random() <= 0.5 ? -1 : 1) * (Random::random() * Constants::BW);

    return new_sensor;
}

Sensor HarmonySearch::random_consideration() {
    return Sensor::random_sensor();
}

void HarmonySearch::update_best_and_worst_index() {
    i_best = i_worst = 0;
    for (int i = 0; i < HM.size(); i++) {
        if (objectives[i] > objectives[i_best])  i_best  = i;
        if (objectives[i] < objectives[i_worst]) i_worst = i;
    }
}

double HarmonySearch::calculate_coverage_ratio(std::vector<Sensor> sensors) {
    int num_covered_pois = 0;
    for (PointOfInterest poi : pois) {
        if (poi.joint_coverage(sensors) > Constants::COVERAGE_THRESHOLD) {
            num_covered_pois++;
        }
    }
    return (double)num_covered_pois / pois.size();
}
